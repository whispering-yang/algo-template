// Kruskal 最小生成树：无向图，点编号 1..n，边权可为负。
// 输入 n, m 及 m 条边 u v w；连通时输出最小生成树权值，否则输出 impossible。
#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v;
    long long w;
};

struct DSU {
    vector<int> fa, size;

    explicit DSU(int n) : fa(n + 1), size(n + 1, 1) {
        iota(fa.begin(), fa.end(), 0);
    }

    int find(int u) {
        while (fa[u] != u) {
            fa[u] = fa[fa[u]];
            u = fa[u];
        }
        return u;
    }

    bool merge(int u, int v) {
        u = find(u);
        v = find(v);
        if (u == v) return false;
        if (size[u] < size[v]) swap(u, v);
        fa[v] = u;
        size[u] += size[v];
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<Edge> edges(m);
    for (auto& [u, v, w] : edges) cin >> u >> v >> w;
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.w < b.w;
    });

    DSU dsu(n);
    long long answer = 0;
    int chosen = 0;
    for (const auto& [u, v, w] : edges) {
        if (!dsu.merge(u, v)) continue;  // 两端已连通，加入会形成环
        answer += w;
        if (++chosen == n - 1) break;
    }

    if (chosen != n - 1) cout << "impossible\n";
    else cout << answer << '\n';
    return 0;
}
