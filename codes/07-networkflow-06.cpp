// =============================================================================
// File:     07-networkflow-06 — 有源汇上下界最大流 (Maximum Flow with Lower Bounds)
// =============================================================================
// 每条边有流量下界 lo 与上界 hi（0 <= lo <= hi），求 s 到 t 的最大流。
// 输入：n m s t，随后 m 行：u v lo hi。
// 输出：存在可行流时输出最大流；否则输出 impossible。
// 转化：记 du[v] = v 的「必须流入 - 必须流出」。
//   1) 原边容量改为 hi - lo，另加 t->s 无穷边，把有源汇化为无源汇循环流；
//   2) du[v] > 0 时连 ss->v 容量 du[v]，du[v] < 0 时连 v->tt 容量 -du[v]；
//   3) 跑 ss->tt 最大流，ss 的出边全部满流 <=> 存在可行流；
//   4) 再直接在残量网络上跑 s->t 最大流，其结果即为答案（t->s 边上的旧流量
//      会被反向边自然退还，无需单独统计）。
// 复杂度：两次 Dinic，O(n^2 m)。

#include <bits/stdc++.h>
using namespace std;

struct Dinic {
    struct Edge {
        int to;
        int64_t cap;
    };
    static constexpr int64_t INF = INT64_MAX / 2;

    vector<Edge> e;
    vector<vector<int>> g;
    vector<int> level, cur;

    explicit Dinic(int n) : g(n + 1), level(n + 1), cur(n + 1) {}

    int add(int u, int v, int64_t cap) {
        g[u].push_back(e.size());
        e.push_back({v, cap});
        g[v].push_back(e.size());
        e.push_back({u, 0});
        return e.size() - 2;  // 正边下标
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int id : g[u]) {
                if (e[id].cap > 0 && level[e[id].to] == -1) {
                    level[e[id].to] = level[u] + 1;
                    q.push(e[id].to);
                }
            }
        }
        return level[t] != -1;
    }

    int64_t dfs(int u, int t, int64_t limit) {
        if (u == t) return limit;
        for (int& i = cur[u]; i < (int)g[u].size(); ++i) {
            int id = g[u][i], v = e[id].to;
            if (e[id].cap > 0 && level[v] == level[u] + 1) {
                int64_t d = dfs(v, t, min(limit, e[id].cap));
                if (d > 0) {
                    e[id].cap -= d;
                    e[id ^ 1].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

    int64_t maxflow(int s, int t) {
        int64_t total = 0;
        while (bfs(s, t)) {
            fill(cur.begin(), cur.end(), 0);
            while (int64_t f = dfs(s, t, INF)) total += f;
        }
        return total;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s, t;
    cin >> n >> m >> s >> t;
    Dinic din(n + 2);
    int ss = n + 1, tt = n + 2;

    vector<int64_t> du(n + 1, 0);  // 必须流入 - 必须流出
    for (int i = 0; i < m; ++i) {
        int u, v;
        int64_t lo, hi;
        cin >> u >> v >> lo >> hi;
        din.add(u, v, hi - lo);
        du[u] -= lo;
        du[v] += lo;
    }
    din.add(t, s, Dinic::INF);  // 化为无源汇循环流

    vector<int> ssEdges;  // 超级源的所有出边下标，用于满流判定
    for (int v = 1; v <= n; ++v) {
        if (du[v] > 0) ssEdges.push_back(din.add(ss, v, du[v]));
        else if (du[v] < 0) din.add(v, tt, -du[v]);
    }

    din.maxflow(ss, tt);
    for (int id : ssEdges)
        if (din.e[id].cap > 0) {
            cout << "impossible\n";
            return 0;
        }

    cout << din.maxflow(s, t) << '\n';
    return 0;
}
