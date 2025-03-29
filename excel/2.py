import pandas as pd

# 文件路径
file1 = r"C:\Users\詹耿羽\Desktop\新建 XLSX 工作表.xlsx"
file2 = r"C:\Users\詹耿羽\Desktop\新建 XLSX 工作表 (3).xlsx"

# 读取两个表格，从第二行开始读取数据
df1 = pd.read_excel(file1, header=1)  # header=1 表示第一行（索引从 0 开始）作为列名
df2 = pd.read_excel(file2, header=1)

# 合并两个表格
combined_df = pd.concat([df1, df2])

# 保留姓名为 x1、x2、x3 的行
valid_names = ['x1', 'x2', 'x3']
filtered_df = combined_df[combined_df['姓名'].isin(valid_names)]

# 输出过滤后的数据
print("过滤后的数据：")
print(filtered_df)

# 如果需要将结果保存到文件，可以使用以下代码：
# filtered_df.to_excel(r"C:\Users\詹耿羽\Desktop\过滤后的结果.xlsx", index=False)