// =============================================================================
// File:     06-math-07 — Sieve of Eratosthenes (埃氏筛)
// =============================================================================

#include <bits/stdc++.h>
using namespace std;

// 埃氏筛 (Sieve of Eratosthenes)
// 筛出 [2, n] 内所有质数，时间复杂度 O(n log log n)
vector<int> sieveEratosthenes(int n) {
    vector<bool> isPrime(n + 1, true);
    if (n >= 0) isPrime[0] = false;
    if (n >= 1) isPrime[1] = false;

    for (int i = 2; 1LL * i * i <= n; ++i) {
        if (isPrime[i]) {
            // 从 i*i 开始标记：小于 i*i 的倍数 i*2, i*3, ...
            // 都含有更小的质因子，已被更早的质数标记过
            for (long long j = 1LL * i * i; j <= n; j += i) {
                isPrime[(int)j] = false;
            }
        }
    }

    vector<int> primes;
    for (int i = 2; i <= n; ++i) {
        if (isPrime[i]) primes.push_back(i);
    }
    return primes;
}

// -------- 使用示例 (main) --------
// int main() {
//     int n = 100;
//     vector<int> primes = sieveEratosthenes(n);
//     cout << "primes in [2, " << n << "]: " << primes.size() << " 个\n";
//     for (int p : primes) cout << p << " ";
//     cout << "\n";
//     return 0;
// }
