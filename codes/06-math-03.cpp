// =============================================================================
// File:     06-math-03 — 矩阵乘法 (Matrix Multiplication)
// =============================================================================

#include <bits/stdc++.h>
using namespace std;

// 计算 result = a × b (mod mod)，其中 a 为 n×k，b 为 k×m，结果为 n×m
// 注意：a[i][p] * b[p][j] 需满足 mod^2 < 2^63（即 mod 大致不超过 3×10^9），
// 更大的模数需改用 __int128 或拆位乘法
vector<vector<int64_t>> mat_mul(const vector<vector<int64_t>>& a,
                                const vector<vector<int64_t>>& b,
                                int64_t mod) {
    int n = (int)a.size();             // a 的行数
    int m = (int)b[0].size();          // b 的列数
    int k = (int)b.size();             // 公共维度：a 的列数 = b 的行数
    vector<vector<int64_t>> result(n, vector<int64_t>(m, 0));
    // 采用 i-p-j 循环顺序：对 a[i][p] 顺序访问，且可跳过零元素，缓存友好
    for (int i = 0; i < n; ++i) {
        for (int p = 0; p < k; ++p) {
            if (a[i][p] == 0) continue;
            for (int j = 0; j < m; ++j) {
                result[i][j] = (result[i][j] + a[i][p] * b[p][j]) % mod;
            }
        }
    }
    return result;
}

// -------- 使用示例 (main) --------
// int main() {
//     int n, m, p;
//     int64_t mod = 1e9 + 7;
//     cin >> n >> m >> p;
//     vector<vector<int64_t>> a(n, vector<int64_t>(m)), b(m, vector<int64_t>(p));
//     for (int i = 0; i < n; ++i) {
//         for (int j = 0; j < m; ++j) cin >> a[i][j];
//     }
//     for (int i = 0; i < m; ++i) {
//         for (int j = 0; j < p; ++j) cin >> b[i][j];
//     }
//     auto c = mat_mul(a, b, mod);
//     for (int i = 0; i < n; ++i) {
//         for (int j = 0; j < p; ++j) cout << c[i][j] << ' ';
//         cout << '\n';
//     }
//     return 0;
// }
