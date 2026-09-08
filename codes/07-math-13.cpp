// =============================================================================
// File:     07-math-13  — Polynomial Division / Modulo (多项式除法与取模)
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

// 多项式带余除法: 返回 {q, r}, 满足 a = q * b + r 且 deg(r) < m - 1
// 要求 b 非零且最高次系数 b[m-1] != 0; 当 deg(a) < deg(b) 时返回 {空, a}
pair<vector<int64_t>, vector<int64_t>> poly_divmod(vector<int64_t> a, vector<int64_t> b) {
    int n = (int)a.size(), m = (int)b.size();
    if (n < m) return {{}, a};
    int qn = n - m + 1;                          // 商的项数
    // 系数反转 A^R(x) = x^{n-1} A(1/x):  A^R ≡ Q^R * B^R (mod x^{qn})
    vector<int64_t> ra(a.rbegin(), a.rend()), rb(b.rbegin(), b.rend());
    ra.resize(qn);
    vector<int64_t> rq = poly_mul(ra, poly_inv(rb, qn));   // Q^R = A^R / B^R
    rq.resize(qn);
    vector<int64_t> q(rq.rbegin(), rq.rend());   // 再反转回得到商 Q
    vector<int64_t> qb = poly_mul(q, b);
    vector<int64_t> r(max(m - 1, 0), 0);         // 余式 R = A - Q * B, 次数 < m-1
    for (int i = 0; i < m - 1; i++)
        r[i] = (a[i] - qb[i] + MOD) % MOD;
    return {q, r};
}

// -------- 使用示例 (main) --------
// [Luogu P4512 多项式除法] 输入 A 的 n+1 项与 B 的 m+1 项, 输出商 Q (n-m+1 项)
// 与余式 R (m 项, 不足高位补 0)
// int main() {
//     ios::sync_with_stdio(false);
//     cin.tie(nullptr);
//     int n, m;
//     cin >> n >> m;
//     vector<int64_t> a(n + 1), b(m + 1);
//     for (auto &x : a) cin >> x;
//     for (auto &x : b) cin >> x;
//     auto [q, r] = poly_divmod(a, b);
//     for (int i = 0; i < (int)q.size(); i++) cout << q[i] << " \n"[i + 1 == (int)q.size()];
//     for (int i = 0; i < m; i++) cout << (i < (int)r.size() ? r[i] : 0) << " \n"[i + 1 == m];
//     return 0;
// }
