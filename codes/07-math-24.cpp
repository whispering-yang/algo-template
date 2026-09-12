// =============================================================================
// File:     07-math-24 — 扩展中国剩余定理 exCRT (Extended CRT)
// =============================================================================
// 求解同余方程组 x ≡ r[i] (mod m[i])，模数不要求互质：方程组有解时返回
// [0, lcm) 内的唯一解（lcm 为所有模数的最小公倍数），矛盾时返回 -1。
// 前提：0 <= r[i] < m[i]；所有模数的 lcm 不超出 int64_t 范围。
// 思路：已合并方程 x ≡ R (mod M) 写成 x = R + k*M，代入新方程 x ≡ r2 (mod m2)
//       得 k*M ≡ r2 - R (mod m2)。设 g = gcd(M, m2)，有解当且仅当 g | (r2-R)；
//       两边与模数同除 g 后 M/g 与 m2/g 互质，扩展欧几里得解出最小非负 k，
//       合并为 x ≡ R + k*M (mod M/g*m2)，模数更新为 lcm。
// 时间复杂度：O(n log max m[i])，每次合并一次扩展欧几里得。

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

// 扩展中国剩余定理：模数不要求互质，返回 [0, lcm) 内的唯一解，无解返回 -1
int64_t excrt(const vector<int64_t>& r, const vector<int64_t>& m) {
    int64_t R = 0, M = 1;                     // 已合并方程：x ≡ R (mod M)
    for (size_t i = 0; i < m.size(); i++) {
        int64_t x, y;
        int64_t g = exgcd(M, m[i], x, y);     // M*x + m[i]*y = g，x 即 (M/g)^(-1)
        if ((r[i] - R) % g != 0) return -1;   // 公因子不整除差，方程组矛盾
        int64_t mm = m[i] / g;                // k 的模数 m[i]/g
        // k = ((r[i]-R)/g)*x (mod mm)；C++ 取模带符号，先 % 再 + 再 % 移到非负
        int64_t k = (int64_t)(((__int128)(r[i] - R) / g * x % mm + mm) % mm);
        R = (int64_t)(R + (__int128)k * M);   // 新余数自动落在 [0, 新模数) 内
        M = M / g * m[i];                     // 新模数 lcm(M, m[i])，先除后乘防溢出
    }
    return R;
}

// -------- 使用示例 (main) --------
// int main() {
//     // 模数不互质：x ≡ 2 (mod 4), x ≡ 4 (mod 6), x ≡ 3 (mod 7)，lcm = 84
//     cout << excrt({2, 4, 3}, {4, 6, 7}) << "\n";  // 10
//     // 方程组矛盾：x ≡ 2 (mod 4) 与 x ≡ 3 (mod 8) 无公共解
//     cout << excrt({2, 3}, {4, 8}) << "\n";        // -1
//     return 0;
// }
