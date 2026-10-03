// 可持久化数组（洛谷 P3919）：单点修改 / 任意历史版本单点查询
// 可持久化线段树的直接应用：只维护叶子处的值，每次修改克隆 O(log n) 个节点

#include <bits/stdc++.h>
using namespace std;

constexpr int N = 1e5 + 5;             // 数组长度上界
constexpr int M = 1e5 + 5;             // 操作次数上界
constexpr int POOL = 2 * N + 20 * M;   // 建树 2n-1 个节点 + 每次修改克隆 O(log n) 个

array<int, POOL> lc, rc, val;          // 节点池：val 仅叶子有意义
vector<int> rt;                        // rt[v]：版本 v 的根（版本 0 为初始数组）
int node_cnt;

// 对 a[1..n] 建树返回版本 0 的根，区间为闭区间 [l, r]
int build(const vector<int>& a, int l, int r) {
    int node = node_cnt++;
    if (l == r) {
        val[node] = a[l];
        return node;
    }
    int mid = (l + r) >> 1;
    lc[node] = build(a, l, mid);
    rc[node] = build(a, mid + 1, r);
    return node;
}

// 基于版本 src 把 pos 改为 v：克隆根到叶的路径返回新根，旧版本不受影响
int modify(int src, int pos, int v, int l, int r) {
    int dst = node_cnt++;
    lc[dst] = lc[src], rc[dst] = rc[src];
    if (l == r) {
        val[dst] = v;
        return dst;
    }
    int mid = (l + r) >> 1;
    if (pos <= mid) lc[dst] = modify(lc[src], pos, v, l, mid);
    else            rc[dst] = modify(rc[src], pos, v, mid + 1, r);
    return dst;
}

// 查询版本 node 中位置 pos 的值：只读，不产生新节点
int query(int node, int pos, int l, int r) {
    while (l < r) {
        int mid = (l + r) >> 1;
        if (pos <= mid) node = lc[node], r = mid;
        else            node = rc[node], l = mid + 1;
    }
    return val[node];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];

    rt.resize(m + 1);
    rt[0] = build(a, 1, n);

    // P3919 约定：无论修改还是查询，第 i 次操作都生成版本 i（查询生成的版本与所查版本相同）
    for (int i = 1; i <= m; ++i) {
        int v, op, p;
        cin >> v >> op >> p;
        if (op == 1) {
            int c;
            cin >> c;
            rt[i] = modify(rt[v], p, c, 1, n);
        } else {
            rt[i] = rt[v];
            cout << query(rt[v], p, 1, n) << '\n';
        }
    }
    return 0;
}
