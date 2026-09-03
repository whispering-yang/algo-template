constexpr int N = 5e5 + 5, LOG = 19;
int n, m, dep[N], pa[LOG][N];
vector<vector<int>> adj;

void dfs(int node, int f) {
    pa[0][node] = f;
    dep[node] = dep[f] + 1;
    for (int i = 1; i < LOG; i++)
        pa[i][node] = pa[i - 1][pa[i - 1][node]];
    for (int nxt : adj[node])
        if (nxt != f) dfs(nxt, node);
}

int lca(int u, int v) {
    if (dep[u] < dep[v]) swap(u, v);
    int diff = dep[u] - dep[v];
    for (int i = 0; i < LOG; i++)
        if (diff >> i & 1) u = pa[i][u];
    if (u == v) return u;
    for (int i = LOG - 1; i >= 0; i--)
        if (pa[i][u] != pa[i][v])
            u = pa[i][u], v = pa[i][v];
    return pa[0][u];
}

// 使用前：dfs(root, 0); dep[root] 会被设为 1
