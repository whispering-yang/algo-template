int n, m;
vector<vector<int>> adj;

int dfn_cnt;
array<int, MAXN> dfn, low, parent;
// parent[i] 父节点编号，根节点 parent 设为 -1

void tarjan(int u, int p) {
    parent[u] = p;
    dfn[u] = low[u] = ++dfn_cnt;
    for (int v : adj[u]) {
        if (v == p) continue;
        if (dfn[v] == 0) {
            tarjan(v, u);
            low[u] = min(low[u], low[v]);
        } else {
            low[u] = min(low[u], dfn[v]);
        }
    }
}

// 在 DFS 之后，利用以下 if 语句判定割点和桥：

// 1. 割点（非根节点）：存在子节点 v 满足 low[v] >= dfn[u]
//    if (low[v] >= dfn[u]) 则 u 是割点

// 2. 割点（根节点）：根节点在 DFS 树中有 >= 2 个子树
//    if (child_cnt[root] >= 2) 则 root 是割点
//    其中 child_cnt 可以在 tarjan 中统计：if (dfn[v] == 0) child_cnt[u]++;

// 3. 桥：树边 (u, v) 且 parent[v] == u，满足 low[v] > dfn[u]
//    if (low[v] > dfn[u]) 则边 (u, v) 是桥
