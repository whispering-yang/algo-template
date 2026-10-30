#include <bits/stdc++.h>
using namespace std;

constexpr long long INF = 0x3f3f3f3f3f3f3f3fLL;
vector<long long> dist;
vector<bool> done;  // done[u]：u 的最短路是否已确定
vector<vector<pair<int, long long>>> adj;

// 每轮线性扫描选出未确定点中 dist 最小者，适合 m 接近 n^2 的稠密图
void dijkstra(int s) {
    int n = (int)dist.size() - 1;
    fill(dist.begin(), dist.end(), INF);
    fill(done.begin(), done.end(), false);
    dist[s] = 0;
    for (int i = 0; i < n; i++) {
        int u = 0;  // dist[0] 恒为 INF，充当「未找到更小点」的哨兵
        for (int v = 1; v <= n; v++)
            if (!done[v] && dist[v] < dist[u]) u = v;
        if (dist[u] == INF) break;  // 剩余未确定点均不可达，提前结束
        done[u] = true;
        for (auto [v, w] : adj[u])
            if (dist[v] > dist[u] + w) dist[v] = dist[u] + w;
    }
}

// 使用前：读入 n, m, s，将 dist、done、adj resize 到 n + 1，向 adj 中加入有向边 (u, v, w)；调用 dijkstra(s)
// 仅适用于边权非负的图；最终 dist[i] 为 s 到 i 的最短路，不可达为 INF
