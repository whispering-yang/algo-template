// =============================================================================
// File:     08-offline-01 — 普通莫队 (Basic Mo's Algorithm)
// =============================================================================
// 离线区间询问：统计量能 O(1) 地随单个元素进出区间而增删时，把询问按
// （左端点所在块，右端点）排序，双指针在数组上滑动，代替暴力反复扫描。
// 示例：m 次询问 [l,r] 内不同数的个数。换题只改 add/del 与答案 cur 的
// 维护方式，骨架不变。块长取 n/sqrt(m) 时总移动量 O(n*sqrt(m))；
// 奇数块内右端点升序、偶数块内降序（奇偶优化），约再省一半移动量。
// 约定：1 <= l <= r <= n。

#include <bits/stdc++.h>
using namespace std;

int n, m, block;
vector<int> a;       // 离散化后的数组（下标 1..n）
vector<int> cnt;     // cnt[c]：当前区间内值 c 的出现次数
int cur = 0;         // 当前答案：区间内不同值的个数

struct Query {
    int l, r, idx;
    bool operator<(const Query &rhs) const {
        int bl = l / block, br = rhs.l / block;
        if (bl != br) return bl < br;            // 先按左端点所在块
        return bl & 1 ? r > rhs.r : r < rhs.r;   // 块内按右端点，奇偶块反向
    }
};
vector<Query> qs;
vector<int> ans;

void add(int i) { if (++cnt[a[i]] == 1) ++cur; }
void del(int i) { if (--cnt[a[i]] == 0) --cur; }

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    a.resize(n + 1);
    vector<int> vals;
    vals.reserve(n);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
        vals.push_back(a[i]);
    }
    // 离散化：值域压缩到 [1, n]，cnt 即可按值开
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    for (int i = 1; i <= n; ++i)
        a[i] = int(lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin()) + 1;
    cnt.assign(n + 1, 0);

    qs.resize(m);
    for (int i = 0; i < m; ++i) {
        cin >> qs[i].l >> qs[i].r;
        qs[i].idx = i;
    }
    block = max<int>(1, int(n / sqrt(double(m))));   // 最优块长 B = n/sqrt(m)
    sort(qs.begin(), qs.end());
    ans.assign(m, 0);

    int l = 1, r = 0;                    // 空区间
    for (int i = 0; i < m; ++i) {
        while (l > qs[i].l) add(--l);    // 先扩张再收缩，指针不会越过空区间
        while (r < qs[i].r) add(++r);
        while (l < qs[i].l) del(l++);
        while (r > qs[i].r) del(r--);
        ans[qs[i].idx] = cur;
    }
    for (int i = 0; i < m; ++i) cout << ans[i] << '\n';
    return 0;
}
