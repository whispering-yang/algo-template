// =============================================================================
// File:     07-math-02  — NTT (Number Theoretic Transform)
// Style:     array-based, constexpr size, unified bit_reversal + ntt interface
// =============================================================================

#include <bits/stdc++.h>
using namespace std;

constexpr int64_t MOD = 998244353;   // 质数模数, 原根为 3
constexpr int64_t G = 3;             // 原根

int64_t fpow(int64_t a, int64_t b) {
    int64_t ans = 1;
    while (b > 0) {
        if (b & 1) ans = ans * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return ans;
}

// 蝴蝶重排 (二进制逆序)
// 适用于 std::array / std::vector / 原生数组等随机访问容器
void bit_reversal(auto &f, int len) {
    static vector<int> rev;
    rev.resize(len);
    for (int i = 0; i < len; i++) {
        rev[i] = rev[i >> 1] >> 1;
        if (i & 1) rev[i] |= len >> 1;
    }
    for (int i = 0; i < len; i++) {
        if (i < rev[i]) swap(f[i], f[rev[i]]);
    }
}

// NTT 迭代实现
//   d =  1 : 正变换 (系数 -> 点值)
//   d = -1 : 逆变换 (点值 -> 系数)
void ntt(auto &f, int len, int d) {
    bit_reversal(f, len);
    for (int h = 2; h <= len; h <<= 1) {
        int64_t wn = fpow(G, (d == 1) ? ((MOD - 1) / h)
                                      : (MOD - 1 - (MOD - 1) / h));
        for (int j = 0; j < len; j += h) {
            int64_t w = 1;
            for (int k = j; k < j + h / 2; k++) {
                int64_t u = f[k];
                int64_t t = w * f[k + h / 2] % MOD;
                f[k] = (u + t) % MOD;
                f[k + h / 2] = (u - t + MOD) % MOD;
                w = w * wn % MOD;
            }
        }
    }
    if (d == -1) {
        int64_t inv_len = fpow(len, MOD - 2);
        for (int i = 0; i < len; i++) {
            f[i] = f[i] * inv_len % MOD;
        }
    }
}

// -------- 使用示例 (main) --------
// 计算两个多项式的卷积
// int main() {
//     int n, m;
//     cin >> n >> m;
//     n++, m++;
//
//     constexpr int MAXN = 1 << (32 - __builtin_clz(unsigned(2e6)));
//     array<int64_t, MAXN> fs{}, gs{};
//
//     for (int i = 0; i < n; i++) cin >> fs[i];
//     for (int i = 0; i < m; i++) cin >> gs[i];
//
//     int len = 1 << (32 - __builtin_clz(unsigned(n + m - 1)));
//
//     ntt(fs, len, 1);
//     ntt(gs, len, 1);
//     for (int i = 0; i < len; i++) fs[i] = fs[i] * gs[i] % MOD;
//     ntt(fs, len, -1);
//
//     for (int i = 0; i < n + m - 1; i++) cout << fs[i] << ' ';
//     cout << '\n';
//     return 0;
// }
