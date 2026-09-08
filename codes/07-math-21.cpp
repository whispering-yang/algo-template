// =============================================================================
// File:     07-math-21 — 大步小步算法 BSGS (Baby-Step Giant-Step)
// =============================================================================
// 求解离散对数：a^x ≡ b (mod m)，返回最小非负整数解 x，无解返回 -1。
// 前提：gcd(a, m) = 1（a 与 m 不互质时需改用扩展 BSGS exBSGS）。
// 思路：令 t = ceil(sqrt(m))，则任意解 x ∈ [0, m) 可写成 x = i*t - j
//       （大步 i ∈ [1, t]，小步 j ∈ [0, t)）。同余式两边同乘 a^j 得
//       a^(i*t) = b * a^j。先用 O(t) 次乘法把所有 b * a^j 存入哈希表（小步），
//       再枚举 i 检查 a^(i*t) 是否在表中（大步）。
// 时间复杂度：O(sqrt(m)) 次乘法 + 哈希均摊 O(1)，空间 O(sqrt(m))。

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

// a ^ x ≡ b (mod m)：返回最小非负解 x，无解返回 -1
ll bsgs(ll a, ll b, ll m) {
    a %= m, b %= m;
    if (m == 1LL) return 0LL;       // 模 1 下任何数同余于 0，x = 0 即解
    if (b == 1LL) return 0LL;       // a^0 = 1，b ≡ 1 时最小解为 0
    ll t = 1;
    while (t * t < m) ++t;          // 分块大小 t = ceil(sqrt(m))
    ll cur = b;
    unordered_map<ll, ll> bs;
    for (ll j = 0; j < t; j++) {    // 小步：bs[cur] = j 记录 b * a^j
        bs[cur] = j;                // 重复值取更大的 j，保证最终解最小
        cur = cur * a % m;
    }
    ll step = fpow(a, t, m);        // 大步长 a^t，每次大步即乘 step
    cur = 1LL;
    for (ll i = 1; i <= t; i++) {   // 大步：检查 a^(i*t) 是否撞上某个 b * a^j
        cur = cur * step % m;
        if (bs.count(cur)) return i * t - bs[cur];
    }
    return -1;                      // gcd(a, m) = 1 时阶 <= phi(m) < m <= t^2，
}                                   // 故 [0, t^2] 内无解即全局无解

// -------- 使用示例 (main) --------
// int main() {
//     cout << bsgs(3, 13, 17) << "\n";    // 4：3^4 = 81 ≡ 13 (mod 17)
//     cout << bsgs(2, 3, 5) << "\n";      // 3：2^3 = 8 ≡ 3 (mod 5)
//     cout << bsgs(2, 5, 7) << "\n";      // -1：2 的阶为 3，模 7 下取不到 5
//     return 0;
// }
