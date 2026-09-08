// =============================================================================
// File:     06-graph-13 — Kahn 拓扑排序与判环 (Topological Sort / Cycle Detection)
// =============================================================================
// 基于入度的 BFS 拓扑排序：反复取出入度为 0 的点并删除其出边。
// 若最终输出的点数 < n，则剩余点构成环（或位于环下游），即图中有环。
// 时间复杂度 O(n + m)；需要字典序最小的拓扑序时，把队列换成小根堆即可。

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> g(n + 1);
    vector<int> indeg(n + 1, 0);        // indeg[v]：节点 v 的入度
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;                  // 有向边 u -> v
        g[u].push_back(v);
        indeg[v]++;
    }

    // 所有入度为 0 的点可以立即输出，先入队
    queue<int> q;
    for (int i = 1; i <= n; i++)
        if (indeg[i] == 0) q.push(i);

    vector<int> order;                  // order 即拓扑序
    order.reserve(n);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);
        for (int v : g[u])
            if (--indeg[v] == 0)        // 删除出边 u -> v，v 入度减一
                q.push(v);              // 入度归零说明所有前驱已输出
    }

    if ((int)order.size() < n) {
        cout << "cycle\n";              // 有环：环上的点入度永远无法归零
    } else {
        for (int i = 0; i < n; i++)
            cout << order[i] << " \n"[i + 1 == n];
    }
    return 0;
}
