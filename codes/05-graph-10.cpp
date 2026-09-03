#include <bits/stdc++.h>
using namespace std;

constexpr long long INF = 0x3f3f3f3f3f3f3f3fLL;
constexpr int N = 1e5 + 5;
int n, m, s;
long long dist[N];
vector<pair<int, long long>> adj[N];
// 小根堆：(当前距离, 节点编号)
priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;

void dijkstra(int s) {
    memset(dist, 0x3f, sizeof(dist));
    dist[s] = 0;
    pq.emplace(0, s);
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue;  // 惰性删除：跳过过期状态
        for (auto [v, w] : adj[u])
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pq.emplace(dist[v], v);
            }
    }
}

// 使用前：读入 n, m, s，向 adj 中加入有向边 (u, v, w)；调用 dijkstra(s)
// 仅适用于边权非负的图；最终 dist[i] 为 s 到 i 的最短路，不可达为 INF
