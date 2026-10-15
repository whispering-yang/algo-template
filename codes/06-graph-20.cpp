#include <bits/stdc++.h>
using namespace std;

vector<vector<pair<int, long long>>> adj;

// 检测整张有向图是否存在负环，包含不连通部分；true 表示存在负环
bool has_negative_cycle() {
    int n = (int)adj.size() - 1;
    vector<long long> dist(n + 1, 0);
    vector<int> cnt(n + 1, 0);
    vector<bool> inq(n + 1, false);
    queue<int> q;
    for (int u = 1; u <= n; u++) {
        q.push(u);  // 等价于超级源向每个点连一条权值为 0 的边
        inq[u] = true;
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        inq[u] = false;
        for (auto [v, w] : adj[u]) {
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                cnt[v] = cnt[u] + 1;
                if (cnt[v] >= n) return true;
                if (!inq[v]) {
                    q.push(v);
                    inq[v] = true;
                }
            }
        }
    }
    return false;
}

// 使用前：读入 n, m，将 adj resize 到 n + 1，加入有向边 (u, v, w)
// 无向边需加入两个方向；无向负权边本身就能形成负环
// 最坏 O(nm)；所有中间距离及 dist[u] + w 必须在 long long 范围内
