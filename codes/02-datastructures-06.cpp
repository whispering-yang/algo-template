// =============================================================================
// File:     02-datastructures-06 — 树链剖分 (Heavy-Light Decomposition)
// =============================================================================

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;

int n, m, root, MOD;
int64_t weight[MAXN];                  // 各点初始权值 (1-indexed)
vector<int> adj[MAXN];                 // 邻接表

// ---------- 第一遍 DFS 求出的树信息 ----------
int parent[MAXN];                      // 父节点
int depth[MAXN];                       // 深度（根为 1）
int subtree_size[MAXN];                // 子树大小
int heavy_child[MAXN];                 // 重儿子（子树最大的儿子，叶子为 0）

// ---------- 第二遍 DFS 求出的剖分信息 ----------
int chain_top[MAXN];                   // 所在重链的链顶（深度最小的节点）
int dfn[MAXN];                         // DFS 序编号（同一条重链上连续）
int dfn_to_node[MAXN];                 // DFS 序 -> 原节点编号 的反映射
int dfn_timer = 0;

void dfs1(int u, int p) {
    parent[u] = p;
    depth[u] = depth[p] + 1;
    subtree_size[u] = 1;
    int max_child_size = 0;
    for (int v : adj[u]) {
        if (v == p) continue;
        dfs1(v, u);
        subtree_size[u] += subtree_size[v];
        if (subtree_size[v] > max_child_size) {
            max_child_size = subtree_size[v];
            heavy_child[u] = v;
        }
    }
}

// top_node 为当前重链的链顶：重儿子沿用链顶，轻儿子自成新链
void dfs2(int u, int top_node) {
    dfn[u] = ++dfn_timer;
    dfn_to_node[dfn_timer] = u;
    chain_top[u] = top_node;
    if (!heavy_child[u]) return;
    dfs2(heavy_child[u], top_node);    // 先走重儿子，保证重链 DFS 序连续
    for (int v : adj[u]) {
        if (v == parent[u] || v == heavy_child[u]) continue;
        dfs2(v, v);                    // 轻儿子作为新链的链顶
    }
}

// ---------- 建在 DFS 序上的线段树（区间加 / 区间求和，带取模） ----------
struct SegTree {
    vector<int64_t> sum, lazy;
    SegTree() {
        sum.assign(MAXN << 2, 0);
        lazy.assign(MAXN << 2, 0);
    }

    void push_up(int node) {
        sum[node] = (sum[node << 1] + sum[node << 1 | 1]) % MOD;
    }

    // 下传懒惰标记：left_size / right_size 为左右儿子管辖的元素个数
    void push_down(int node, int left_size, int right_size) {
        if (lazy[node]) {
            lazy[node << 1] = (lazy[node << 1] + lazy[node]) % MOD;
            lazy[node << 1 | 1] = (lazy[node << 1 | 1] + lazy[node]) % MOD;
            sum[node << 1] = (sum[node << 1] + lazy[node] * left_size) % MOD;
            sum[node << 1 | 1] = (sum[node << 1 | 1] + lazy[node] * right_size) % MOD;
            lazy[node] = 0;
        }
    }

    void build(int node, int lo, int hi) {
        if (lo == hi) {
            sum[node] = weight[dfn_to_node[lo]] % MOD;
            return;
        }
        int mid = (lo + hi) >> 1;
        build(node << 1, lo, mid);
        build(node << 1 | 1, mid + 1, hi);
        push_up(node);
    }

    void range_add(int node, int lo, int hi, int ql, int qr, int64_t k) {
        if (ql <= lo && hi <= qr) {
            sum[node] = (sum[node] + k * (hi - lo + 1)) % MOD;
            lazy[node] = (lazy[node] + k) % MOD;
            return;
        }
        int mid = (lo + hi) >> 1;
        push_down(node, mid - lo + 1, hi - mid);
        if (ql <= mid) range_add(node << 1, lo, mid, ql, qr, k);
        if (qr > mid)  range_add(node << 1 | 1, mid + 1, hi, ql, qr, k);
        push_up(node);
    }

    int64_t range_query(int node, int lo, int hi, int ql, int qr) {
        if (ql <= lo && hi <= qr) return sum[node];
        int mid = (lo + hi) >> 1;
        push_down(node, mid - lo + 1, hi - mid);
        int64_t result = 0;
        if (ql <= mid) result = (result + range_query(node << 1, lo, mid, ql, qr)) % MOD;
        if (qr > mid)  result = (result + range_query(node << 1 | 1, mid + 1, hi, ql, qr)) % MOD;
        return result;
    }
} seg;

// ---------- 路径 / 子树操作接口 ----------
// 路径 u -> v 上所有点权加 k
void path_add(int u, int v, int64_t k) {
    k %= MOD;
    while (chain_top[u] != chain_top[v]) {
        // 每次处理链顶更深的一端：修改 [dfn[链顶], dfn[u]] 后跳到链顶的父节点
        if (depth[chain_top[u]] < depth[chain_top[v]]) swap(u, v);
        seg.range_add(1, 1, n, dfn[chain_top[u]], dfn[u], k);
        u = parent[chain_top[u]];
    }
    if (depth[u] > depth[v]) swap(u, v);   // 已在同一条重链上，处理剩余区间
    seg.range_add(1, 1, n, dfn[u], dfn[v], k);
}

// 路径 u -> v 上所有点权和
int64_t path_sum(int u, int v) {
    int64_t result = 0;
    while (chain_top[u] != chain_top[v]) {
        if (depth[chain_top[u]] < depth[chain_top[v]]) swap(u, v);
        result = (result + seg.range_query(1, 1, n, dfn[chain_top[u]], dfn[u])) % MOD;
        u = parent[chain_top[u]];
    }
    if (depth[u] > depth[v]) swap(u, v);
    result = (result + seg.range_query(1, 1, n, dfn[u], dfn[v])) % MOD;
    return result;
}

// 以 u 为根的子树内所有点权加 k（子树对应连续 DFS 序区间）
void subtree_add(int u, int64_t k) {
    k %= MOD;
    seg.range_add(1, 1, n, dfn[u], dfn[u] + subtree_size[u] - 1, k);
}

// 以 u 为根的子树内所有点权和
int64_t subtree_sum(int u) {
    return seg.range_query(1, 1, n, dfn[u], dfn[u] + subtree_size[u] - 1) % MOD;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> root >> MOD;
    for (int i = 1; i <= n; ++i) cin >> weight[i];
    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs1(root, 0);
    dfs2(root, root);
    seg.build(1, 1, n);

    while (m--) {
        int op, x, y;
        int64_t k;
        cin >> op;
        if (op == 1) {                 // 路径 x -> y 点权加 k
            cin >> x >> y >> k;
            path_add(x, y, k);
        } else if (op == 2) {          // 路径 x -> y 点权和
            cin >> x >> y;
            cout << path_sum(x, y) << '\n';
        } else if (op == 3) {          // 子树 x 点权加 k
            cin >> x >> k;
            subtree_add(x, k);
        } else {                       // 子树 x 点权和
            cin >> x;
            cout << subtree_sum(x) << '\n';
        }
    }

    return 0;
}
