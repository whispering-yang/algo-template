// =============================================================================
// File:     06-math-14  — Sprague-Grundy Function (SG 函数与有向图游戏)
// =============================================================================

#include <bits/stdc++.h>
using namespace std;

// mex: 集合中未出现的最小非负整数 (Minimum EXcluded value)
// 例如 mex{0, 1, 3} = 2,  mex{1, 2} = 0,  mex{} = 0
int mex(vector<int> s) {
    sort(s.begin(), s.end());
    s.erase(unique(s.begin(), s.end()), s.end());
    int m = 0;
    for (int v : s) {
        if (v == m) m++;
        else break;                              // 排序去重后一旦跳号即得出答案
    }
    return m;
}

// 有向图游戏: 局面编号 0..n-1 构成 DAG, g[u] 为局面 u 的所有后继局面
// sg[u] = mex{ sg[v] : v ∈ g[u] }, 无后继的终态 sg = 0
// sg[u] == 0  <=>  局面 u 为必败态 (P 局面), 即轮到行动者必败
int n;
vector<vector<int>> g;
vector<int> sg_val;                              // -1 表示尚未计算

int sg(int u) {
    if (sg_val[u] != -1) return sg_val[u];
    vector<int> nxt;
    nxt.reserve(g[u].size());
    for (int v : g[u]) nxt.push_back(sg(v));
    return sg_val[u] = mex(std::move(nxt));
}

// SG 定理 (Sprague-Grundy Theorem):
//   总游戏由若干【独立】子游戏组成时, 总 SG 值 = 各子游戏 SG 值的异或 (Nim 和);
//   先手必胜 <=> 总 SG 值 != 0。
// Nim 游戏: "一堆 a 颗石子, 每次取任意颗" 的 sg 值恰为 a
//   (后继 SG 集合为 {0, 1, ..., a-1}), 代入 SG 定理即得 Nim 结论。

// -------- 使用示例 (main) --------
// 例 1: [Luogu P2197 nim 游戏] T 组数据, 每组 n 堆石子, 先手必胜输出 Yes
// int main() {
//     ios::sync_with_stdio(false);
//     cin.tie(nullptr);
//     int T;
//     cin >> T;
//     while (T--) {
//         int m, x = 0;
//         cin >> m;
//         for (int i = 0; i < m; i++) {
//             int a;
//             cin >> a;
//             x ^= a;                             // SG 定理: sg(一堆 a 颗) = a
//         }
//         cout << (x != 0 ? "Yes" : "No") << '\n';
//     }
//     return 0;
// }
//
// 例 2: 一般有向图游戏 — 建图后调用
//     n = 状态总数;
//     g.assign(n, {});
//     sg_val.assign(n, -1);
//     // 按具体规则加边: g[u].push_back(v) 表示局面 u 可一步走到 v
//     // 先手从局面 start 出发必胜 <=> sg(start) != 0
