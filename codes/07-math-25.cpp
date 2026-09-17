// =============================================================================
// File:     07-math-25 — 狄利克雷前缀和与后缀和 (Dirichlet Prefix and Suffix Sum)
// =============================================================================
// 整除偏序上的快速求和：把每个下标 m 看作质因子指数向量，d | m 即逐维指数
// 都不超过，故「约数和 / 倍数和」可像高维前缀和 (SOS) 一样按质数逐维累加。
// 用法：先线性筛出 [2, n] 的质数表，再对下标 1..n 的数组 a（a[0] 不使用）
// 做下列变换之一。四个变换均原地、均可逆：逆变换是同一骨架的差分——
// 把 += 换成 -= 且内层反方向枚举（保证读取的一侧仍是原值）。链式累加的
// 逆变换若不反转方向，深度 >= 3 的链会算成交错和。即莫比乌斯反演方向。
// 时间 O(n log log n)，空间 O(n)。
// 模意义下使用时，在每次 += / -= 后追加一步取模即可（单步加减不会溢出）。

#include <bits/stdc++.h>
using namespace std;

// 线性筛：返回 [2, n] 内所有质数（升序）
vector<int> linear_sieve_primes(int n) {
    vector<int> primes;
    vector<bool> is_composite(n + 1, false);
    for (int i = 2; i <= n; ++i) {
        if (!is_composite[i]) primes.push_back(i);
        for (int p : primes) {
            if (int64_t(i) * p > n) break;   // 判界用 int64_t 防 i*p 溢出
            is_composite[i * p] = true;
            if (i % p == 0) break;           // 每个合数只被最小质因子筛一次
        }
    }
    return primes;
}

// 狄利克雷前缀和（原地）：a[i] <- sum_{d | i} a[d]，即与全 1 函数的 Dirichlet 卷积 f * 1
void dirichlet_prefix_sum(vector<int64_t>& a, int n, const vector<int>& primes) {
    for (int p : primes) {
        if (p > n) break;
        for (int i = 1; i * p <= n; ++i)
            a[i * p] += a[i];        // 沿质数 p 的指数维做前缀和（i 递增保证级联）
    }
}

// 前缀和的逆变换（原地）：已知 b[i] = sum_{d|i} f[d]，还原 f；
// 即 f(i) = sum_{d|i} mu(i/d) * b(d)，无需显式筛 mu。
// 内层倒序：读取 a[i] 时 i 尚未被本趟更新（仍为原值），才是正确的差分
void dirichlet_prefix_inv(vector<int64_t>& a, int n, const vector<int>& primes) {
    for (int p : primes) {
        if (p > n) break;
        for (int i = n / p; i >= 1; --i)
            a[i * p] -= a[i];        // 逐维差分
    }
}

// 狄利克雷后缀和（原地）：a[i] <- sum_{i | d, d <= n} a[d]
void dirichlet_suffix_sum(vector<int64_t>& a, int n, const vector<int>& primes) {
    for (int p : primes) {
        if (p > n) break;
        for (int i = n / p; i >= 1; --i)
            a[i] += a[i * p];        // 沿质数 p 的指数维做后缀和（i 递减保证级联）
    }
}

// 后缀和的逆变换（原地）：已知 b[i] = sum_{i|d} f[d]，还原 f。
// 内层正序：读取 a[i*p] 时其倍数一侧尚未被本趟更新（仍为原值）
void dirichlet_suffix_inv(vector<int64_t>& a, int n, const vector<int>& primes) {
    for (int p : primes) {
        if (p > n) break;
        for (int i = 1; i * p <= n; ++i)
            a[i] -= a[i * p];
    }
}

// -------- 使用示例 (main) --------
// int main() {
//     int n = 12;
//     vector<int> primes = linear_sieve_primes(n);
//     vector<int64_t> a = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
//     dirichlet_prefix_sum(a, n, primes);       // a[i] = sigma(i) = 约数和
//     for (int i = 1; i <= n; ++i) cout << a[i] << " ";
//     cout << "\n";  // 1 3 4 7 6 12 8 15 13 18 12 28
//     dirichlet_prefix_inv(a, n, primes);       // 还原 1..12
//     dirichlet_suffix_sum(a, n, primes);       // a[i] = i 的所有倍数(<=n)之和
//     for (int i = 1; i <= n; ++i) cout << a[i] << " ";
//     cout << "\n";  // 78 42 30 24 15 18 7 8 9 10 11 12
//     return 0;
// }
