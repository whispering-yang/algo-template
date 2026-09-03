// =============================================================================
// File:     06-math-10  — Polynomial Inverse (多项式求逆, Newton 迭代)
// Style:    自包含模板: 内嵌 NTT (同 06-math-02), 模数 998244353
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

// NTT 迭代实现:  d = 1 正变换 (系数 -> 点值),  d = -1 逆变换 (点值 -> 系数)
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

// 多项式乘法 (卷积): 返回完整乘积的系数
vector<int64_t> poly_mul(vector<int64_t> a, vector<int64_t> b) {
    if (a.empty() || b.empty()) return {};
    int n = (int)a.size(), m = (int)b.size();
    int len = 1;
    while (len < n + m - 1) len <<= 1;
    a.resize(len, 0);
    b.resize(len, 0);
    ntt(a, len, 1);
    ntt(b, len, 1);
    for (int i = 0; i < len; i++) a[i] = a[i] * b[i] % MOD;
    ntt(a, len, -1);
    a.resize(n + m - 1);
    return a;
}

// 多项式求逆: 返回 b[0..n), 满足 a(x) * b(x) ≡ 1 (mod x^n)
// 要求 a[0] != 0 (模意义下可逆)
vector<int64_t> poly_inv(const vector<int64_t> &a, int n) {
    vector<int64_t> b{fpow(a[0], MOD - 2)};      // 初值 B_1 = a[0]^{-1}
    int k = 1;                                   // 当前精度: B 已对 mod x^k 正确
    while (k < n) {
        k <<= 1;
        int len = k << 1;                        // 变换长度取 2k, 保证 A*B^2 (次数 < 2k) 不回绕
        vector<int64_t> ta(len, 0), tb(len, 0);
        for (int i = 0; i < k && i < (int)a.size(); i++) ta[i] = a[i];
        for (int i = 0; i < k / 2; i++) tb[i] = b[i];
        ntt(ta, len, 1);
        ntt(tb, len, 1);
        // Newton 迭代: B_{2k} = B_k * (2 - A * B_k)  (mod x^k)
        for (int i = 0; i < len; i++)
            tb[i] = tb[i] * ((2 - ta[i] * tb[i] % MOD + MOD) % MOD) % MOD;
        ntt(tb, len, -1);
        b.assign(tb.begin(), tb.begin() + k);
    }
    b.resize(n);
    return b;
}

// -------- 使用示例 (main) --------
// [Luogu P4238 多项式乘法逆] 输入 n 与 A 的 n 项系数, 输出 A 的逆 (mod 998244353)
// int main() {
//     ios::sync_with_stdio(false);
//     cin.tie(nullptr);
//     int n;
//     cin >> n;
//     vector<int64_t> a(n);
//     for (auto &x : a) cin >> x;
//     auto b = poly_inv(a, n);
//     for (int i = 0; i < n; i++) cout << b[i] << " \n"[i + 1 == n];
//     return 0;
// }
