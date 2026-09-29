// =============================================================================
// File:     07-networkflow-04 — 二分图最大匹配 (Bipartite Matching, Hungarian)
// =============================================================================
// 左部 n 个点、右部 m 个点、k 条边，求两两不共端点的最大边集。
// 输入：n m k，随后 k 行：u v（左部 u 与右部 v 有边，1 <= u <= n, 1 <= v <= m）。
// 输出：最大匹配数。
// 算法：匈牙利算法——对每个左部点 DFS 找增广路（非匹配边/匹配边交替，
//       以非匹配边结尾），找到后整条取反，匹配数加一。
// 复杂度：O(n * k)。规模大时改用 Dinic 建模（s->左 1，左->右 1，右->t 1），
//         单位容量下为 O(k sqrt n)。

#include <bits/stdc++.h>
using namespace std;

struct Hungary {
    int n, m;
    vector<vector<int>> g;  // 左点的邻接表（存右点编号）
    vector<int> matchy;     // 右点匹配到的左点，0 表示未匹配
    vector<bool> vis;       // 本轮 DFS 已访问的右点

    Hungary(int n, int m) : n(n), m(m), g(n + 1), matchy(m + 1, 0), vis(m + 1, false) {}

    bool dfs(int u) {
        for (int v : g[u]) {
            if (vis[v]) continue;
            vis[v] = true;
            if (!matchy[v] || dfs(matchy[v])) {  // v 空闲，或其匹配者能改配其他右点
                matchy[v] = u;
                return true;
            }
        }
        return false;
    }

    int matching() {
        int ans = 0;
        for (int u = 1; u <= n; ++u) {
            fill(vis.begin(), vis.end(), false);
            if (dfs(u)) ++ans;
        }
        return ans;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;
    Hungary hg(n, m);
    for (int i = 0; i < k; ++i) {
        int u, v;
        cin >> u >> v;
        hg.g[u].push_back(v);
    }
    cout << hg.matching() << '\n';
    return 0;
}
