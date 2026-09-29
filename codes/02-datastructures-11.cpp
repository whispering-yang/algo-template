// 动态开点线段树：单点加 / 区间计数 / 全局第 k 小，值域可达 1e9 而无需离散化

#include <bits/stdc++.h>
using namespace std;

constexpr int N = 1e5 + 5;        // 单点修改次数上界（决定节点池大小）
constexpr int V = 1e9;            // 值域上界，闭区间 [1, V]

// 每次单点修改至多新建 ceil(log2 V) ≈ 30 个节点，节点池开 N << 5
array<int, N << 5> lc, rc, cnt;   // 左右儿子编号、区间内元素个数
int node_cnt;                     // 已用节点数；编号 0 保留为空节点
int root;                         // 树根，初始为 0 表示空树

// 空节点约定：lc[0] = rc[0] = cnt[0] = 0，缺失的儿子可直接当权值 0 的节点参与运算

// 单点修改：位置 pos 的计数加 val（[l, r] 为 node 管辖的值域）
void modify(int& node, int l, int r, int pos, int val) {
    if (!node) node = ++node_cnt;             // 首次访问，开点
    cnt[node] += val;
    if (l == r) return;
    int mid = l + ((r - l) >> 1);             // 值域 1e9 时 l + r 会溢出 int
    if (pos <= mid) modify(lc[node], l, mid, pos, val);
    else            modify(rc[node], mid + 1, r, pos, val);
}

// 区间查询 [ql, qr] 内的元素个数
int query(int node, int l, int r, int ql, int qr) {
    if (!node) return 0;                      // 空子树贡献为 0
    if (ql <= l && r <= qr) return cnt[node];
    int mid = l + ((r - l) >> 1);
    int res = 0;
    if (ql <= mid) res += query(lc[node], l, mid, ql, qr);
    if (qr > mid)  res += query(rc[node], mid + 1, r, ql, qr);
    return res;
}

// 全局第 k 小（要求 1 <= k <= cnt[root]），返回元素值
int kth(int node, int l, int r, int k) {
    while (l < r) {
        int mid = l + ((r - l) >> 1);
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
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;                             // 1 <= x <= V，无需离散化
        modify(root, 1, V, x, 1);
    }
    while (m--) {
        int op;
        cin >> op;
        if (op == 1) {                        // 查询 [l, r] 内元素个数
            int l, r;
            cin >> l >> r;
            cout << query(root, 1, V, l, r) << '\n';
        } else {                              // 查询全局第 k 小
            int k;
            cin >> k;
            cout << kth(root, 1, V, k) << '\n';
        }
    }
    return 0;
}
