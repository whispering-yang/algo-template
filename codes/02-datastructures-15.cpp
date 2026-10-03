// 可持久化并查集（洛谷 P3402）：fa 与 siz 存于同一棵可持久化数组
// 按大小合并，禁用路径压缩：树高 O(log n) 保证 find 复杂度可控

#include <bits/stdc++.h>
using namespace std;

constexpr int N = 1e5 + 5;             // 元素个数上界
constexpr int M = 2e5 + 5;             // 操作次数上界
constexpr int POOL = 2 * N + 40 * M;   // 建树 2n + 每次合并两处单点修改各克隆 O(log n) 个

array<int, POOL> lc, rc;               // 节点池：叶子处同时存父指针与集合大小
array<int, POOL> fa, siz;
vector<int> rt;                        // rt[v]：版本 v 的根（版本 0 为初始状态）
int node_cnt;
int n;

// 建初始树：fa[i] = i, siz[i] = 1，闭区间 [l, r]
int build(int l, int r) {
    int node = node_cnt++;
    if (l == r) {
        fa[node] = l, siz[node] = 1;
        return node;
    }
    int mid = (l + r) >> 1;
    lc[node] = build(l, mid);
    rc[node] = build(mid + 1, r);
    return node;
}

// 基于版本 src 把叶子 pos 置为 (f, s)：克隆路径返回新根
int modify(int src, int pos, int f, int s, int l, int r) {
    int dst = node_cnt++;
    lc[dst] = lc[src], rc[dst] = rc[src];
    if (l == r) {
        fa[dst] = f, siz[dst] = s;
        return dst;
    }
    int mid = (l + r) >> 1;
    if (pos <= mid) lc[dst] = modify(lc[src], pos, f, s, l, mid);
    else            rc[dst] = modify(rc[src], pos, f, s, mid + 1, r);
    return dst;
}

// 定位版本 root 中叶子 pos 的节点编号（只读）
int locate(int root, int pos) {
    int l = 1, r = n;
    while (l < r) {
        int mid = (l + r) >> 1;
        if (pos <= mid) root = lc[root], r = mid;
        else            root = rc[root], l = mid + 1;
    }
    return root;
}

// 只读 find：不路径压缩；按大小合并保证树高 O(log n)，
// 每跳一次父指针需一次 O(log n) 的叶子定位，故单次 find 为 O(log^2 n)
int find(int root, int x) {
    int f = fa[locate(root, x)];
    while (f != x) {
        x = f;
        f = fa[locate(root, x)];
    }
    return x;
}

// 在版本 root 上合并 x, y 所在集合，返回新版本根（已连通时直接共享原树）
int unite(int root, int x, int y) {
    x = find(root, x), y = find(root, y);
    if (x == y) return root;
    int lx = locate(root, x), ly = locate(root, y);
    if (siz[lx] > siz[ly]) swap(x, y), swap(lx, ly);    // 小树挂大树
    int nrt = modify(root, x, y, siz[lx], 1, n);              // fa[x] = y
    return modify(nrt, y, y, siz[ly] + siz[lx], 1, n);        // siz[y] += siz[x]
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    cin >> n >> m;
    rt.resize(m + 1);
    rt[0] = build(1, n);

    // 三种操作都推进版本：1 合并、2 回退、3 查询（查询生成的版本与当前相同）
    for (int i = 1; i <= m; ++i) {
        int op;
        cin >> op;
        if (op == 1) {
            int a, b;
            cin >> a >> b;
            rt[i] = unite(rt[i - 1], a, b);
        } else if (op == 2) {
            int k;
            cin >> k;
            rt[i] = rt[k];              // O(1) 回退：复制根指针，不产生新节点
        } else {
            int a, b;
            cin >> a >> b;
            rt[i] = rt[i - 1];
            cout << (find(rt[i], a) == find(rt[i], b) ? 1 : 0) << '\n';
        }
    }
    return 0;
}
