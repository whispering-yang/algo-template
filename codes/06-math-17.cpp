// =============================================================================
// File:     06-math-17 — Euler's Totient, Trial Division (试除法求欧拉函数)
// =============================================================================

#include <bits/stdc++.h>
using namespace std;

// 单点求欧拉函数 phi(n)
// 按通项公式 phi(n) = n * prod(1 - 1/p)，在试除分解质因数的同时累乘
// 时间复杂度 O(sqrt(n))，适用于 n 高达 1e18 级别的单次查询
int64_t eulerPhi(int64_t n) {
    int64_t result = n;
    for (int64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            result = result / p * (p - 1);   // 先除后乘，防止中间结果溢出
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) result = result / n * (n - 1); // 剩余的大于 sqrt(n) 的质因子
    return result;
}

// -------- 使用示例 (main) --------
// int main() {
//     cout << eulerPhi(1) << "\n";         // 1
//     cout << eulerPhi(12) << "\n";        // 4  (1, 5, 7, 11)
//     cout << eulerPhi(998244353) << "\n"; // 998244352 (质数)
//     return 0;
// }
