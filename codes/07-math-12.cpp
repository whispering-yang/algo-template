// =============================================================================
// File:     07-math-12  — Polynomial ln / exp (多项式对数与指数函数)
// Style:    自包含模板: 内嵌 NTT (同 07-math-02) + poly_mul / poly_inv, 模数 998244353
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

// 线性求逆元表: inv[i] = i^{-1} (1 <= i <= n)
vector<int64_t> linear_inv(int n) {
    vector<int64_t> inv(n + 1);
    if (n >= 1) inv[1] = 1;
    for (int i = 2; i <= n; i++)
        inv[i] = (MOD - MOD / i) * inv[MOD % i] % MOD;
    return inv;
}

// 多项式 ln: 返回 b[0..n), 满足 b = ln(a) (mod x^n)
// 要求 a[0] = 1; 原理: ln(a) = ∫ a'(x) / a(x) dx
vector<int64_t> poly_ln(const vector<int64_t> &a, int n) {
    vector<int64_t> d(max(n - 1, 0));            // a 的导数 (积分会丢失常数项)
    for (int i = 1; i < n && i < (int)a.size(); i++)
        d[i - 1] = a[i] * i % MOD;
    vector<int64_t> t = poly_mul(d, poly_inv(a, n));
    t.resize(max(n - 1, 0));
    vector<int64_t> inv = linear_inv(n);
    vector<int64_t> b(n, 0);                     // 积分: b[i] = t[i-1] / i
    for (int i = 0; i < n - 1; i++)
        b[i + 1] = t[i] * inv[i + 1] % MOD;
    return b;
}

// 多项式 exp: 返回 b[0..n), 满足 b = e^a (mod x^n)
// 要求 a[0] = 0; Newton 迭代解 ln(E) = a:  E_{2k} = E_k * (1 + a - ln E_k)
vector<int64_t> poly_exp(const vector<int64_t> &a, int n) {
    vector<int64_t> b{1};                        // 初值 E_1 = e^0 = 1
    int k = 1;
    while (k < n) {
        k <<= 1;
        vector<int64_t> t = poly_ln(b, k);       // ln(E_k), b[0] = 1 故合法
        for (int i = 0; i < k; i++) {
            int64_t ai = (i < (int)a.size() ? a[i] : 0);
            t[i] = (ai - t[i] + MOD) % MOD;      // a - ln(E_k)
        }
        t[0] = (t[0] + 1) % MOD;                 // 1 + a - ln(E_k)
        b = poly_mul(b, t);
        b.resize(k);
    }
    b.resize(n);
    return b;
}

// -------- 使用示例 (main) --------
// [Luogu P4726 多项式指数函数] 保证 a[0] = 0, 输出 e^A 的前 n 项
// [Luogu P4725 多项式对数函数] 保证 a[0] = 1, 把 poly_exp 换成 poly_ln 即可
// int main() {
//     ios::sync_with_stdio(false);
//     cin.tie(nullptr);
//     int n;
//     cin >> n;
//     vector<int64_t> a(n);
//     for (auto &x : a) cin >> x;
//     auto b = poly_exp(a, n);
//     for (int i = 0; i < n; i++) cout << b[i] << " \n"[i + 1 == n];
//     return 0;
// }
