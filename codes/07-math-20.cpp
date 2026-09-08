// =============================================================================
// File:     07-math-20 — 阶与原根 (Multiplicative Order and Primitive Root)
// =============================================================================
// 阶 ord_m(a)：gcd(a, m) = 1 时，使 a^k ≡ 1 (mod m) 的最小正整数 k。
// 性质：ord_m(a) 整除 phi(m)（欧拉定理推论）；a^k ≡ 1 当且仅当 ord_m(a) | k。
// 原根：阶恰为 phi(m) 的元素 g。模 m 存在原根 <=> m ∈ {1, 2, 4, p^e, 2p^e}；
//       有原根时原根恰有 phi(phi(m)) 个，且最小原根通常非常小（质数模下一般 <= 100）。
// 时间复杂度：试除分解 O(sqrt(m))；求阶 / 判定原根 多做若干次快速幂，O(log^2 m) 级别。

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// 快速幂：a^b mod m
ll fpow(ll a, ll b, ll m) {
    ll res = 1;
    a %= m;
    while (b > 0) {
        if (b & 1) res = res * a % m;
        a = a * a % m;
        b >>= 1;
    }
    return res;
}

// 试除法求欧拉函数 O(sqrt(m))，注意先除后乘防溢出
ll euler_phi(ll n) {
    ll res = n;
    for (ll i = 2; i * i <= n; i++)
        if (n % i == 0) {
            res = res / i * (i - 1);
            while (n % i == 0) n /= i;
        }
    if (n > 1) res = res / n * (n - 1);
    return res;
}

// 试除法分解质因数，返回去重后的质因子列表（升序）
vector<ll> factorize(ll n) {
    vector<ll> ps;
    for (ll i = 2; i * i <= n; i++)
        if (n % i == 0) {
            ps.push_back(i);
            while (n % i == 0) n /= i;
        }
    if (n > 1) ps.push_back(n);
    return ps;
}

// 求阶：要求 gcd(a, m) = 1
// ord 整除 phi(m)：从 x = phi(m) 开始，对每个质因子 p 反复试除——
// 只要 a^(x/p) ≡ 1 就把 x 缩小为 x/p，最终剩下的 x 即最小阶
ll order(ll a, ll m) {
    ll x = euler_phi(m);
    for (ll p : factorize(x))
        while (x % p == 0 && fpow(a, x / p, m) == 1)
            x /= p;
    return x;
}

// 原根判定：g 是模 m 的原根 <=> gcd(g, m) = 1 且
// 对 phi(m) 的每个质因子 q，g^(phi/q) ≢ 1 (mod m)
bool is_primitive_root(ll g, ll m) {
    if (__gcd(g, m) != 1) return false;
    ll ph = euler_phi(m);
    for (ll q : factorize(ph))
        if (fpow(g, ph / q, m) == 1) return false;
    return true;
}

// 求模 m 的最小原根；模 m 不存在原根时返回 -1
// 从小到大枚举并判定，实践中枚举个数极少
ll min_primitive_root(ll m) {
    if (m == 1) return 0;          // 约定模 1 的原根为 0
    if (m == 2) return 1;
    ll ph = euler_phi(m);
    vector<ll> ps = factorize(ph);
    for (ll g = 2; g < m; g++) {
        if (__gcd(g, m) != 1) continue;
        bool ok = true;
        for (ll q : ps)
            if (fpow(g, ph / q, m) == 1) { ok = false; break; }
        if (ok) return g;
    }
    return -1;                     // 仅 m ∈ {1, 2, 4, p^e, 2p^e} 才有原根
}

// -------- 使用示例 (main) --------
// int main() {
//     ll m = 998244353;
//     cout << min_primitive_root(m) << "\n";   // 3（NTT 模数的常用原根）
//     cout << order(3, m) << "\n";             // 998244352 = phi(998244353)
//     cout << is_primitive_root(3, m) << "\n"; // 1
//     cout << order(2, 7) << "\n";             // 3：2^3 = 8 ≡ 1 (mod 7)
//     cout << min_primitive_root(15) << "\n";  // -1：15 = 3*5 不含 4 与奇素数幂，无原根
//     return 0;
// }
