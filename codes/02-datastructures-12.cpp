// =============================================================================
// File:     02-datastructures-12 — 带修莫队 (Mo's Algorithm with Updates)
// =============================================================================
// 在普通莫队上加入第三维「时间」：修改与询问穿插读入后一起离线处理。
// 询问按（左端点所在块，右端点所在块，时间戳）排序；除 l、r 双指针外，
// 时间指针 T 在修改序列上前移为执行、后移为撤销。执行第 i 个修改时
// swap(a[p], upd[i].val) 使 val 变成旧值，再 swap 一次即还原，故执行与
// 撤销共用同一段代码。块长取 n^(2/3)，询问数与 n 同阶时总移动量
// O(n^(5/3))。示例：单点改值 + 询问 [l,r] 内不同数的个数。
// 约定：修改为 a[p] <- v（1 <= p <= n），询问满足 1 <= l <= r <= n。

#include <bits/stdc++.h>
using namespace std;

constexpr int MAXN = 133333 + 15;   // 数颜色规模，按题调整

int n, m, block;
int a[MAXN];          // 数组当前状态（下标 1..n，离散化后）
int cur = 0;          // 当前答案：区间内不同值的个数
int cl = 1, cr = 0, ct = 0;   // 当前区间与已执行的修改数
vector<int> cnt;      // cnt[c]：当前区间内值 c 的出现次数

struct Update {
    int pos, val;     // 修改：a[pos] <- val（离散化后的值）
} upd[MAXN];

struct Query {
    int l, r, t, idx;         // t：该询问之前已发生的修改数
    bool operator<(const Query &rhs) const {
        if (l / block != rhs.l / block) return l / block < rhs.l / block;
        if (r / block != rhs.r / block) return r / block < rhs.r / block;
        return t < rhs.t;
    }
} qs[MAXN];

int ans[MAXN];

void add(int i) { if (++cnt[a[i]] == 1) ++cur; }
void del(int i) { if (--cnt[a[i]] == 0) --cur; }

// 执行/撤销第 i 个修改：连续调用两次即状态还原（swap 的自反性）
void apply_update(int i) {
    int p = upd[i].pos;
    if (cl <= p && p <= cr) del(p);   // 旧值离开区间
    swap(a[p], upd[i].val);           // 新旧值互换
    if (cl <= p && p <= cr) add(p);   // 新值进入区间
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    vector<int> vals;
    vals.reserve(n + m);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
        vals.push_back(a[i]);
    }
    int totq = 0, totupd = 0;
    for (int j = 0; j < m; ++j) {
        char op;
        int x, y;
        cin >> op >> x >> y;
        if (op == 'Q') {
            qs[totq] = {x, y, totupd, totq};
            ++totq;
        } else {                      // 修改：a[x] <- y
            upd[totupd] = {x, y};
            vals.push_back(y);
            ++totupd;
        }
    }
    // 离散化：初始值与全部修改值一起压到 [1, n + 修改数]
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    auto id = [&](int v) {
        return int(lower_bound(vals.begin(), vals.end(), v) - vals.begin()) + 1;
    };
    for (int i = 1; i <= n; ++i) a[i] = id(a[i]);
    for (int i = 0; i < totupd; ++i) upd[i].val = id(upd[i].val);
    cnt.assign(vals.size() + 1, 0);

    block = max<int>(1, int(pow(n, 2.0 / 3)));   // 块长 n^(2/3)
    sort(qs, qs + totq);

    for (int i = 0; i < totq; ++i) {
        while (cl > qs[i].l) add(--cl);
        while (cr < qs[i].r) add(++cr);
        while (cl < qs[i].l) del(cl++);
        while (cr > qs[i].r) del(cr--);
        while (ct < qs[i].t) apply_update(ct++);   // 补执行修改
        while (ct > qs[i].t) apply_update(--ct);   // 撤销多余修改
        ans[qs[i].idx] = cur;
    }
    for (int i = 0; i < totq; ++i) cout << ans[i] << '\n';
    return 0;
}
