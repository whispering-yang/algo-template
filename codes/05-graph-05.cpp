#include <array>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

constexpr int MAXN = 5e5 + 15;
array<vector<int>, MAXN> adj, bcc_arr;
array<int, MAXN> dfn, low, bcc;
array<bool, MAXN> in_stk;
int n, m, dfn_cnt = 0, bcc_cnt = 0;
stack<int> stk;

void tarjan(int u, int p) {
    low[u] = dfn[u] = ++dfn_cnt;
    in_stk[u] = true;
    stk.push(u);
    bool flag = false;
    for (auto v : adj[u]) {
        if (dfn[v] == 0) {
            tarjan(v, u);
            low[u] = min(low[u], low[v]);
        } else {
            if (v != p || flag) {
                low[u] = min(low[u], dfn[v]);
            } else {
                flag = true;
            }
        }
    }
    if (dfn[u] == low[u]) {
        while (stk.top() != u) {
            bcc_arr[bcc_cnt].push_back(stk.top());
            stk.pop();
        }
        bcc_arr[bcc_cnt++].push_back(stk.top());
        stk.pop();
    }
}

int main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    for (int i = 1; i <= n; i++) {
        if (dfn[i] == 0) {
            tarjan(i, 0);
        }
    }

    cout << bcc_cnt << '\n';
    for (int i = 0; i < bcc_cnt; i++) {
        int sz = bcc_arr[i].size();
        cout << sz << ' ';
        for (int j = 0; j < sz; j++) {
            cout << bcc_arr[i][j] << " \n"[j == sz - 1];
        }
    }

    return 0;
}
