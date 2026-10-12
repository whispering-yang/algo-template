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

constexpr int64_t INF = 4e18;

struct MCMF {
    struct Edge {
        int to = 0;
        int64_t cap = 0LL;
        int64_t cost = 0LL;
        int nxt = -1;
    };

    int s, t;
    vector<Edge> edg;
    vector<int> head, pre;      // pre[v]：当前最短路树上到达 v 的边下标
    vector<int64_t> dst;        // SPFA 最短路
    vector<bool> inq;

    MCMF(int n) : edg(0), head(n + 1, -1), pre(n + 1, -1), dst(n + 1, INF), inq(n + 1, false) {}

    void add_edge(int u, int v, int64_t cap, int64_t cost) {
        edg.emplace_back(v, cap, cost, head[u]);
        head[u] = int(edg.size()) - 1;
        edg.emplace_back(u, 0LL, -cost, head[v]);  // 反向边：撤销单位流量，费用同时退还
        head[v] = int(edg.size()) - 1;
    }

    bool spfa() {
        fill(dst.begin(), dst.end(), INF);
        fill(inq.begin(), inq.end(), false);

        dst[s] = 0LL;
        inq[s] = true;

        queue<int> qu;
        qu.push(s);

        while (!qu.empty()) {
            int u = qu.front();
            qu.pop();
            inq[u] = false;

            for (int j = head[u]; j != -1; j = edg[j].nxt) {
                int v = edg[j].to;

                if (edg[j].cap <= 0LL) continue;
                if (dst[u] + edg[j].cost >= dst[v]) continue;

                dst[v] = dst[u] + edg[j].cost;
                pre[v] = j;

                if (inq[v]) continue;

                inq[v] = true;
                qu.push(v);
            }
        }

        return dst[t] != INF;
    }

    pair<int64_t, int64_t> mcmf(int source, int sink) {
        s = source;
        t = sink;

        int64_t flow = 0LL, cost = 0LL;

        while (spfa()) {
            int64_t aug = INF;
            for (int v = t; v != s; v = edg[pre[v] ^ 1].to) {  // edg[pre[v]^1].to 即路径上的前驱点
                aug = min(aug, edg[pre[v]].cap);
            }

            for (int v = t; v != s; v = edg[pre[v] ^ 1].to) {
                edg[pre[v]].cap -= aug;
                edg[pre[v] ^ 1].cap += aug;
            }

            flow += aug;
            cost += aug * dst[t];
        }

        return {flow, cost};
    }
};

int main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    int n, m, s, t;
    cin >> n >> m >> s >> t;

    MCMF mc(n);

    for (int _ = 0; _ < m; _++) {
        int u, v;
        int64_t cap, cost;
        cin >> u >> v >> cap >> cost;
        mc.add_edge(u, v, cap, cost);
    }

    auto [flow, cost] = mc.mcmf(s, t);
    cout << flow << ' ' << cost << '\n';

    return 0;
}
