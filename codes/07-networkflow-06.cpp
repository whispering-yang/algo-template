// =============================================================================
// File:     07-networkflow-06 — 有源汇上下界最大流 (Maximum Flow with Lower Bounds)
// =============================================================================
// 每条边有流量下界 lo 与上界 hi（0 <= lo <= hi），求 s 到 t 的最大流。
// 输入：n m s t，随后 m 行：u v lo hi。
// 输出：存在可行流时输出最大流；否则输出 impossible。
// 转化：记 du[v] = v 的「必须流入 - 必须流出」。
//   1) 原边容量改为 hi - lo，另加 t->s 无穷边，把有源汇化为无源汇循环流；
//   2) du[v] > 0 时连 ss->v 容量 du[v]，du[v] < 0 时连 v->tt 容量 -du[v]；
//   3) 跑 ss->tt 最大流，流量等于正 du 之和（即 ss 的出边全部满流）<=> 存在可行流；
//   4) 再直接在残量网络上跑 s->t 最大流，其结果即为答案（t->s 边上的旧流量
//      会被反向边自然退还，无需单独统计）。
// 复杂度：两次 Dinic，O(n^2 m)。

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

    Dinic dnc(n + 2);
    int ss = n + 1, tt = n + 2;

    vector<int64_t> du(n + 1, 0LL);  // 必须流入 - 必须流出

    for (int _ = 0; _ < m; _++) {
        int u, v;
        int64_t lo, hi;
        cin >> u >> v >> lo >> hi;
        dnc.add_edge(u, v, hi - lo);
        du[u] -= lo;
        du[v] += lo;
    }

    dnc.add_edge(t, s, INF);  // 化为无源汇循环流

    int64_t need = 0LL;  // 超级源出边容量之和，用于满流判定
    for (int v = 1; v <= n; v++) {
        if (du[v] > 0LL) {
            dnc.add_edge(ss, v, du[v]);
            need += du[v];
        } else if (du[v] < 0LL) {
            dnc.add_edge(v, tt, -du[v]);
        }
    }

    if (dnc.max_flow(ss, tt) != need) {
        cout << "impossible\n";
        return 0;
    }

    cout << dnc.max_flow(s, t) << '\n';

    return 0;
}
