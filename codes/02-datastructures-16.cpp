#include <bits/stdc++.h>
using namespace std;

int n, top, root;
vector<long long> w;
vector<int> ls, rs, stk;

// 小根笛卡尔树，相等时靠左的下标优先；将 > 改为 < 即为大根树。
void build() {
    ls.assign(n + 1, 0);
    rs.assign(n + 1, 0);
    stk.resize(n + 1);
    top = 0;

    // stk 维护右链上的节点下标。
    for (int i = 1; i <= n; i++) {
        int k = top;  // top 为操作前的栈顶，k 为当前栈顶
        while (k > 0 && w[stk[k]] > w[i]) k--;
        if (k) rs[stk[k]] = i;
        if (k < top) ls[i] = stk[k + 1];
        stk[++k] = i;
        top = k;
    }
    root = top ? stk[1] : 0;
}

// 在 main 中读入 n，w.resize(n + 1)，读入 w[1..n] 后调用 build()。
// ls/rs 为左右儿子，0 表示空节点；空树 root = 0。支持重复建树。
