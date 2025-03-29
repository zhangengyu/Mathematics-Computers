import pandas as pd

# 文件路径
file1 = r"C:\Users\詹耿羽\Desktop\新建 XLSX 工作表.xlsx"
file2 = r"C:\Users\詹耿羽\Desktop\新建 XLSX 工作表 (3).xlsx"

# 读取两个表格，从第二行开始读取数据
df1 = pd.read_excel(file1, header=1)  # header=1 表示第一行（索引从 0 开始）作为列名
df2 = pd.read_excel(file2, header=1)

# 合并两个表格
combined_df = pd.concat([df1, df2])

# 指定姓名
target_name = "x1"

# 统计目标姓名的总金额
total_money = combined_df.loc[combined_df['姓名'] == target_name, '钱'].sum()

print(f"姓名为 {target_name} 的总金额为：{total_money}")