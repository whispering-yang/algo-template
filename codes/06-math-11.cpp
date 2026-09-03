// =============================================================================
// File:     06-math-11  — Polynomial Square Root (多项式开方, Newton 迭代)
// Style:    自包含模板: 内嵌 NTT (同 06-math-02) + poly_mul / poly_inv, 模数 998244353
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

// 多项式求逆 (Newton 迭代, 详见 06-math-10)
vector<int64_t> poly_inv(const vector<int64_t> &a, int n) {
    vector<int64_t> b{fpow(a[0], MOD - 2)};
    int k = 1;
    while (k < n) {
        k <<= 1;
        int len = k << 1;
        vector<int64_t> ta(len, 0), tb(len, 0);
        for (int i = 0; i < k && i < (int)a.size(); i++) ta[i] = a[i];
        for (int i = 0; i < k / 2; i++) tb[i] = b[i];
        ntt(ta, len, 1);
        ntt(tb, len, 1);
        for (int i = 0; i < len; i++)
            tb[i] = tb[i] * ((2 - ta[i] * tb[i] % MOD + MOD) % MOD) % MOD;
        ntt(tb, len, -1);
        b.assign(tb.begin(), tb.begin() + k);
    }
    b.resize(n);
    return b;
}

// 多项式开方: 返回 b[0..n), 满足 b(x)^2 ≡ a(x) (mod x^n)
// 常数项需在模意义下可开方; 本模板默认 a[0] = 1 (竞赛题最常见),
// 一般情形需先用 Cipolla (二次剩余) 求出 sqrt(a[0]) 作为初值
vector<int64_t> poly_sqrt(const vector<int64_t> &a, int n) {
    constexpr int64_t INV2 = (MOD + 1) / 2;      // 1/2 (MOD 为奇数)
    vector<int64_t> b{1};                        // 初值 B_1 = sqrt(a[0]) = 1
    int k = 1;
    while (k < n) {
        k <<= 1;
        vector<int64_t> at(min<size_t>(a.size(), k));
        for (int i = 0; i < (int)at.size(); i++) at[i] = a[i];
        vector<int64_t> t = poly_mul(at, poly_inv(b, k));   // A * B^{-1}
        t.resize(k);
        b.resize(k, 0);
        for (int i = 0; i < k; i++)              // B_{2k} = (B_k + A / B_k) / 2
            b[i] = (b[i] + t[i]) % MOD * INV2 % MOD;
    }
    b.resize(n);
    return b;
}

// -------- 使用示例 (main) --------
// [Luogu P5277 多项式乘法开方] 保证 a[0] = 1, 输出 sqrt(A) 的前 n 项
// int main() {
//     ios::sync_with_stdio(false);
//     cin.tie(nullptr);
//     int n;
//     cin >> n;
//     vector<int64_t> a(n);
//     for (auto &x : a) cin >> x;
//     auto b = poly_sqrt(a, n);
//     for (int i = 0; i < n; i++) cout << b[i] << " \n"[i + 1 == n];
//     return 0;
// }
