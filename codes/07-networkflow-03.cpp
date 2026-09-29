// =============================================================================
// File:     07-networkflow-03 — 最小费用最大流 (Minimum Cost Maximum Flow)
// =============================================================================
// 每条边另有单位费用 cost，在流量最大的前提下使总费用最小。
// 输入：n m s t，随后 m 行：u v cap cost（有向边，cap >= 0；费用可负，但初始网络无负环）。
// 输出：一行两个数：最大流、最小费用。
// 算法：连续最短增广路（SSP）+ SPFA；反向边费用取负，增广经过反边即退流退费。
// 复杂度：O(f * n * m)，f 为最大流量（伪多项式）。

#include <bits/stdc++.h>
using namespace std;

struct MCMF {
    struct Edge {
        int to;
        int64_t cap, cost;
    };
    static constexpr int64_t INF = INT64_MAX / 2;

    vector<Edge> e;
    vector<vector<int>> g;
    vector<int64_t> dist;   // SPFA 最短路
    vector<int> pre;        // pre[v]：当前最短路树上到达 v 的边下标
    vector<bool> inq;

    explicit MCMF(int n) : g(n + 1), dist(n + 1), pre(n + 1), inq(n + 1) {}

    void add(int u, int v, int64_t cap, int64_t cost) {
        g[u].push_back(e.size());
        e.push_back({v, cap, cost});
        g[v].push_back(e.size());
        e.push_back({u, 0, -cost});  // 反向边：撤销单位流量，费用同时退还
    }

    bool spfa(int s, int t) {
        fill(dist.begin(), dist.end(), INF);
        fill(inq.begin(), inq.end(), false);
        queue<int> q;
        dist[s] = 0;
        inq[s] = true;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            inq[u] = false;
            for (int id : g[u]) {
                int v = e[id].to;
                if (e[id].cap > 0 && dist[u] + e[id].cost < dist[v]) {
                    dist[v] = dist[u] + e[id].cost;
                    pre[v] = id;
                    if (!inq[v]) {
                        inq[v] = true;
                        q.push(v);
                    }
                }
            }
        }
        return dist[t] != INF;
    }

    pair<int64_t, int64_t> mcmf(int s, int t) {
        int64_t flow = 0, cost = 0;
        while (spfa(s, t)) {
            int64_t aug = INF;
            for (int v = t; v != s; v = e[pre[v] ^ 1].to)  // e[pre[v]^1].to 即路径上的前驱点
                aug = min(aug, e[pre[v]].cap);
            for (int v = t; v != s; v = e[pre[v] ^ 1].to) {
                e[pre[v]].cap -= aug;
                e[pre[v] ^ 1].cap += aug;
            }
            flow += aug;
            cost += aug * dist[t];
        }
        return {flow, cost};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s, t;
    cin >> n >> m >> s >> t;
    MCMF mc(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        int64_t cap, cost;
        cin >> u >> v >> cap >> cost;
        mc.add(u, v, cap, cost);
    }

    auto [flow, cost] = mc.mcmf(s, t);
    cout << flow << ' ' << cost << '\n';
    return 0;
}
