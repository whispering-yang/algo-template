// =============================================================================
// File:     07-networkflow-02 — 最小割 (Minimum Cut)
// =============================================================================
// 求源点 s 到汇点 t 的最小割容量，并给出一组割边方案。
// 输入：n m s t，随后 m 行：u v c（有向边 u->v，容量 c >= 0）。
// 输出：第一行为最小割容量（等于最大流）；
//       第二行为 k 与一组最小割边的输入序号（1..m，升序）。
// 方案求法：跑完最大流后，在残量网络上从 s 做 BFS 得可达集 S；
//           u ∈ S 且 v ∉ S 的原图边必然满流，全体这样的边构成一组最小割。

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

    vector<array<int, 2>> ed(m);  // 只需端点：容量可由残量推出
    for (int i = 0; i < m; i++) {
        int u, v;
        int64_t cap;
        cin >> u >> v >> cap;
        ed[i] = {u, v};
        dnc.add_edge(u, v, cap);  // 第 i 条输入边的正边下标恰为 2 * i
    }

    int64_t cut = dnc.max_flow(s, t);

    // 残量网络上从 s 可达的点集 S（此时 t 不可达）
    vector<bool> inS(n + 1, false);
    inS[s] = true;

    queue<int> qu;
    qu.push(s);

    while (!qu.empty()) {
        int u = qu.front();
        qu.pop();

        for (int j = dnc.head[u]; j != -1; j = dnc.edg[j].nxt) {
            int v = dnc.edg[j].to;

            if (dnc.edg[j].cap <= 0LL) continue;
            if (inS[v]) continue;

            inS[v] = true;
            qu.push(v);
        }
    }

    cout << cut << '\n';

    vector<int> idx;
    for (int i = 0; i < m; i++) {
        if (inS[ed[i][0]] && !inS[ed[i][1]]) idx.push_back(i + 1);
    }

    cout << idx.size();
    for (int x : idx) cout << ' ' << x;
    cout << '\n';

    return 0;
}
