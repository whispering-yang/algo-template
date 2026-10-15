#include <bits/stdc++.h>
using namespace std;

constexpr long long INF = 0x3f3f3f3f3f3f3f3fLL;
vector<long long> dist;
vector<vector<pair<int, long long>>> adj;

// 队列优化的 Bellman-Ford：仅当点的距离被松弛变小后才重新入队
// 返回 false 表示存在从 s 出发可达的负环（此时 dist 无意义）
bool spfa(int s) {
    int n = (int)dist.size() - 1;
    fill(dist.begin(), dist.end(), INF);
    dist[s] = 0;
    vector<int> cnt(n + 1);  // 当前距离对应的松弛路径边数，不是入队次数
    vector<bool> inq(n + 1);
    queue<int> q;
    q.push(s);
    inq[s] = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        inq[u] = false;
        for (auto [v, w] : adj[u]) {
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                cnt[v] = cnt[u] + 1;
                if (cnt[v] >= n) return false;  // 松弛路径含 n 条边，存在可达负环
                if (!inq[v]) {
                    q.push(v);
                    inq[v] = true;
                }
            }
        }
    }
    return true;
}

// 使用前：读入 n, m, s，将 dist、adj resize 到 n + 1，向 adj 中加入有向边 (u, v, w)；调用 spfa(s)
// 支持负权边；返回 true 时 dist[i] 即 s 到 i 的最短路，不可达为 INF
// 只检测 s 可达的负环；最坏 O(nm)，非负权图优先使用 Dijkstra
// 约定所有中间距离及 dist[u] + w 均在 (-INF, INF) 内
