import random

# ---------- 播种 ----------
random.seed(20260901)  # 固定种子使结果可复现，对拍/调试时推荐
# random.seed()        # 不传参则由系统熵源自动播种

# ---------- 整数 ----------
random.randint(1, 100)       # [1, 100] 闭区间随机整数
random.randrange(0, 100, 2)  # [0, 100) 内步长为 2 的随机整数（不含上界）
random.getrandbits(64)       # 随机 64 位整数，取大随机数时比 randint 快

# ---------- 浮点数 ----------
random.random()              # [0.0, 1.0) 均匀浮点数
random.uniform(2.5, 10.0)    # [2.5, 10.0] 均匀浮点数

# ---------- 序列操作 ----------
a = [1, 2, 3, 4, 5]
random.shuffle(a)                   # 原地打乱序列
random.choice(a)                    # 等概率取一个元素
random.choices(a, k=3)              # 有放回抽样 3 次，可传 weights=[...] 加权
random.sample(a, 3)                 # 无放回抽样，取出的元素互不重复

# ---------- 常用技巧 ----------
n, m = 10**5, 10**9

# 批量生成 n 个 [1, m] 的随机数：choices 由 C 实现，比循环调 randint 快数倍
arr = random.choices(range(1, m + 1), k=n)

# [1, m] 中随机取 k 个互不相同的数（range 惰性求值，不占额外内存）
ids = random.sample(range(1, m + 1), k=1000)

# 随机树：节点 i 的父节点在 [1, i-1] 中随机选取
fa = [0] * (n + 1)
for i in range(2, n + 1):
    fa[i] = random.randint(1, i - 1)

# ---------- 防 hack ----------
# random 模块是确定性伪随机（梅森旋转），输出序列可被预测；
# 对抗性场景改用 SystemRandom（读系统熵源，不可预测，但更慢）
sr = random.SystemRandom()
sr.randint(1, 10**9)
