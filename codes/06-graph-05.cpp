#include <bits/stdc++.h>
using namespace std;

// 顶点编号 1~n；分量编号 0~components.size()-1。
int n, m, dfn_cnt = 0;
vector<vector<int>> adj, components;
vector<int> dfn, low, component_id;
stack<int> stk;

void tarjan(int u, int p) {
    low[u] = dfn[u] = ++dfn_cnt;
    stk.push(u);
    bool parent_skipped = false;
    for (int v : adj[u]) {
        // 只跳过一次父节点方向的边，保留重边作为回边。
        if (v == p && !parent_skipped) {
            parent_skipped = true;
            continue;
        }
        if (dfn[v] == 0) {
            tarjan(v, u);
            low[u] = min(low[u], low[v]);
        } else {
            low[u] = min(low[u], dfn[v]);
        }
    }
    if (dfn[u] == low[u]) {
        int id = (int)components.size();
        components.emplace_back();
        while (true) {
            int v = stk.top();
            stk.pop();
            components.back().push_back(v);
            component_id[v] = id;
            if (v == u) break;
        }
    }
}

void find_edge_bcc() {
    dfn_cnt = 0;
    dfn.assign(n + 1, 0);
    low.assign(n + 1, 0);
    component_id.assign(n + 1, -1);
    components.clear();
    stk = {};
    for (int u = 1; u <= n; ++u) {
        if (dfn[u] == 0) tarjan(u, 0);
    }
}

int main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    cin >> n >> m;
    adj.resize(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    find_edge_bcc();

    cout << components.size() << '\n';
    for (const auto &component : components) {
        cout << component.size();
        for (int u : component) cout << ' ' << u;
        cout << '\n';
    }

    return 0;
}
