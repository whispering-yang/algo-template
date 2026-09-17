#include <iostream>
#include <vector>
using namespace std;

// 圆方树 (Block-Cut Tree)
// 圆点 1~n 对应原图的点；方点 n+1~tot 每个对应一个点双连通分量，
// 与该点双内的所有圆点连边。
int n, m, dfn_cnt = 0, tot = 0;  // tot 初值为 n，每发现一个点双 ++tot
vector<vector<int>> adj, vt;     // 原图邻接表；圆方树（方点编号 n+1..tot）
vector<int> dfn, low, stk;
int top = 0;

void link(int u, int v) {
    vt[u].push_back(v);
    vt[v].push_back(u);
}

void tarjan(int u) {
    dfn[u] = low[u] = ++dfn_cnt;
    stk[++top] = u;
    for (int v : adj[u]) {
        if (dfn[v] == 0) {
            tarjan(v);
            low[u] = min(low[u], low[v]);
            if (low[v] >= dfn[u]) {        // v 子树回不到 u 上方，弹出一个点双
                ++tot;                      // 新建方点
                while (stk[top] != v) link(stk[top--], tot);
                link(stk[top--], tot);      // 弹栈直到弹出 v
                link(u, tot);               // 割点 u 可属于多个方点
            }
        } else {
            // 不跳过父节点：重边被当作回边，两条平行边恰好构成 {u, v} 点双
            low[u] = min(low[u], dfn[v]);
        }
    }
}

int main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    cin >> n >> m;
    tot = n;
    // 方点数 = 点双数 <= m（每条边恰属一个点双），故 tot <= n + m
    adj.resize(n + 1);
    vt.resize(n + m + 2);
    dfn.resize(n + 1);
    low.resize(n + 1);
    stk.resize(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    for (int i = 1; i <= n; i++) {
        if (dfn[i] == 0) {
            top = 0;
            tarjan(i);  // 孤立点不属于任何含边的点双，不会建方点，按需特判
        }
    }

    // 输出每个方点连接的圆点集合（即一个点双），可与点双模板输出对照
    cout << tot - n << '\n';
    for (int b = n + 1; b <= tot; b++) {
        cout << vt[b].size() << ' ';
        for (int j = 0; j < (int)vt[b].size(); j++) {
            cout << vt[b][j] << " \n"[j + 1 == (int)vt[b].size()];
        }
    }

    return 0;
}
