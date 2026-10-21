#include <bits/stdc++.h>
using namespace std;

vector<int> fa, siz, L, R;

// 可选：先调用上一模板的 build()，再统计父亲、子树大小与区间。
void calc_subtrees() {
    fa.assign(n + 1, 0);
    siz.assign(n + 1, 0);
    L.assign(n + 1, 0);
    R.assign(n + 1, 0);
    vector<int> order;
    order.reserve(n);
    if (root) order.push_back(root);
    for (int i = 0; i < (int)order.size(); i++) {
        int u = order[i];
        if (ls[u]) {
            fa[ls[u]] = u;
            order.push_back(ls[u]);
        }
        if (rs[u]) {
            fa[rs[u]] = u;
            order.push_back(rs[u]);
        }
    }
    // 逆序保证先统计儿子，避免链状树递归爆栈。
    for (int i = (int)order.size() - 1; i >= 0; i--) {
        int u = order[i];
        siz[u] = siz[ls[u]] + siz[rs[u]] + 1;
        L[u] = ls[u] ? L[ls[u]] : u;
        R[u] = rs[u] ? R[rs[u]] : u;
    }
}
