#include <bits/stdc++.h>
using namespace std;

vector<vector<pair<int, long long>>> adj;
vector<long long> x;

// 添加约束 x[v] - x[u] <= w，即有向边 u -> v，边权 w
void add_constraint(int u, int v, long long w) {
    adj[u].emplace_back(v, w);
}

// 返回 true 时 x[1..n] 是一组可行解；false 表示无解
bool difference_constraints() {
    int n = (int)adj.size() - 1;
    x.assign(n + 1, 0);
    vector<int> cnt(n + 1, 0);
    vector<bool> inq(n + 1, false);
    queue<int> q;
    for (int u = 1; u <= n; u++) {
        q.push(u);  // 所有变量均从超级源可达，避免漏掉不连通的矛盾
        inq[u] = true;
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        inq[u] = false;
        for (auto [v, w] : adj[u]) {
            if (x[v] > x[u] + w) {
                x[v] = x[u] + w;
                cnt[v] = cnt[u] + 1;
                if (cnt[v] >= n) return false;
                if (!inq[v]) {
                    q.push(v);
                    inq[v] = true;
                }
            }
        }
    }
    return true;
}

// 使用前：读入变量数 n，将 adj resize 到 n + 1，再逐条调用 add_constraint
// x[v] - x[u] >= w：add_constraint(v, u, -w)
// x[v] - x[u] == w：同时添加上述 <= w 与 >= w 两条约束
// 整数变量的 x[v] - x[u] < w 可写成 <= w-1（实数变量不适用）
// 最坏 O(nm)；所有中间值及取负操作必须在 long long 范围内
