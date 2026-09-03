#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200005;

int n, m;                    // n: 数组长度, m: 离散化后值域
int a[MAXN];                 // 原数组 (1-indexed)
vector<int> uniq;            // 离散化

// 主席树
int lc[MAXN << 5], rc[MAXN << 5], cnt[MAXN << 5];  // 左右儿子, 节点计数
int rt[MAXN], node_cnt;      // rt[i]: 版本 i 的根节点, node_cnt: 节点计数器

// 建空树（版本 0）[l, r)
int build(int l, int r) {
    int node = node_cnt++;
    if (r - l <= 1) {
        cnt[node] = 0;
        return node;
    }
    int m = (l + r) >> 1;
    lc[node] = build(l, m);
    rc[node] = build(m, r);
    cnt[node] = cnt[lc[node]] + cnt[rc[node]];
    return node;
}

// 在版本 src 的基础上在位置 pos 加 1，返回新版本根节点编号 [l, r)
int modify(int src, int pos, int l, int r) {
    int dst = node_cnt++;
    lc[dst] = lc[src], rc[dst] = rc[src], cnt[dst] = cnt[src] + 1;
    if (r - l <= 1) return dst;

    int m = (l + r) >> 1;
    if (pos < m)
        lc[dst] = modify(lc[src], pos, l, m);
    else
        rc[dst] = modify(rc[src], pos, m, r);
    return dst;
}

// 查询区间第 k 小，u = rt[l-1], v = rt[r], [l, r)
int query(int u, int v, int k, int l, int r) {
    if (r - l <= 1) return l;

    int m = (l + r) >> 1;
    int left_cnt = cnt[lc[v]] - cnt[lc[u]];

    if (k <= left_cnt)
        return query(lc[u], lc[v], k, l, m);
    else
        return query(rc[u], rc[v], k - left_cnt, m, r);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
        uniq.push_back(a[i]);
    }

    // 离散化
    sort(uniq.begin(), uniq.end());
    uniq.erase(unique(uniq.begin(), uniq.end()), uniq.end());
    int sz = uniq.size();

    // 建空树 [0, sz)
    rt[0] = build(0, sz);

    // 依次插入元素，生成 n 个版本
    for (int i = 1; i <= n; ++i) {
        int pos = lower_bound(uniq.begin(), uniq.end(), a[i]) - uniq.begin();
        rt[i] = modify(rt[i - 1], pos, 0, sz);
    }

    // 查询区间第 k 小
    while (m--) {
        int l, r, k;
        cin >> l >> r >> k;
        int idx = query(rt[l - 1], rt[r], k, 0, sz);
        cout << uniq[idx] << '\n';           // 映射回原值
    }

    return 0;
}
