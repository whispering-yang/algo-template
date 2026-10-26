#include <bits/stdc++.h>
using namespace std;

// 顶点编号 1~n；支持非连通图、重边和自环。
int n, m, dfn_cnt = 0;
vector<vector<int>> adj;
vector<int> dfn, low;
vector<bool> is_cut;
vector<pair<int, int>> bridges;

void tarjan(int u, int p) {
    dfn[u] = low[u] = ++dfn_cnt;
    int child_count = 0;
    bool parent_skipped = false;
    for (int v : adj[u]) {
        // 只跳过一条到父节点的边，其余平行边参与 low 更新。
        if (v == p && !parent_skipped) {
            parent_skipped = true;
            continue;
        }
        if (dfn[v] == 0) {
            ++child_count;
            tarjan(v, u);
            low[u] = min(low[u], low[v]);
            if (p != 0 && low[v] >= dfn[u]) is_cut[u] = true;
            if (low[v] > dfn[u]) bridges.emplace_back(u, v);
        } else {
            low[u] = min(low[u], dfn[v]);
        }
    }
    if (p == 0) is_cut[u] = (child_count >= 2);
}

void find_cuts_and_bridges() {
    dfn_cnt = 0;
    dfn.assign(n + 1, 0);
    low.assign(n + 1, 0);
    is_cut.assign(n + 1, false);
    bridges.clear();
    for (int u = 1; u <= n; ++u) {
        if (dfn[u] == 0) tarjan(u, 0);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    adj.resize(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    find_cuts_and_bridges();

    vector<int> cut_vertices;
    for (int u = 1; u <= n; ++u) {
        if (is_cut[u]) cut_vertices.push_back(u);
    }
    // 割点数、割点列表（无割点时输出空行）、桥数、每条桥的端点。
    cout << cut_vertices.size() << '\n';
    for (int u : cut_vertices) cout << u << ' ';
    cout << '\n' << bridges.size() << '\n';
    for (auto [u, v] : bridges) cout << u << ' ' << v << '\n';
    return 0;
}
