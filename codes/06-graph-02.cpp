int n, m;
vector<vector<int>> adj;

int dfn_cnt = 0, scc_cnt = 0;
array<int, MAXN> dfn, low, scc;
array<bool, MAXN> in_stk;
stack<int> stk;

void tarjan(int node) {
    low[node] = dfn[node] = ++dfn_cnt;
    in_stk[node] = true;
    stk.push(node);

    for (auto nxt : adj[node]) {
        if (dfn[nxt] == 0) {
            tarjan(nxt);
            low[node] = min(low[nxt], low[node]);
        } else if (in_stk[nxt]) {
            low[node] = min(dfn[nxt], low[node]);
        }
    }

    if (low[node] == dfn[node]) {
        scc_cnt++;
        while (!stk.empty()) {
            int tp = stk.top();
            stk.pop();
            scc[tp] = scc_cnt;
            in_stk[tp] = false;
            if (tp == node) break;
        }
    }
}
