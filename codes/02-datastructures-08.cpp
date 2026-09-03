// 李超线段树 (Li Chao Segment Tree)
// 用途：维护平面上多条线段，支持 O(log^2 C) 插入一条线段，O(log C) 查询
//       某横坐标处所有线段的最大值（或最小值，修改比较符号即可）。
// 核心思想：线段树的每个节点保存"在该区间中点处取值最大的线段"，
//           通过标记永久化避免下传，查询时沿路径取 max。
// 时间复杂度：加线段 O(log^2 XMAX)，查询 O(log XMAX)

#include <bits/stdc++.h>
using namespace std;

constexpr long double eps = 1e-9;

// 三值比较：lhs > rhs 返回 1，lhs < rhs 返回 -1，相等返回 0
int cmp(long double lhs, long double rhs) {
    if (rhs - lhs > eps) return -1;
    if (lhs - rhs > eps) return 1;
    return 0;
}

struct Line {
    long double k, d;        // y = k * x + d
    long double get(int x) const {
        return k * x + d;
    }
    // 通过两端点构造线段（若 x0==x1 退化为竖直线段，取最高点）
    void build(int x0, int y0, int x1, int y1) {
        if (x0 != x1) {
            k = 1.0L * (y1 - y0) / (x1 - x0);
            d = -k * x0 + y0;
        } else {
            k = 0.0L;
            d = 1.0L * max(y0, y1);
        }
    }
};

// ===== 使用前请修改以下常量 =====
constexpr int MAXN = 1e5 + 15;   // 最大线段数量
constexpr int XMAX = 40000;      // x 坐标范围（右开区间 [0, XMAX)）
// ================================


int lcnt = 0;                        // 当前线段总数
array<Line, MAXN> ls;                // 线段数组，编号从 1 开始
array<int, 4 * XMAX> tree{};         // 线段树节点中"优势线段"的编号

// 将编号为 lid 的线段插入到 node 管辖的区间 [l, r)
// 用于 add_segment 内部递归，一般不需直接调用
void add_line(int lid, int node, int l, int r) {
    int m = (l + r) >> 1;
    if (tree[node] == 0) {
        swap(lid, tree[node]);
        return;
    }
    // 保留在中点处取值更大的那条作为节点的优势线段
    if (cmp(ls[lid].get(m), ls[tree[node]].get(m)) > 0) {
        swap(lid, tree[node]);
    }
    if (r - l <= 1) return;
    // 将另一条线段尝试下放到子区间
    if (cmp(ls[lid].get(l), ls[tree[node]].get(l)) > 0) {
        add_line(lid, node << 1 | 0, l, m);
    } else {
        add_line(lid, node << 1 | 1, m, r);
    }
}

// 将编号为 lid 的线段插入到区间 [ql, qr)
void add_segment(int lid, int ql, int qr, int node = 1, int l = 0, int r = XMAX) {
    if (r <= ql || qr <= l) return;
    if (ql <= l && r <= qr) {
        add_line(lid, node, l, r);
        return;
    }
    int m = (l + r) >> 1;
    add_segment(lid, ql, qr, node << 1 | 0, l, m);
    add_segment(lid, ql, qr, node << 1 | 1, m, r);
}

// 查询 x = pos 处所有线段的最大值，返回线段编号
int query(int pos, int node = 1, int l = 0, int r = XMAX) {
    int ans = 0;
    long double maxh = -LDBL_MAX;
    while (true) {
        if (tree[node] != 0) {
            int cres = cmp(ls[tree[node]].get(pos), maxh);
            if (cres > 0 || (cres == 0 && tree[node] < ans)) {
                ans = tree[node];
                maxh = ls[tree[node]].get(pos);
            }
        }
        if (r - l <= 1) break;
        int m = (l + r) >> 1;
        if (pos < m) node = node << 1, r = m;
        else         node = node << 1 | 1, l = m;
    }
    return ans;
}

