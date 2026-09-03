#include <bits/stdc++.h>
using namespace std;

constexpr long long INF = 0x3f3f3f3f3f3f3f3fLL;
constexpr int N = 505;
int n, m;
long long d[N][N];

void floyd() {
    for (int k = 1; k <= n; k++)        // 枚举中转点
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
}

// 使用前：初始化 d[i][j] = INF，d[i][i] = 0，读入边权 d[u][v] = min(d[u][v], w)
// 支持负权边，但图中不能存在负环；结束后 d[i][j] 为 i 到 j 的最短路
