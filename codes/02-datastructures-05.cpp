// =============================================================================
// File:     02-datastructures-05 — 线段树 (Segment Tree：区间加 / 区间求和)
// =============================================================================

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5 + 5;              // 按需调整数组上界

int64_t init_values[MAXN];             // 原始数组 (1-indexed)，建树前填入
int64_t sum[MAXN << 2];                // sum[node]：节点管辖区间的元素和
int64_t lazy[MAXN << 2];               // lazy[node]：子树中每个元素待加的值（尚未下传）

// 用左右儿子的信息更新父节点
void push_up(int node) {
    sum[node] = sum[node << 1] + sum[node << 1 | 1];
}

// 建树：node 管辖区间 [lo, hi]，叶子直接取原始值
void build(int node, int lo, int hi) {
    if (lo == hi) {
        sum[node] = init_values[lo];
        return;
    }
    int mid = (lo + hi) >> 1;
    build(node << 1, lo, mid);
    build(node << 1 | 1, mid + 1, hi);
    push_up(node);
}

// 下传懒惰标记：left_size / right_size 为左右儿子管辖的元素个数
void push_down(int node, int left_size, int right_size) {
    if (lazy[node]) {
        lazy[node << 1] += lazy[node];
        lazy[node << 1 | 1] += lazy[node];
        sum[node << 1] += lazy[node] * left_size;
        sum[node << 1 | 1] += lazy[node] * right_size;
        lazy[node] = 0;
    }
}

// 区间加：将 [ql, qr] 内每个数加上 val
void range_add(int node, int lo, int hi, int ql, int qr, int64_t val) {
    if (ql <= lo && hi <= qr) {        // 当前区间被完全覆盖，打标记后不再下递
        sum[node] += val * (hi - lo + 1);
        lazy[node] += val;
        return;
    }
    int mid = (lo + hi) >> 1;
    push_down(node, mid - lo + 1, hi - mid);
    if (ql <= mid) range_add(node << 1, lo, mid, ql, qr, val);
    if (qr > mid)  range_add(node << 1 | 1, mid + 1, hi, ql, qr, val);
    push_up(node);
}

// 区间查询：返回 [ql, qr] 的元素和
int64_t range_query(int node, int lo, int hi, int ql, int qr) {
    if (ql <= lo && hi <= qr) return sum[node];
    int mid = (lo + hi) >> 1;
    push_down(node, mid - lo + 1, hi - mid);
    int64_t result = 0;
    if (ql <= mid) result += range_query(node << 1, lo, mid, ql, qr);
    if (qr > mid)  result += range_query(node << 1 | 1, mid + 1, hi, ql, qr);
    return result;
}
