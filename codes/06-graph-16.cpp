// Prim 最小生成树（邻接表 + 小根堆）：无向图，点编号 1..n，边权可为负。
// 输入 n, m 及 m 条边 u v w；连通时输出最小生成树权值，否则输出 impossible。
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, long long>>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    // 堆元素为 (跨越当前生成树割的边权, 待加入的点)。
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    vector<bool> used(n + 1, false);
    pq.push({0, 1});
    long long answer = 0;
    int chosen = 0;
    while (!pq.empty() && chosen < n) {
        auto [w, u] = pq.top();
        pq.pop();
        if (used[u]) continue;        // 惰性删除过期候选边
        used[u] = true;
        chosen++;
        answer += w;
        for (auto [v, cost] : adj[u])
            if (!used[v]) pq.push({cost, v});
    }

    if (chosen != n) cout << "impossible\n";
    else cout << answer << '\n';
    return 0;
}
