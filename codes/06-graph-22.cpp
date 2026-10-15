#include <bits/stdc++.h>
using namespace std;

constexpr long long INF = 0x3f3f3f3f3f3f3f3fLL;
vector<long long> dist;
vector<vector<pair<int, int>>> adj;

// 仅适用于边权为 0 或 1 的图；不满足条件时应改用其他最短路算法
void bfs01(int s) {
    fill(dist.begin(), dist.end(), INF);
    deque<pair<long long, int>> q;
    dist[s] = 0;
    q.emplace_front(0, s);
    while (!q.empty()) {
        auto [d, u] = q.front();
        q.pop_front();
        if (d != dist[u]) continue;  // 跳过被更短路径替代的旧状态
        for (auto [v, w] : adj[u]) {
            if (dist[v] > d + w) {
                dist[v] = d + w;
                if (w == 0) q.emplace_front(dist[v], v);
                else q.emplace_back(dist[v], v);
            }
        }
    }
}

// 使用前：读入 n, m, s，将 dist、adj resize 到 n + 1，加入有向边
// 调用 bfs01(s) 后 dist[i] 为最短距离，不可达为 INF；时间 O(n+m)
