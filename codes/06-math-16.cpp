// =============================================================================
// File:     06-math-16  — NTT without bit-reversal (DIF + DIT)
// Style:    forward = decimation-in-frequency (natural -> bit-reversed)
//           inverse = decimation-in-time (bit-reversed -> natural)
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

// 正变换: 按频率抽取 (DIF)
// 输入自然序, 输出位逆序, 全程不做位逆序置换
// 层长从大到小: 蝴蝶先加减, 后乘旋转因子
void ntt_dif(auto &f, int len) {
    for (int h = len; h >= 2; h >>= 1) {
        int64_t wn = fpow(G, (MOD - 1) / h);
        for (int i = 0; i < len; i += h) {
            int64_t w = 1;
            for (int k = i; k < i + h / 2; k++) {
                int64_t u = f[k];
                int64_t v = f[k + h / 2];
                f[k] = (u + v) % MOD;
                f[k + h / 2] = (u - v + MOD) % MOD * w % MOD;
                w = w * wn % MOD;
            }
        }
    }
}

// 逆变换: 按时间抽取 (DIT)
// 输入位逆序 (即 DIF 的输出), 输出自然序
// 层长从小到大: 蝴蝶先乘旋转因子, 后加减
void ntt_idit(auto &f, int len) {
    for (int h = 2; h <= len; h <<= 1) {
        int64_t wn = fpow(G, MOD - 1 - (MOD - 1) / h);
        for (int i = 0; i < len; i += h) {
            int64_t w = 1;
            for (int k = i; k < i + h / 2; k++) {
                int64_t u = f[k];
                int64_t t = w * f[k + h / 2] % MOD;
                f[k] = (u + t) % MOD;
                f[k + h / 2] = (u - t + MOD) % MOD;
                w = w * wn % MOD;
            }
        }
    }
    int64_t inv_len = fpow(len, MOD - 2);
    for (int i = 0; i < len; i++) {
        f[i] = f[i] * inv_len % MOD;
    }
}

// -------- 卷积: 正变换用 DIF, 逆变换用 DIT, 位逆序状态只存在于中间 --------
// 要求 len 为 2 的幂, len >= 两多项式次数之和 + 1, 且 len | (MOD - 1)
void convolution(auto &a, auto &b, int len) {
    ntt_dif(a, len);                            // a: 自然序 -> 位逆序点值
    ntt_dif(b, len);                            // b: 自然序 -> 位逆序点值
    for (int i = 0; i < len; i++) {
        a[i] = a[i] * b[i] % MOD;               // 逐点相乘与下标顺序无关
    }
    ntt_idit(a, len);                           // 位逆序 -> 自然序系数
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
//     convolution(fs, gs, len);
//
//     for (int i = 0; i < n + m - 1; i++) cout << fs[i] << ' ';
//     cout << '\n';
//     return 0;
// }
