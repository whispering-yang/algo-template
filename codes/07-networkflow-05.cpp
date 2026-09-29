// =============================================================================
// File:     07-networkflow-05 — 二分图带权匹配 (Kuhn-Munkres / KM)
// =============================================================================
// 左右两部点带完全二分图的边权矩阵，求权和最大的完美匹配（每个左点都匹配）。
// 输入：n m（约定 n <= m），随后 n 行每行 m 个整数：w[i][j]，权值可负。
// 输出：最大权完美匹配的权值和。
// 核心思想：给每个点一个可行顶标 lx[i] + ly[j] >= w[i][j]，只在
//       lx[i] + ly[j] == w[i][j] 的「相等子图」上找完美匹配；找不到时将
//       顶标整体松弛 d = min{lx[i] + ly[j] - w[i][j]}，逐步扩大相等子图。
// 约定：必须给出完全矩阵（不存在的边用足够小的权值填充，如 -1e15），
//       权值绝对值 <= 1e15，保证顶标运算不溢出 int64。
// 复杂度：O(n^2 * m)。

#include <bits/stdc++.h>
using namespace std;

struct KM {
    int n, m;
    vector<vector<int64_t>> w;
    vector<int64_t> lx, ly, slack;  // slack[v]：本轮所有已访问左点到 v 的最小差值
    vector<bool> visx, visy;
    vector<int> matchy;             // 右点匹配到的左点，0 表示未匹配
    static constexpr int64_t INF = INT64_MAX / 4;

    KM(int n, int m) : n(n), m(m), w(n + 1, vector<int64_t>(m + 1)),
                       lx(n + 1, 0), ly(m + 1, 0), slack(m + 1, INF),
                       visx(n + 1, false), visy(m + 1, false), matchy(m + 1, 0) {}

    bool dfs(int u) {
        visx[u] = true;
        for (int v = 1; v <= m; ++v) {
            if (visy[v]) continue;
            int64_t diff = lx[u] + ly[v] - w[u][v];
            if (diff == 0) {  // 相等边：可进入交替路
                visy[v] = true;
                if (!matchy[v] || dfs(matchy[v])) {
                    matchy[v] = u;
                    return true;
                }
            } else {
                slack[v] = min(slack[v], diff);
            }
        }
        return false;
    }

    int64_t solve() {
        for (int i = 1; i <= n; ++i)
            for (int j = 1; j <= m; ++j) lx[i] = max(lx[i], w[i][j]);

        for (int i = 1; i <= n; ++i) {
            while (true) {
                fill(slack.begin(), slack.end(), INF);
                fill(visx.begin(), visx.end(), false);
                fill(visy.begin(), visy.end(), false);
                if (dfs(i)) break;
                // 未找到增广路：顶标松弛量 d 取未访问右点的最小 slack
                int64_t d = INF;
                for (int v = 1; v <= m; ++v)
                    if (!visy[v]) d = min(d, slack[v]);
                for (int u = 1; u <= n; ++u)
                    if (visx[u]) lx[u] -= d;
                for (int v = 1; v <= m; ++v)
                    if (visy[v]) ly[v] += d;
            }
        }

        int64_t ans = 0;
        for (int v = 1; v <= m; ++v)
            if (matchy[v]) ans += w[matchy[v]][v];
        return ans;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    KM km(n, m);
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j) cin >> km.w[i][j];
    cout << km.solve() << '\n';
    return 0;
}
