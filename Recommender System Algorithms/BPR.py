import numpy as np
import time
from collections import defaultdict

class BPR:
    def __init__(self, n_users, n_items, dim=20, alpha_u=0.01, alpha_v=0.01, beta_v=0.01, lr=0.01, num_iter=500):
        self.dim = dim
        self.alpha_u = alpha_u
        self.alpha_v = alpha_v
        self.beta_v = beta_v
        self.lr = lr
        self.num_iter = num_iter
        
        # 初始化模型参数
        self.U = np.random.normal(scale=0.01, size=(n_users+1, dim))  # 用户矩阵
        self.V = np.random.normal(scale=0.01, size=(n_items+1, dim))  # 物品矩阵
        self.biasV = np.zeros(n_items+1)  # 物品偏置

    def fit(self, train_data, initial_data, item_set, initial_item_set):
        # 初始化物品偏置
        item_counts = defaultdict(int)
        for u, items in train_data.items():
            for i in items:
                item_counts[i] += 1
                
        global_avg = sum(item_counts.values()) / (len(train_data) * len(item_set))
        for i in item_set:
            self.biasV[i] = item_counts[i]/len(train_data) - global_avg

        # 准备训练样本
        samples = []
        for u, items in train_data.items():
            for i in items:
                samples.append((u, i))
        samples = np.array(samples)

        # 开始训练
        for it in range(self.num_iter):
            np.random.shuffle(samples)
            for u, i in samples:
                # 采样负样本
                j = np.random.choice(list(item_set))
                while j in train_data[u]:
                    j = np.random.choice(list(item_set))
                
                # 计算得分差
                x_uij = self.biasV[i] - self.biasV[j]
                x_uij += np.dot(self.U[u], self.V[i] - self.V[j])
                
                # 计算梯度
                coeff = -1 / (1 + np.exp(x_uij))
                
                # 更新参数
                grad_U = coeff * (self.V[i] - self.V[j]) + self.alpha_u * self.U[u]
                grad_Vi = coeff * self.U[u] + self.alpha_v * self.V[i]
                grad_Vj = -coeff * self.U[u] + self.alpha_v * self.V[j]
                
                self.U[u] -= self.lr * grad_U
                self.V[i] -= self.lr * grad_Vi
                self.V[j] -= self.lr * grad_Vj
                
                # 更新偏置
                self.biasV[i] -= self.lr * (coeff + self.beta_v * self.biasV[i])
                self.biasV[j] -= self.lr * (-coeff + self.beta_v * self.biasV[j])

    def evaluate(self, test_data, train_data, initial_data, topk=5):
        precision_sum = 0.0
        recall_sum = 0.0
        user_count = 0

        for u in test_data:
            if u not in train_data:
                continue
                
            # 获取候选物品
            train_items = set(initial_data[u])
            candidates = [i for i in self.biasV.keys() if i != 0 and i not in train_items]
            
            # 计算得分
            scores = {}
            for i in candidates:
                scores[i] = np.dot(self.U[u], self.V[i]) + self.biasV[i]
            
            # 取TopK
            top_items = [i for i, _ in sorted(scores.items(), key=lambda x: -x[1])[:topk]]
            
            # 计算指标
            test_items = test_data[u]
            hits = len(set(top_items) & set(test_items))
            
            precision = hits / topk
            recall = hits / len(test_items) if len(test_items) > 0 else 0
            
            precision_sum += precision
            recall_sum += recall
            user_count += 1

        return precision_sum / user_count, recall_sum / user_count

def read_data(filename, threshold=5):
    data = defaultdict(set)
    initial_data = defaultdict(set)
    items = set()
    initial_items = set()
    
    with open(filename, 'r') as f:
        for line in f:
            parts = line.strip().split('\t')
            u = int(parts[0])
            i = int(parts[1])
            rating = float(parts[2])
            
            if rating > 0:
                initial_data[u].add(i)
                initial_items.add(i)
                
            if rating >= threshold:
                data[u].add(i)
                items.add(i)
                
    return data, initial_data, items, initial_items

if __name__ == '__main__':
    # 数据路径
    train_file = r"E:\datasets\ml-100k\u1.base"
    test_file = r"E:\datasets\ml-100k\u1.test"

    # 读取数据
    train_data, initial_train, train_items, initial_train_items = read_data(train_file)
    test_data, _, _, _ = read_data(test_file, threshold=1)  # 测试集所有评分视为正样本

    # 获取用户和物品数量
    n_users = max(max(train_data.keys()), max(test_data.keys())) if test_data else max(train_data.keys())
    n_items = max(max(train_items), max(initial_train_items)) if train_items else 0

    # 训练模型
    start = time.time()
    model = BPR(n_users, n_items)
    model.fit(train_data, initial_train, train_items, initial_train_items)
    print(f"Training time: {time.time()-start:.2f}s")

    # 评估模型
    precision, recall = model.evaluate(test_data, train_data, initial_train, topk=5)
    print(f"Precision@5: {precision:.4f}")
    print(f"Recall@5: {recall:.4f}")
