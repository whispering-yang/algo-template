// =============================================================================
// File:     06-math-05 — Gaussian Elimination
// =============================================================================

#include <bits/stdc++.h>
using namespace std;

// 高斯消元 — 列主元，返回解的情况
//   0 : 唯一解, 解存放在 a 最后一列
//   1 : 无穷多解
//  -1 : 无解
int gauss(vector<vector<double>>& a) {
    const double eps = 1e-9;
    int n = (int)a.size();
    int m = (int)a[0].size() - 1;  // 变量个数
    vector<int> where(m, -1);      // 第 i 列的主元行号

    int row = 0;
    for (int col = 0; col < m && row < n; ++col) {
        // 1. 选取列主元
        int sel = row;
        for (int i = row; i < n; ++i) {
            if (fabs(a[i][col]) > fabs(a[sel][col])) sel = i;
        }
        if (fabs(a[sel][col]) < eps) continue;

        // 2. 交换到当前行
        swap(a[row], a[sel]);
        where[col] = row;

        // 3. 将主元系数归一
        double piv = a[row][col];
        for (int j = col; j <= m; ++j) a[row][j] /= piv;

        // 4. 消去其他行的当前列
        for (int i = 0; i < n; ++i) {
            if (i == row) continue;
            double factor = a[i][col];
            for (int j = col; j <= m; ++j) {
                a[i][j] -= factor * a[row][j];
            }
        }
        ++row;
    }

    // 检查无解
    for (int i = row; i < n; ++i) {
        if (fabs(a[i][m]) > eps) return -1;
    }

    // 检查无穷多解
    for (int i = 0; i < m; ++i) {
        if (where[i] == -1) return 1;
    }
    return 0;
}

// -------- 使用示例 (main) --------
// int main() {
//     int n;
//     cin >> n;
//     vector<vector<double>> a(n, vector<double>(n + 1));
//     for (int i = 0; i < n; ++i) {
//         for (int j = 0; j <= n; ++j) cin >> a[i][j];
//     }
//     int sol = gauss(a);
//     cout << sol << '\n';
//     return 0;
// }
