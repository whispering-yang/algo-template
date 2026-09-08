// =============================================================================
// File:     06-graph-08 — 树形偏序计数 (Tree-Poset Topological Ordering Count)
// =============================================================================
// 偏序约束构成一棵外向树（每个非根节点恰有一个直接前驱，祖先必须排在子孙前）
// 时，拓扑序个数为
//     ans = n! / prod(subtree_size[u])
// 若约束是森林，建一个虚拟根连向所有树根，再套用同一公式即可（n 相应 +1）。
// =============================================================================

#include <bits/stdc++.h>
using namespace std;

constexpr int MOD = 1e9 + 7;

// 模意义快速幂：base^exp mod MOD
int64_t fpow(int64_t base, int64_t exp) {
    int64_t result = 1;
    base %= MOD;
    while (exp) {
        if (exp & 1) result = result * base % MOD;
        base = base * base % MOD;
        exp >>= 1;
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;                     // n 个节点，m 条约束（有向边 a -> b：a 必须排在 b 前）

    vector<vector<int>> children(n);   // 外向树：存每个节点的孩子
    vector<int> indegree(n, 0);
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        children[a].push_back(b);
        ++indegree[b];
    }

    // 树根：唯一入度为 0 的节点。若入度为 0 的节点多于一个，
    // 说明是森林，需建虚拟根连向所有根后再套用公式（见文件末尾注释）
    int root = -1;
    for (int i = 0; i < n; ++i) {
        if (indegree[i] == 0) {
            root = i;
            break;
        }
    }

    // 一次 DFS 求每个节点的子树大小 subtree_size[u]
    vector<int> subtree_size(n, 0);
    function<void(int)> compute_size = [&](int u) {
        subtree_size[u] = 1;
        for (int v : children[u]) {
            compute_size(v);
            subtree_size[u] += subtree_size[v];
        }
    };
    compute_size(root);

    // ans = n! / prod(subtree_size[u])，模意义下除法转为乘逆元（费马小定理）
    int64_t factorial = 1;
    for (int i = 2; i <= n; ++i) factorial = factorial * i % MOD;

    int64_t size_product = 1;
    for (int u = 0; u < n; ++u) size_product = size_product * subtree_size[u] % MOD;

    int64_t answer = factorial * fpow(size_product, MOD - 2) % MOD;
    cout << answer << "\n";

    return 0;
}

// 森林版本（多棵树）：建虚拟根 n 连向所有入度为 0 的节点，n' = n + 1，
// 对新树套用 ans = n'! / prod(subtree_size[u])，即可自动完成各棵树的交错合并。
