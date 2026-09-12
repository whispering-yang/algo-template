// =============================================================================
// File:     07-math-22 — 扩展大步小步算法 exBSGS (Extended Baby-Step Giant-Step)
// =============================================================================
// 求解离散对数：a^x ≡ b (mod m)，不要求 gcd(a, m) = 1，返回最小非负整数解 x，
// 无解返回 -1；gcd(a, m) = 1 时自动退化为普通 BSGS。
// 调用前提：0 <= a, b < m。入口不做取模：只要首次调用时 a、b 不越界，
// 中间乘积均不超过 m^2，后续同样不会越界，省去每次入口取模的常数。
// 思路：不断约去 a 与 m 的公因子。设 g = gcd(a, m) > 1，若解存在则必有 g | b
//       （否则无解）；把同余式两边与模数同除以 g，得
//       a^(x-1) * (a/g) ≡ b/g (mod m/g)。每约一步，左侧多出累积系数
//       d *= a/g（记步数 cnt），直到 gcd(a, m) = 1。此时方程化为
//       a^(x-cnt) ≡ b' * d^(-1) (mod m')，令 y = x - cnt 用普通 BSGS 解出 y，
//       答案即 y + cnt。约分途中若 d == b，则 x = cnt 已是解，直接返回——
//       这一步同时保证不会漏掉更小的解。
// 时间复杂度：约去公因子每步 m 至少减半，至多 O(log m) 步；主体仍为 O(sqrt(m))。

#include <bits/stdc++.h>
using namespace std;

// 快速幂：a^b mod m
int64_t fpow(int64_t a, int64_t b, int64_t m) {
    int64_t res = 1;
    a %= m;
    while (b > 0) {
        if (b & 1) res = res * a % m;
        a = a * a % m;
        b >>= 1;
    }
    return res;
}

// 扩展欧几里得：解 a*x + b*y = gcd(a, b)
int64_t exgcd(int64_t a, int64_t b, int64_t &x, int64_t &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    int64_t g = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return g;
}

// d 模 m 的乘法逆元（要求 gcd(d, m) = 1）
int64_t inv(int64_t d, int64_t m) {
    int64_t x, y;
    exgcd((d % m + m) % m, m, x, y);
    return (x % m + m) % m;
}

// 普通 BSGS（要求 gcd(a, m) = 1、0 <= b < m；a 可不约简）：内部版，供 exbsgs 调用
int64_t bsgs(int64_t a, int64_t b, int64_t m) {
    if (m == 1) return 0;
    if (b == 1) return 0;
    int64_t t = 1;
    while (t * t < m) ++t;
    int64_t cur = b;
    unordered_map<int64_t, int64_t> bs;
    for (int64_t j = 0; j < t; j++) {
        bs[cur] = j;                // 重复值取更大的 j，保证最终解最小
        cur = cur * a % m;
    }
    int64_t step = fpow(a, t, m);
    cur = 1;
    for (int64_t i = 1; i <= t; i++) {
        cur = cur * step % m;
        if (bs.count(cur)) return i * t - bs[cur];
    }
    return -1;
}

// a ^ x ≡ b (mod m)：不要求 a 与 m 互质，要求 0 <= a, b < m；返回最小非负解 x，无解返回 -1
int64_t exbsgs(int64_t a, int64_t b, int64_t m) {
    if (m == 1 || b == 1) return 0;
    int64_t cnt = 0, d = 1;
    while (true) {
        int64_t g = std::gcd(a, m);
        if (g == 1) break;              // 已互质，转普通 BSGS
        if (b % g != 0) return -1;      // g 不整除 b，无解
        b /= g, m /= g;                 // b、m 同除 g 后仍满足 b < m
        d = d * (a / g) % m;            // 方程变为 a^(x-cnt) * d ≡ b (mod m)
        ++cnt;
        if (d == b) return cnt;         // x = cnt 恰为解，且不会再有更小的
    }
    int64_t y = bsgs(a, b * inv(d, m) % m, m);  // 解 a^y ≡ b * d^(-1) (mod m)
    return y == -1 ? -1 : y + cnt;
}

// -------- 使用示例 (main) --------
// int main() {
//     cout << exbsgs(2, 4, 6) << "\n";    // 2：2^2 = 4 ≡ 4 (mod 6)
//     cout << exbsgs(2, 6, 10) << "\n";   // 4：2^4 = 16 ≡ 6 (mod 10)
//     cout << exbsgs(2, 0, 8) << "\n";    // 3：2^3 = 8 ≡ 0 (mod 8)，b = 0 自动处理
//     cout << exbsgs(2, 3, 6) << "\n";    // -1：gcd(2, 6) = 2 不整除 3
//     return 0;
// }
