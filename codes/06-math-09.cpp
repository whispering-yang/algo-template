// =============================================================================
// File:     06-math-09 — Convolution & Correlation (卷积与相关)
// Style:    精简版 — 假设 fpow / bit_reversal / ntt 已实现 (见 codes/06-math-02.cpp),
//           此处仅保留卷积与相关的核心封装; 需要 C++20 (std::bit_ceil)
// =============================================================================

#include <bits/stdc++.h>
using namespace std;

// -------- 依赖外部 NTT 模板 (见 codes/06-math-02.cpp) --------
constexpr int64_t MOD = 998244353;   // 质数模数, 原根为 3
constexpr int64_t G = 3;             // 原根
int64_t fpow(int64_t a, int64_t b);
void bit_reversal(auto &f, int len);
void ntt(auto &f, int len, int d);   // d=1 正变换, d=-1 逆变换 (内部已乘 len^{-1})

// -------- 普通卷积 (Linear Convolution) --------
// c[k] = sum_{i+j=k} a[i]*b[j], k in [0, n+m-2]
// 补零至 N >= n+m-1 后 NTT, 避免循环卷积的"回绕"混叠
template <typename T>
vector<int64_t> convolution(const vector<T>& a, const vector<T>& b) {
    int n = (int)a.size(), m = (int)b.size();
    if (n == 0 || m == 0) return {};
    int len = (int)bit_ceil((unsigned)(n + m - 1));   // 最小 2 的幂 (C++20)
    vector<int64_t> fa(len, 0), fb(len, 0);
    for (int i = 0; i < n; i++) fa[i] = a[i] % MOD;
    for (int i = 0; i < m; i++) fb[i] = b[i] % MOD;
    ntt(fa, len, 1);
    ntt(fb, len, 1);
    for (int i = 0; i < len; i++) fa[i] = fa[i] * fb[i] % MOD;
    ntt(fa, len, -1);
    fa.resize(n + m - 1);
    return fa;
}

// -------- 循环卷积 (Circular Convolution) --------
// c[k] = sum_{i+j=k (mod N)} a[i]*b[j], k in [0, N-1]
// 通用做法: 先算普通卷积, 再按长度 N 折叠回绕相加 (对任意 N 有效)
// 若 N 为 2 的幂且 N | (MOD-1), 可直接做长度 N 的 NTT 点值相乘 (省一次变换)
template <typename T>
vector<int64_t> circular_convolution(const vector<T>& a, const vector<T>& b, int N) {
    vector<int64_t> lin = convolution(a, b);   // 线性卷积
    vector<int64_t> res(N, 0);
    for (int i = 0; i < (int)lin.size(); i++) {
        res[i % N] = (res[i % N] + lin[i]) % MOD;  // 回绕 (wrap-around)
    }
    return res;
}

// -------- 普通互相关 (Linear Cross-Correlation) --------
// r[k] = sum_i a[i]*b[i+k], k in [-(m-1), n-1]
// 返回 res 满足 res[k + (m-1)] = r[k], 即 res[m-1] 对应零移位 (k=0)
// 实现: 翻转 b 后做普通卷积, 再镜像回原顺序 (NTT 无共轭, 用翻转代替)
template <typename T>
vector<int64_t> cross_correlation(const vector<T>& a, const vector<T>& b) {
    int n = (int)a.size(), m = (int)b.size();
    vector<T> rb(b.rbegin(), b.rend());        // 翻转 b
    vector<int64_t> c = convolution(a, rb);    // c[j] = r[m-1-j] (镜像)
    vector<int64_t> res(n + m - 1, 0);
    for (int k = -(m - 1); k <= n - 1; k++) {
        int j = m - 1 - k;                     // 在 c 中的下标
        if (j >= 0 && j < (int)c.size()) res[k + (m - 1)] = c[j];
    }
    return res;
}

// -------- 自相关 (Auto-Correlation) --------
// res[k + (n-1)] = r[k] = sum_i a[i]*a[i+k]
// 性质: r[-k] = r[k] (偶对称), r[0] = sum a[i]^2 为序列能量
template <typename T>
vector<int64_t> auto_correlation(const vector<T>& a) {
    return cross_correlation(a, a);
}

// -------- 循环互相关 (Circular Cross-Correlation) --------
// r[k] = sum_i a[i]*b[(i+k) mod N], k in [0, N-1]
// 实现: 翻转 b 后做循环卷积得 c, 再由 r[k] = c[(m-1-k) mod N] 镜像
template <typename T>
vector<int64_t> circular_cross_correlation(const vector<T>& a, const vector<T>& b, int N) {
    int m = (int)b.size();
    vector<T> rb(b.rbegin(), b.rend());
    vector<int64_t> c = circular_convolution(a, rb, N);
    vector<int64_t> res(N);
    for (int k = 0; k < N; k++) {
        res[k] = c[((m - 1 - k) % N + N) % N];
    }
    return res;
}

// -------- 循环自相关 (Circular Auto-Correlation) --------
// r[k] = sum_i a[i]*a[(i+k) mod N], k in [0, N-1]
// 性质: r[k] = r[(N-k) mod N]
template <typename T>
vector<int64_t> circular_auto_correlation(const vector<T>& a, int N) {
    return circular_cross_correlation(a, a, N);
}

// -------- 使用示例 (main) --------
// int main() {
//     vector<int64_t> a = {1, 2, 3}, b = {4, 5};
//     auto c = convolution(a, b);            // 普通卷积:  [4, 13, 22, 15]
//     auto cc = circular_convolution(a, b, 4);   // 循环卷积
//     auto cr = cross_correlation(a, b);     // res[k+1] = r[k], res[1] 为零移位
//     auto ar = auto_correlation(a);         // 自相关, 偶对称
//     auto crc = circular_cross_correlation(a, b, 4);
//     auto arc = circular_auto_correlation(a, 4);
//     return 0;
// }
