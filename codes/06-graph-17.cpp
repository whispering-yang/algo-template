// 最小 Kruskal 重构树：询问两点连通所需的最小边权阈值。
// 输入 n, m, q，随后 m 条无向边 u v w、q 个询问 u v。
// 不连通输出 impossible；u == v 时约定输出 0。边权可为负。
#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v;
    long long w;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m >> q;
    vector<Edge> edges(m);
    for (auto& [u, v, w] : edges) cin >> u >> v >> w;
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.w < b.w;
    });

    // 原图顶点为 1..n；每次有效合并新建一个节点，最多用到 2n-1。
    // dsu 的代表元就是当前连通块在重构树上的根，不按集合大小合并。
    vector<int> dsu(2 * n + 1), parent(2 * n + 1, 0);
    vector<long long> val(2 * n + 1, 0);  // 仅新建节点的点权有效
    iota(dsu.begin(), dsu.end(), 0);
    auto find = [&](int u) {
        int r = u;
        while (dsu[r] != r) r = dsu[r];
        while (dsu[u] != u) {
            int p = dsu[u];
            dsu[u] = r;  // 将路径上的节点直接连向代表元
            u = p;
        }
        return r;
    };

    int tot = n;
    for (const auto& [u, v, w] : edges) {
        int x = find(u), y = find(v);
        if (x == y) continue;
        ++tot;
        val[tot] = w;
        parent[x] = parent[y] = tot;  // 两棵重构树挂到新节点下
        dsu[x] = dsu[y] = tot;       // 新节点成为合并后连通块的代表元
    }

    int log = 1;
    while ((1LL << log) <= tot) ++log;
    vector<vector<int>> up(log, vector<int>(tot + 1, 0));
    vector<int> depth(tot + 1, 0);
    vector<int> root(tot + 1, 0);
    // 父节点编号总大于子节点；倒序即可先算出父节点的深度和倍增表。
    for (int u = tot; u >= 1; --u) {
        int p = parent[u];
        if (p != 0) depth[u] = depth[p] + 1;
        root[u] = p == 0 ? u : root[p];
        up[0][u] = p;
        for (int k = 1; k < log; k++) up[k][u] = up[k - 1][up[k - 1][u]];
    }

    auto lca = [&](int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        int diff = depth[u] - depth[v];
        for (int k = 0; k < log; k++)
            if ((diff >> k) & 1) u = up[k][u];
        if (u == v) return u;
        for (int k = log - 1; k >= 0; --k) {
            if (up[k][u] == up[k][v]) continue;
            u = up[k][u];
            v = up[k][v];
        }
        return up[0][u];
    };

    while (q--) {
        int u, v;
        cin >> u >> v;
        if (u == v) cout << "0\n";
        else if (root[u] != root[v]) cout << "impossible\n";
        else cout << val[lca(u, v)] << '\n';
    }
    return 0;
}
