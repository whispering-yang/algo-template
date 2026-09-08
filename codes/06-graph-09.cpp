// 2-SAT (2-Satisfiability)
// 使用 Tarjan SCC 判定可满足性并构造解
// 变量编号 1~n, 节点 x + a*n 表示文字 (x == a)
// a=0 → x 为 假, a=1 → x+n 为 真
// 子句 (x=a) || (y=b) 转化为蕴含边:
//   ~(x=a) -> (y=b)  (若 x 不取 a, 则 y 必取 b)
//   ~(y=b) -> (x=a)  (若 y 不取 b, 则 x 必取 a)

#include <bits/stdc++.h>
using namespace std;

struct TwoSAT {
    int n;
    vector<vector<int>> adj;
    vector<int> dfn, low, scc;
    vector<char> in_stk;
    stack<int> stk;
    int dfn_cnt, scc_cnt;

    TwoSAT(int _n) : n(_n) {
        adj.assign(2 * n + 2, vector<int>());
        dfn.assign(2 * n + 2, 0);
        low.assign(2 * n + 2, 0);
        scc.assign(2 * n + 2, 0);
        in_stk.assign(2 * n + 2, false);
        dfn_cnt = scc_cnt = 0;
    }

    // 添加子句 (x == a) || (y == b), 其中 a,b ∈ {0,1}
    // 原理: ~(x=a) -> (y=b) 且 ~(y=b) -> (x=a)
    // x + a*n = 节点(x=a), x + (!a)*n = 节点(x≠a)
    void add_clause(int x, int a, int y, int b) {
        adj[x + (!a) * n].push_back(y + b * n);    // ~(x=a) -> (y=b)
        adj[y + (!b) * n].push_back(x + a * n);    // ~(y=b) -> (x=a)
    }

    // Tarjan SCC: 一次 DFS 求出所有强连通分量
    // 原理: dfn 为 DFS 序, low 为能回到的最早 dfn
    // 当 low[u]==dfn[u] 时, u 是 SCC 的根, 弹栈获得整个 SCC
    void tarjan(int u) {
        low[u] = dfn[u] = ++dfn_cnt;
        in_stk[u] = true;
        stk.push(u);

        for (int v : adj[u]) {
            if (dfn[v] == 0) {          // 树边
                tarjan(v);
                low[u] = min(low[u], low[v]);
            } else if (in_stk[v]) {     // 回边 (v 在栈中, 属于当前 SCC)
                low[u] = min(low[u], dfn[v]);
            }
        }

        if (low[u] == dfn[u]) {         // u 能扎起口袋
            scc_cnt++;
            while (true) {
                int t = stk.top();
                stk.pop();
                scc[t] = scc_cnt;
                in_stk[t] = false;
                if (t == u) break;
            }
        }
    }

    // 返回 true 表示存在可行解, answer 数组存储赋值 (1=true, 0=false)
    bool solve(vector<int>& answer) {
        // 对所有节点运行 Tarjan, 包括未连通的孤立节点
        for (int i = 1; i <= 2 * n; i++) {
            if (dfn[i] == 0) tarjan(i);
        }

        // 检查是否存在变量 i 使得 i 和 i+n 在同一 SCC 中
        for (int i = 1; i <= n; i++) {
            if (scc[i] == scc[i + n]) {
                return false; // 不可满足
            }
        }

        // 构造解: SCC 编号越小 → 拓扑序越靠后
        // i 为假节点, i+n 为真节点 → 靠后的赋值成立
        answer.assign(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            answer[i] = scc[i] < scc[i + n] ? 0 : 1;
        }
        return true;
    }
};
