// =============================================================================
// File:     02-datastructures-18 — 长链剖分 (Long-Chain Decomposition)
//           按深度合并树形 DP：统计每个子树各深度的节点数
// =============================================================================

#include <bits/stdc++.h>
using namespace std;

int n;
vector<vector<int>> graph;
vector<int> parent_node, height, long_child;

// ptr[u] 指向 f_u[0]；同一条长链上的相邻节点指针相差 1。
vector<int> ptr, best;
vector<int> best_depth;
vector<int> pool;

void get_long_child(int u, int parent) {
    parent_node[u] = parent;
    height[u] = 1;
    long_child[u] = 0;
    for (int v : graph[u]) {
        if (v == parent) continue;
        get_long_child(v, u);
        if (height[v] + 1 > height[u]) {
            height[u] = height[v] + 1;
            long_child[u] = v;
        }
    }
}

// 每条长链只分配一次空间；所有长链长度之和恰为 n。
void assign_storage(int u, int chain_head) {
    if (u == chain_head) {
        ptr[u] = static_cast<int>(pool.size());
        pool.resize(pool.size() + height[u], 0);
    } else {
        ptr[u] = ptr[parent_node[u]] + 1;
    }
    if (long_child[u]) assign_storage(long_child[u], chain_head);
    for (int v : graph[u]) {
        if (v == parent_node[u] || v == long_child[u]) continue;
        assign_storage(v, v);
    }
}

void merge_depth_dp(int u) {
    for (int v : graph[u]) {
        if (v != parent_node[u]) merge_depth_dp(v);
    }

    int* fu = pool.data() + ptr[u];
    fu[0] = 1;  // 节点 u 自身

    // 长儿子数组已经位于 fu[1], fu[2], ...，无需复制。
    for (int v : graph[u]) {
        if (v == parent_node[u] || v == long_child[u]) continue;
        int* fv = pool.data() + ptr[v];
        for (int d = 1; d <= height[v]; ++d) {
            fu[d] += fv[d - 1];
        }
    }

    best[u] = 0;
    best_depth[u] = 0;
    for (int d = 0; d < height[u]; ++d) {
        if (fu[d] > best[u]) {
            best[u] = fu[d];
            best_depth[u] = d;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    graph.assign(n + 1, {});
    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    parent_node.assign(n + 1, 0);
    height.assign(n + 1, 0);
    long_child.assign(n + 1, 0);
    ptr.assign(n + 1, 0);
    best.assign(n + 1, 0);
    best_depth.assign(n + 1, 0);

    get_long_child(1, 0);
    pool.reserve(n + 1);
    assign_storage(1, 1);
    merge_depth_dp(1);

    // 示例：输出每个节点子树中出现次数最多的深度层（并列取最小深度）。
    for (int u = 1; u <= n; ++u) {
        cout << best_depth[u] << ' ' << best[u] << '\n';
    }
}
