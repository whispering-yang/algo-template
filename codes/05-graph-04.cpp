#include <array>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

constexpr int MAXN = 5e5 + 15;
int n, m, dfn_cnt = 0, bcc_cnt = 0;
array<vector<int>, MAXN> adj, bcc_arr;
array<int, MAXN> dfn, low;
stack<int> stk;

void tarjan(int u, int p) {
    int sz = 0;
    low[u] = dfn[u] = ++dfn_cnt;
    stk.push(u);
    for (auto v : adj[u]) {
        if (dfn[v] == 0) {
            sz++;
            tarjan(v, u);
            low[u] = min(low[u], low[v]);
            if (low[v] >= dfn[u]) {
                while (stk.top() != v) {
                    bcc_arr[bcc_cnt].push_back(stk.top());
                    stk.pop();
                }
                bcc_arr[bcc_cnt].push_back(stk.top());
                stk.pop();
                bcc_arr[bcc_cnt++].push_back(u);
            }
        } else if (v != p) {
            low[u] = min(low[u], dfn[v]);
        }
    }

    if (u == p && sz == 0) {
        bcc_arr[bcc_cnt++].push_back(u);
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
            stk = {};
            tarjan(i, i);
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
