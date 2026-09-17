// =============================================================================
// File:     07-math-26 — 莫比乌斯反演 (Möbius Function and Möbius Inversion)
// =============================================================================
// 线性筛求莫比乌斯函数 mu 及其前缀和，并用「反演 + 整除分块」求解经典问题：
//     ans(k) = #{(i, j) : 1 <= i <= n, 1 <= j <= m, gcd(i, j) = k}
// 推导：设 f(t) = #{t | gcd(i, j)} = floor(n/t) * floor(m/t)，g(t) = #{gcd = t}，
// 则 f(t) = sum_{t | d} g(d)，由倍数形式反演 g(k) = sum_{k | d} mu(d/k) f(d)，
// 换元 d = k*d' 得 g(k) = sum_{d'=1}^{min(n,m)/k} mu(d') * floor(n/(k*d')) * floor(m/(k*d'))，
// 对两个商同时整除分块，每块内乘 mu 的前缀和，单次询问 O(sqrt(n))。

#include <bits/stdc++.h>
using namespace std;

// 线性筛 mu（积性函数筛法）：
//   i 为质数               : mu[i] = -1
//   p 为 i 的最小质因子    : i*p 含平方因子（p^2 | i*p），mu[i*p] = 0
//   p 与 i 互质            : 积性，mu[i*p] = -mu[i]
// 同时构建 mu 前缀和 mu_prefix，供整除分块 O(1) 区间求和。
void sieve_mu(int n, vector<int> &mu, vector<int> &mu_prefix) {
    mu.assign(n + 1, 0);
    vector<int> primes;
    vector<bool> vis(n + 1, false);
    mu[1] = 1;
    for (int i = 2; i <= n; ++i) {
        if (!vis[i]) {                    // i 是质数
            primes.push_back(i);
            mu[i] = -1;
        }
        for (int p : primes) {
            int64_t nxt = int64_t(i) * p; // 可能超 int，用 int64_t 判界
            if (nxt > n) break;
            vis[nxt] = true;
            if (i % p == 0) {             // p 已是 i 的最小质因子
                mu[nxt] = 0;              // i*p 必含平方因子
                break;
            }
            mu[nxt] = -mu[i];             // 积性：多一个互异质因子
        }
    }
    mu_prefix.assign(n + 1, 0);
    for (int i = 1; i <= n; ++i) mu_prefix[i] = mu_prefix[i - 1] + mu[i];
}

// ans(k) = #{(i, j) : gcd(i, j) = k}
// 调用前提：mu_prefix 已筛到 >= min(n, m) / k；n, m, k >= 1
int64_t count_gcd_pairs(int n, int m, int k, const vector<int> &mu_prefix) {
    int lim = min(n, m) / k;              // d' 上界：k*d' <= min(n, m)
    int64_t ans = 0;
    for (int l = 1, r; l <= lim; l = r + 1) {
        int tn = n / (k * l), tm = m / (k * l);       // 当前块内两个商
        r = min(lim, min(n / k / tn, m / k / tm));    // 商保持不变的最后 d'
        ans += int64_t(mu_prefix[r] - mu_prefix[l - 1]) * tn * tm;
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> mu, mu_prefix;
    sieve_mu(min(n, m), mu, mu_prefix);

    cout << count_gcd_pairs(n, m, k, mu_prefix) << '\n';
    return 0;
}
