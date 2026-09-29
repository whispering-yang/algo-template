// 线段树分裂（含合并/插入/查询）：以洛谷 P5494【模板】线段树分裂为例
// 维护若干可重集（多棵定义在值域 [1, n] 上的动态开点权值线段树）：
//   0 p x y：把可重集 p 中值在 [x, y] 的部分分裂成新可重集
//   1 p t  ：把可重集 t 并入 p（此后 t 作废，不再出现）
//   2 p x q：向 p 加入 x 个数值 q
//   3 p x y：查询 p 中值在 [x, y] 的元素个数
//   4 p k  ：查询 p 中第 k 小，不存在输出 -1

#include <bits/stdc++.h>
using namespace std;

constexpr int N = 2e5 + 5;        // 值域与操作数上界（P5494: n, m <= 2e5）

// 初始 O(n) 建树约 2n 个节点，此后每次分裂/插入至多新建 O(log n) 个，池开 N << 5
array<int, N << 5> lc, rc;
array<int64_t, N << 5> cnt;       // 计数总量可达 m*m 级别，必须用 64 位
int node_cnt;                     // 编号 0 保留为空节点
array<int, N> rt;                 // rt[i]：可重集 i 的根；分裂产生的新集编号递增
int set_cnt = 1;

array<int64_t, N> a;              // 初始每个值的出现次数

// O(n) 建初始树：叶子 cnt = a[pos]
int build(int l, int r) {
    int node = ++node_cnt;
    if (l == r) {
        cnt[node] = a[l];
        return node;
    }
    int mid = (l + r) >> 1;
    lc[node] = build(l, mid);
    rc[node] = build(mid + 1, r);
    cnt[node] = cnt[lc[node]] + cnt[rc[node]];
    return node;
}

// 单点修改：位置 pos 的计数加 val
void modify(int& node, int l, int r, int pos, int64_t val) {
    if (!node) node = ++node_cnt;
    cnt[node] += val;
    if (l == r) return;
    int mid = (l + r) >> 1;
    if (pos <= mid) modify(lc[node], l, mid, pos, val);
    else            modify(rc[node], mid + 1, r, pos, val);
}

// 合并：把树 y 并入树 x，返回新根；y 的节点被复用（此后 y 作废）
int merge(int x, int y) {
    if (!x || !y) return x | y;
    lc[x] = merge(lc[x], lc[y]);
    rc[x] = merge(rc[x], rc[y]);
    cnt[x] += cnt[y];
    return x;
}

// 分裂：把树 node 中值域 [ql, qr] 的部分抽成新树并返回其根，原树原地剔除该部分
int split(int& node, int l, int r, int ql, int qr) {
    if (!node) return 0;                      // 空树抽不出元素
    if (ql <= l && r <= qr) {                 // 整棵子树都在抽取范围内，零拷贝搬走
        int moved = node;
        node = 0;
        return moved;
    }
    int y = ++node_cnt;                       // 新树在当前层的节点
    int mid = (l + r) >> 1;
    if (ql <= mid) lc[y] = split(lc[node], l, mid, ql, qr);
    if (qr > mid)  rc[y] = split(rc[node], mid + 1, r, ql, qr);
    cnt[y] = cnt[lc[y]] + cnt[rc[y]];
    cnt[node] -= cnt[y];
    return y;
}

// 区间查询 [ql, qr] 内的元素个数
int64_t query(int node, int l, int r, int ql, int qr) {
    if (!node || qr < l || r < ql) return 0;
    if (ql <= l && r <= qr) return cnt[node];
    int mid = (l + r) >> 1;
    int64_t res = 0;
    if (ql <= mid) res += query(lc[node], l, mid, ql, qr);
    if (qr > mid)  res += query(rc[node], mid + 1, r, ql, qr);
    return res;
}

// 第 k 小（要求 1 <= k <= cnt[node]），返回元素值
int kth(int node, int l, int r, int k) {
    while (l < r) {
        int mid = (l + r) >> 1;
        if (k <= cnt[lc[node]]) { node = lc[node]; r = mid; }
        else                    { k -= cnt[lc[node]]; node = rc[node]; l = mid + 1; }
    }
    return l;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    rt[1] = build(1, n);

    while (m--) {
        int op;
        cin >> op;
        if (op == 0) {
            int p, x, y;
            cin >> p >> x >> y;
            rt[++set_cnt] = split(rt[p], 1, n, x, y);
        } else if (op == 1) {
            int p, t;
            cin >> p >> t;
            rt[p] = merge(rt[p], rt[t]);
        } else if (op == 2) {
            int p, x, q;
            cin >> p >> x >> q;
            modify(rt[p], 1, n, q, x);
        } else if (op == 3) {
            int p, x, y;
            cin >> p >> x >> y;
            cout << query(rt[p], 1, n, x, y) << '\n';
        } else {
            int p, k;
            cin >> p >> k;
            if (k > cnt[rt[p]]) cout << -1 << '\n';
            else                cout << kth(rt[p], 1, n, k) << '\n';
        }
    }
    return 0;
}
