constexpr int N = 5e5 + 5;
int n, m, dfn_cnt, dfn[N], st[19][N];
vector<vector<int>> adj;

// 返回两个节点中 dfn 值较小的那个节点编号
int get(int x, int y) { return dfn[x] < dfn[y] ? x : y; }

void dfs(int node, int f) {
    st[0][dfn[node] = ++dfn_cnt] = f;
    for (int nxt : adj[node]) if (nxt != f) dfs(nxt, node);
}

void build() {
    for (int i = 1; i <= __lg(n); i++)
        for (int j = 1; j + (1 << i) - 1 <= n; j++)
            st[i][j] = get(st[i-1][j], st[i-1][j + (1 << i-1)]);
}

int lca(int u, int v) {
    if (u == v) return u;
    if ((u = dfn[u]) > (v = dfn[v])) swap(u, v);
    int d = __lg(v - u++);
    return get(st[d][u], st[d][v - (1 << d) + 1]);
}

// 使用前：dfs(root, 0); build();
