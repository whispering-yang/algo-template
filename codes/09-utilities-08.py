import os

gen = "gen"        # 随机数据生成程序
my  = "my"         # 你的程序
std = "std"        # 标准程序

# 如需自动编译
for p in (gen, sol, std):
    os.system(f"g++ --std=gnu++20 -O2 {p}.cpp -o {p}")

i = 0
while True:
    i += 1
    os.system(f"{gen} > in")
    out1 = os.popen(f"{my} < in").read().strip()
    out2 = os.popen(f"{std} < in").read().strip()
    if out1 != out2:
        print(f"WA on test {i}")
        print("Input:", open("in").read())
        print("Your:", out1)
        print("Std :", out2)
        # 保存输出到文件
        open(f"{sol}_out.txt", "wt").write(out1)
        open(f"{std}_out.txt", "wt").write(out2)
        break
    print(f"Test {i} AC")
