#include <bits/stdc++.h>
using namespace std;

// 顶点编号 1~n；components[id] 保存一个点双，分量编号从 0 开始。
// 支持非连通图和重边；忽略自环，孤立点单独成分量。
int n, m, dfn_cnt = 0;
vector<vector<int>> adj, components;
vector<int> dfn, low;
stack<int> stk;

void tarjan(int u, int p) {
    dfn[u] = low[u] = ++dfn_cnt;
    stk.push(u);
    int child_count = 0;
    bool parent_skipped = false;
    for (int v : adj[u]) {
        if (v == u) continue;
        // 仅跳过一条父边，其余平行边作为回边更新 low。
        if (v == p && !parent_skipped) {
            parent_skipped = true;
            continue;
        }
        if (dfn[v] == 0) {
            ++child_count;
            tarjan(v, u);
            low[u] = min(low[u], low[v]);
            if (low[v] >= dfn[u]) {
                components.push_back({u});
                // u 仍留在栈中，可以属于其他点双。
                while (true) {
                    int x = stk.top();
                    stk.pop();
                    components.back().push_back(x);
                    if (x == v) break;
                }
            }
        } else {
            low[u] = min(low[u], dfn[v]);
        }
    }
    if (p == 0 && child_count == 0) components.push_back({u});
}

void find_vertex_bcc() {
    dfn_cnt = 0;
    dfn.assign(n + 1, 0);
    low.assign(n + 1, 0);
    components.clear();
    stk = {};
    for (int u = 1; u <= n; ++u) {
        if (dfn[u] != 0) continue;
        tarjan(u, 0);
        stk.pop();  // 此连通块仅剩 DFS 根，清除后再处理下一个连通块。
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
    find_vertex_bcc();

    cout << components.size() << '\n';
    for (const auto &component : components) {
        cout << component.size();
        for (int u : component) cout << ' ' << u;
        cout << '\n';
    }
    return 0;
}
