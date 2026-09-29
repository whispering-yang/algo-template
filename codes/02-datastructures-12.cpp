// 线段树合并：以洛谷 P3224 [HNOI2012] 永无乡为例
// n 座岛各有互不相同的排名（1~n 的排列），支持加边合并连通块、查询连通块内第 k 小排名的岛编号

#include <bits/stdc++.h>
using namespace std;

constexpr int N = 1e5 + 5;        // 岛数上界

array<int, N> dsu;                // 并查集父指针（路径压缩）
array<int, N> rt;                 // rt[i]：并查集根 i 对应的权值线段树根
array<int, N> rank_of;            // rank_of[i]：岛 i 的排名
array<int, N> island_of;          // island_of[r]：排名 r 对应的岛编号

// 动态开点权值线段树，值域为排名 [1, n]；n 次插入共新建约 n*log2(n) 个节点，池开 N << 5
array<int, N << 5> lc, rc, cnt;
int node_cnt;                     // 编号 0 保留为空节点

int find(int x) { return dsu[x] == x ? x : dsu[x] = find(dsu[x]); }

// 单点插入：排名 pos 的计数 +1
void insert(int& node, int l, int r, int pos) {
    if (!node) node = ++node_cnt;
    ++cnt[node];
    if (l == r) return;
    int mid = (l + r) >> 1;
    if (pos <= mid) insert(lc[node], l, mid, pos);
    else            insert(rc[node], mid + 1, r, pos);
}

// 合并：把树 y 并入树 x，返回新根；y 的节点被复用（此后 y 作废）
// 只在两棵树都存在的节点处递归；绝不能写 merge(x, x)
int merge(int x, int y) {
    if (!x || !y) return x | y;
    lc[x] = merge(lc[x], lc[y]);
    rc[x] = merge(rc[x], rc[y]);
    cnt[x] += cnt[y];
    return x;
}

// 第 k 小（要求 1 <= k <= cnt[node]），返回排名
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
    for (int i = 1; i <= n; ++i) {
        cin >> rank_of[i];
        island_of[rank_of[i]] = i;
    }
    iota(dsu.begin(), dsu.begin() + n + 1, 0);
    for (int i = 1; i <= n; ++i)
        insert(rt[i], 1, n, rank_of[i]);      // 每座岛初始单独成一个连通块

    for (int j = 0; j < m; ++j) {             // 初始 m 座桥
        int a, b;
        cin >> a >> b;
        int ra = find(a), rb = find(b);
        if (ra != rb) {
            dsu[ra] = rb;
            rt[rb] = merge(rt[rb], rt[ra]);
        }
    }

    int q;
    cin >> q;
    while (q--) {
        char op;
        int x, y;
        cin >> op >> x >> y;
        if (op == 'B') {                      // 加桥：合并两个连通块
            int rx = find(x), ry = find(y);
            if (rx != ry) {
                dsu[rx] = ry;
                rt[ry] = merge(rt[ry], rt[rx]);
            }
        } else {                              // 查询：连通块内第 y 小排名对应的岛
            int r = rt[find(x)];
            cout << (y > cnt[r] ? -1 : island_of[kth(r, 1, n, y)]) << '\n';
        }
    }
    return 0;
}
