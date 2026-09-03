// 拓扑排序方案数 - 状压 DP (n ≤ 20)
// 给定 n 个元素和若干 m 个形如 a_i < a_j 的约束
// 计算满足所有约束的排列总数
// 时间复杂度 O(n * 2^n)

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    // pre[i]: 元素 i 的前驱集合 (bitmask)
    vector<int> pre(n, 0);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b; // a < b
        // a 必须在 b 之前, 即 b 的前驱包含 a
        pre[b] |= 1 << a;
    }

    int S = 1 << n;
    vector<long long> dp(S, 0);
    dp[0] = 1;

    for (int mask = 0; mask < S; mask++) {
        if (dp[mask] == 0) continue;
        for (int i = 0; i < n; i++) {
            if (mask >> i & 1) continue;        // i 已选
            if ((mask & pre[i]) == pre[i]) {     // 所有前驱都已在 mask 中
                dp[mask | (1 << i)] += dp[mask];
            }
        }
    }

    cout << dp[S - 1] << "\n";
    return 0;
}
