// 最小费用最大流：势能 + Dijkstra（SSP）
// 输入：n m s t，随后 m 行 u v cap cost；点编号 1..n，s != t。
// 容量为非负整数，允许负费用，但初始正容量网络不能有负环。
// 输出：最大流、最小费用；mcmf(s, t, limit) 可限制本次新增流量。
// 复杂度：O(nm + k(n + m)log(n + m))，k 为增广次数。
// 非负费用时省略 O(nm) 初始化；所有距离/势能/重标权绝对值须小于 INF，
// 总流量须小于 INF，费用及其中间运算须能用 int64_t 表示。

#include <bits/stdc++.h>
using namespace std;

struct MCMFDijkstra {
    static constexpr int64_t INF = 4'000'000'000'000'000'000LL;

    struct Edge {
        int to;
        int64_t cap, cost;
        int nxt;
    };

    int n;
    vector<Edge> edg;
    vector<int> head, pre;
    vector<int64_t> h, dst;

    explicit MCMFDijkstra(int n)
        : n(n), head(n + 1, -1), pre(n + 1), h(n + 1), dst(n + 1) {}

    void add_edge(int u, int v, int64_t cap, int64_t cost) {
        edg.push_back({v, cap, cost, head[u]});
        head[u] = int(edg.size()) - 1;
        edg.push_back({u, 0, -cost, head[v]});
        head[v] = int(edg.size()) - 1;
    }

    void init_potential(int s) {
        fill(h.begin(), h.end(), 0);
        bool negative = false;
        for (const auto &e : edg)
            if (e.cap > 0 && e.cost < 0) negative = true;
        if (!negative) return;

        // 仅初始化一次 Bellman-Ford；无负环是调用前提。
        fill(h.begin(), h.end(), INF);
        h[s] = 0;
        for (int round = 1; round < n; round++) {
            bool changed = false;
            for (int u = 1; u <= n; u++) {
                if (h[u] == INF) continue;
                for (int j = head[u]; j != -1; j = edg[j].nxt) {
                    const auto &e = edg[j];
                    if (e.cap <= 0 || h[e.to] <= h[u] + e.cost) continue;
                    h[e.to] = h[u] + e.cost;
                    changed = true;
                }
            }
            if (!changed) break;
        }
        for (int v = 1; v <= n; v++)
            if (h[v] == INF) h[v] = 0;  // 不可达点不参与最短路
    }

    bool dijkstra(int s, int t) {
        fill(dst.begin(), dst.end(), INF);
        fill(pre.begin(), pre.end(), -1);
        priority_queue<pair<int64_t, int>, vector<pair<int64_t, int>>,
                       greater<pair<int64_t, int>>> qu;
        dst[s] = 0;
        qu.push({0, s});
        while (!qu.empty()) {
            auto [d, u] = qu.top();
            qu.pop();
            if (d != dst[u]) continue;
            for (int j = head[u]; j != -1; j = edg[j].nxt) {
                const auto &e = edg[j];
                if (e.cap <= 0) continue;
                int64_t reduced = e.cost + h[u] - h[e.to];
                if (dst[e.to] <= d + reduced) continue;
                dst[e.to] = d + reduced;
                pre[e.to] = j;
                qu.push({dst[e.to], e.to});
            }
        }
        // 不在弹出 t 时提前退出：更新势能需要所有可达点的最终距离。
        if (dst[t] == INF) return false;
        for (int v = 1; v <= n; v++)
            if (dst[v] != INF) h[v] += dst[v];
        return true;
    }

    pair<int64_t, int64_t> mcmf(int s, int t, int64_t limit = INF) {
        assert(s != t && limit >= 0);
        if (limit == 0) return {0, 0};
        init_potential(s);
        int64_t flow = 0, cost = 0;
        while (flow < limit && dijkstra(s, t)) {
            int64_t aug = limit - flow, path_cost = 0;
            for (int v = t; v != s; v = edg[pre[v] ^ 1].to) {
                aug = min(aug, edg[pre[v]].cap);
                path_cost += edg[pre[v]].cost;
            }
            for (int v = t; v != s; v = edg[pre[v] ^ 1].to) {
                edg[pre[v]].cap -= aug;
                edg[pre[v] ^ 1].cap += aug;
            }
            flow += aug;
            cost += aug * path_cost;  // 使用原费用，而非重标权距离
        }
        return {flow, cost};
    }
};

int main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    MCMFDijkstra mc(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        int64_t cap, cost;
        cin >> u >> v >> cap >> cost;
        mc.add_edge(u, v, cap, cost);
    }
    auto [flow, cost] = mc.mcmf(s, t);
    cout << flow << ' ' << cost << '\n';
    return 0;
}
