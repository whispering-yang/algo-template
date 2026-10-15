#include <bits/stdc++.h>
using namespace std;

constexpr long long INF = 0x3f3f3f3f3f3f3f3fLL;
struct Edge {
    int u, v;
    long long w;
};
vector<Edge> edges;
vector<long long> dist;

// 第 k 轮后，所有至多经过 k 条边的最短路径都已被考虑（也可能传播得更远）
// 返回 false 表示存在从 s 可达的负环，此时 dist 不是最终答案
bool bellman_ford(int s) {
    int n = (int)dist.size() - 1;
    fill(dist.begin(), dist.end(), INF);
    dist[s] = 0;
    for (int k = 1; k <= n; k++) {
        bool changed = false;
        for (auto [u, v, w] : edges) {
            if (dist[u] == INF) continue;  // 不从不可达点转移
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                changed = true;
            }
        }
        if (!changed) return true;
        if (k == n) return false;  // 第 n 轮仍能松弛，存在可达负环
    }
    return true;
}

// 使用前：读入 n, m, s，将 dist resize 到 n + 1，向 edges 加入有向边
// 返回 true 时 dist[i] 为最短距离，不可达为 INF；时间 O(nm+n)
// 若只求至多 K 条边的最短路：每轮需先 auto prev = dist，改用 prev[u] 转移
// 此时只执行 K 轮，不使用第 n 轮判负环；备份版时间 O(K(n+m))
// 约定所有中间距离及 dist[u] + w 均在 (-INF, INF) 内
