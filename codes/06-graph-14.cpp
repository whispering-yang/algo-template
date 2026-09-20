// =============================================================================
// File:     06-graph-14 — Tarjan 离线求 LCA (Tarjan's Offline LCA)
// =============================================================================
// 离线回答全部 LCA 询问：DFS 回溯时用并查集把已回溯完的子树并到当前节点，
// 集合的根即集合内节点的最深「未回溯」祖先。节点 u 回溯完毕时检查挂在 u 上
// 的询问 (u, v)，若对端 v 也已回溯，则 find(v) 就是 LCA(u, v)。每个询问由
// 后回溯的一端回答，恰好回答一次。总复杂度 O((n + q) * alpha(n))，其中
// alpha 为反阿克曼函数，可视为常数；询问必须全部事先给出，不支持在线。
// 约定：树以 1 为根，1 <= u, v <= n。

#include <bits/stdc++.h>
using namespace std;

int n, q;
vector<vector<int>> adj;              // 树的邻接表
vector<vector<pair<int, int>>> qry;   // qry[u]：挂在 u 上的询问（对端点，询问编号）
vector<int> fa;                       // 并查集：回溯时子树集合并入父节点
vector<bool> vis;                     // vis[u]：u 是否已回溯（访问完毕）
vector<int> ans;

int find(int u) { return fa[u] == u ? u : fa[u] = find(fa[u]); }

void tarjan(int u, int f) {
    for (int v : adj[u]) {
        if (v == f) continue;
        tarjan(v, u);
        fa[find(v)] = u;              // v 及其子树全部回溯完，并入 u
    }
    vis[u] = true;
    for (auto [w, id] : qry[u])       // 对端先回溯的询问现在可以回答
        if (vis[w]) ans[id] = find(w);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> q;
    adj.assign(n + 1, {});
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;                // 无向树边
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    qry.assign(n + 1, {});
    for (int i = 0; i < q; i++) {
        int u, v;
        cin >> u >> v;
        qry[u].push_back({v, i});     // 询问挂在两个端点上
        qry[v].push_back({u, i});
    }

    fa.resize(n + 1);
    iota(fa.begin(), fa.end(), 0);
    vis.assign(n + 1, false);
    ans.assign(q, 0);

    tarjan(1, 0);

    for (int i = 0; i < q; i++) cout << ans[i] << '\n';
    return 0;
}
