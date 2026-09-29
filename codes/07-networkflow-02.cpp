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

    void add(int u, int v, int64_t cap) {
        g[u].push_back(e.size());
        e.push_back({v, cap});
        g[v].push_back(e.size());
        e.push_back({u, 0});
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
    Dinic din(n);
    vector<array<int, 2>> ed(m);  // 只需端点：容量可由残量推出
    for (int i = 0; i < m; ++i) {
        int u, v;
        int64_t c;
        cin >> u >> v >> c;
        ed[i] = {u, v};
        din.add(u, v, c);  // 第 i 条输入边的正边下标恰为 2 * i
    }

    int64_t cut = din.maxflow(s, t);

    // 残量网络上从 s 可达的点集 S（此时 t 不可达）
    vector<bool> inS(n + 1, false);
    queue<int> q;
    inS[s] = true;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int id : din.g[u]) {
            int v = din.e[id].to;
            if (din.e[id].cap > 0 && !inS[v]) {
                inS[v] = true;
                q.push(v);
            }
        }
    }

    cout << cut << '\n';
    vector<int> idx;
    for (int i = 0; i < m; ++i)
        if (inS[ed[i][0]] && !inS[ed[i][1]]) idx.push_back(i + 1);
    cout << idx.size();
    for (int x : idx) cout << ' ' << x;
    cout << '\n';
    return 0;
}
