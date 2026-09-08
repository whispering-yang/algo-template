// =============================================================================
// File:     07-math-18 — Linear Sieve for Euler's Totient (线性筛求欧拉函数)
// =============================================================================

#include <bits/stdc++.h>
using namespace std;

const int N = 1e7 + 5;  // 按需调整上界

int phi[N];             // phi[x] 为欧拉函数 phi(x)
int min_prime_factor[N];    // x 的最小质因子；等于 0 当且仅当 x 尚未被筛到（x 为质数）
vector<int> primes;     // [2, n] 内所有质数（升序）

// 欧拉筛：每个合数只被其最小质因子筛掉一次，严格 O(n)
// 同时递推 phi：
//   i 为质数              : phi[i] = i - 1
//   p 为 i 的最小质因子   : phi[i*p] = phi[i] * p      (i*p 与 i 质因子集合相同)
//   p 与 i 互质           : phi[i*p] = phi[i] * (p-1)  (积性)
void sieve(int n) {
    phi[1] = 1;
    for (int i = 2; i <= n; ++i) {
        if (min_prime_factor[i] == 0) {     // i 是质数
            min_prime_factor[i] = i;
            primes.push_back(i);
            phi[i] = i - 1;
        }
        for (int p : primes) {
            int64_t composite = int64_t(i) * p;   // 可能超 int，用 int64_t 判界
            if (composite > n) break;
            min_prime_factor[composite] = p;      // 用最小质因子 p 标记合数 i*p
            if (i % p == 0) {                     // p 已是 i 的最小质因子时停止，
                phi[composite] = phi[i] * p;      // 保证每个合数只被标记一次
                break;
            }
            phi[composite] = phi[i] * (p - 1);
        }
    }
}

// 借助最小质因子数组在 O(log x) 时间内分解 x 的质因数（要求 x <= n）
// 返回 (质因子, 指数) 列表，如 84 -> {(2,2), (3,1), (7,1)}
vector<pair<int, int>> factorize(int x) {
    vector<pair<int, int>> factors;
    while (x > 1) {
        int p = min_prime_factor[x], exponent = 0;
        while (x % p == 0) {
            x /= p;
            ++exponent;
        }
        factors.push_back({p, exponent});
    }
    return factors;
}

// -------- 使用示例 (main) --------
// int main() {
//     int n = 100;
//     sieve(n);
//     for (int i = 1; i <= 12; ++i) cout << phi[i] << " ";
//     cout << "\n";  // 1 1 2 2 4 2 6 4 6 4 10 4
//
//     auto factors = factorize(84);  // 84 = 2^2 * 3 * 7
//     for (auto [p, e] : factors) cout << p << "^" << e << " ";
//     cout << "\n";
//     return 0;
// }
