// =============================================================================
// File:     07-math-23 — 中国剩余定理 CRT (Chinese Remainder Theorem)
// =============================================================================
// 求解同余方程组 x ≡ r[i] (mod m[i])（模数两两互质），返回 [0, M) 内的唯一解，
// 其中 M = ∏ m[i]。
// 前提：0 <= r[i] < m[i]；模数两两互质；乘积 M 不超出 int64_t 范围。
// 构造：x = Σ r[i] * c[i] * t[i] (mod M)，其中 c[i] = M / m[i]，t[i] 为
//       c[i] 在模 m[i] 下的逆元。代入第 j 个方程时，i ≠ j 的项都含因子
//       m[j] 而消失，只剩 r[j] * c[j] * t[j] ≡ r[j] (mod m[j])。
// 时间复杂度：O(n log M)，每个方程一次扩展欧几里得求逆元。

#include <bits/stdc++.h>
using namespace std;

// 求 a*x + b*y = gcd(a, b) 的一组整数特解 (x, y)，返回 gcd(a, b)
int64_t exgcd(int64_t a, int64_t b, int64_t& x, int64_t& y) {
    if (b == 0) { x = 1; y = 0; return a; }
    int64_t x1, y1;
    int64_t g = exgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// 中国剩余定理：模数 m[] 两两互质，返回 x ≡ r[i] (mod m[i]) 在 [0, M) 内的唯一解
int64_t crt(const vector<int64_t>& r, const vector<int64_t>& m) {
    int64_t M = 1;
    for (int64_t mi : m) M *= mi;
    int64_t x = 0;
    for (size_t i = 0; i < m.size(); i++) {
        int64_t ci = M / m[i];              // 其余模数之积，与 m[i] 互质
        int64_t x1, y1;
        exgcd(ci, m[i], x1, y1);            // 解 c[i]*x1 + m[i]*y1 = 1
        int64_t ti = (x1 % m[i] + m[i]) % m[i];  // t[i] = c[i]^(-1) mod m[i]
        // 单项 r[i]*ci*ti 最高约 M*m[i]，必须用 __int128 相乘再取模
        x = (int64_t)((x + (__int128)r[i] * ci * ti) % M);
    }
    return x;
}

// -------- 使用示例 (main) --------
// int main() {
//     // 「物不知数」：x ≡ 2 (mod 3), x ≡ 3 (mod 5), x ≡ 2 (mod 7)
//     cout << crt({2, 3, 2}, {3, 5, 7}) << "\n";   // 23
//     // x ≡ 1 (mod 4), x ≡ 2 (mod 9), x ≡ 3 (mod 25)，M = 900
//     cout << crt({1, 2, 3}, {4, 9, 25}) << "\n";  // 353
//     return 0;
// }
