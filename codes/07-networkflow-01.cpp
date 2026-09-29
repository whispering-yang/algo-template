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

struct Dinic {
    struct Edge {
        int to;
        int64_t cap;
    };
    static constexpr int64_t INF = INT64_MAX / 2;

    vector<Edge> e;          // e[i] 与 e[i ^ 1] 互为反向边
    vector<vector<int>> g;   // 邻接表：存边在 e 中的下标
    vector<int> level, cur;  // BFS 层次；当前弧（每个点已扫描到的出边下标）

    explicit Dinic(int n) : g(n + 1), level(n + 1), cur(n + 1) {}

    void add(int u, int v, int64_t cap) {
        g[u].push_back(e.size());
        e.push_back({v, cap});
        g[v].push_back(e.size());
        e.push_back({u, 0});  // 反向边初始容量 0，伴随正边流量增长
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
        for (int& i = cur[u]; i < (int)g[u].size(); ++i) {  // 当前弧：不再回头扫已失效的出边
            int id = g[u][i], v = e[id].to;
            if (e[id].cap > 0 && level[v] == level[u] + 1) {
                int64_t d = dfs(v, t, min(limit, e[id].cap));
                if (d > 0) {
                    e[id].cap -= d;
                    e[id ^ 1].cap += d;  // 反向边容量增加，代表可撤销这部分流量
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
    Dinic din(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        int64_t c;
        cin >> u >> v >> c;
        din.add(u, v, c);
    }
    cout << din.maxflow(s, t) << '\n';
    return 0;
}
