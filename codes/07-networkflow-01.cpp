// =============================================================================
// File:     07-networkflow-01 — 最大流 (Maximum Flow, Dinic)
// =============================================================================
// 求有向图中源点 s 到汇点 t 的最大流。
// 输入：n m s t（点数、边数、源点、汇点，点编号 1..n）
//       m 行：u v c（有向边 u->v，容量 c >= 0，允许重边与自环）
// 输出：最大流量。
// 约定：所有容量非负，容量总和在 int64 范围内。
// 复杂度：一般图 O(n^2 m)；单位容量图 O(m sqrt n)。

#include <bits/stdc++.h>
using namespace std;

constexpr int64_t INF = 4e18;

struct Dinic {
    struct Edge {
        int to = 0;
        int64_t cap = 0LL;
        int nxt = -1;
    };

    int s, t;
    vector<Edge> edg;
    vector<int> head, cur, dst;

    Dinic(int n) : edg(0), head(n + 1, -1), cur(n + 1, -1), dst(n + 1, -1) {}

    void add_edge(int u, int v, int64_t cap) {
        edg.emplace_back(v, cap, head[u]);
        head[u] = int(edg.size()) - 1;
        edg.emplace_back(u, 0, head[v]);
        head[v] = int(edg.size()) - 1;
    }

    bool bfs() {
        fill(dst.begin(), dst.end(), -1);
        dst[s] = 0;

        queue<int> qu;
        qu.push(s);

        while (!qu.empty()) {
            int u = qu.front();
            qu.pop();

            for (int j = head[u]; j != -1; j = edg[j].nxt) {
                if (edg[j].cap <= 0LL) continue;
                if (dst[edg[j].to] >= 0) continue;

                qu.push(edg[j].to);
                dst[edg[j].to] = dst[u] + 1;
            }
        }

        return dst[t] != -1;
    }

    int64_t dfs(int u, int64_t limit) {
        if (u == t) return limit;

        int64_t pushed = 0LL;
        for (int &j = cur[u]; j != -1; j = edg[j].nxt) {
            int v = edg[j].to;

            if (edg[j].cap <= 0 || dst[v] != dst[u] + 1) continue;

            int64_t canPush = min(limit - pushed, edg[j].cap);
            int64_t sent = dfs(v, canPush);

            edg[j].cap -= sent;
            edg[j ^ 1].cap += sent;
            pushed += sent;

            if (pushed == limit) break;
        }

        if (pushed == 0) dst[u] = -1;
        return pushed;
    }

    int64_t max_flow(int source, int sink) {
        s = source;
        t = sink;

        int64_t flow = 0LL;

        while (bfs()) {
            cur = head;

            int64_t pushed;
            while ((pushed = dfs(s, INF)) > 0LL) {
                flow += pushed;
            }
        }

        return flow;
    }
};

int main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    int n, m, s, t;
    cin >> n >> m >> s >> t;

    Dinic dnc(n);

    for (int _ = 0; _ < m; _++) {
        int u, v;
        int64_t cap;
        cin >> u >> v >> cap;
        dnc.add_edge(u, v, cap);
    }

    cout << dnc.max_flow(s, t) << '\n';

    return 0;
}
