// =============================================================================
// File:     02-datastructures-12 — 树状数组求逆序对 (Inversion Count by Fenwick Tree)
// =============================================================================
// 从右往左扫描，树状数组（值域为离散化后的排名 1..n）维护已出现值的个数，
// query(rk[i] - 1) 即右侧严格小于 a[i] 的元素个数，累加即为逆序对数。

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 5e5 + 5;

int n, a[MAXN], rk[MAXN];   // rk[i]：a[i] 离散化后的排名 (1..m)
int tree[MAXN];             // 树状数组：各排名的出现次数
int64_t cnt = 0;            // 逆序对最多 n(n-1)/2，须用 int64_t

int lowbit(int x) {
    return x & -x;
}

void modify(int x) {        // 排名 x 处计数 +1
    for (; x <= n; x += lowbit(x)) tree[x]++;
}

int query(int x) {          // 排名 1..x 的计数前缀和
    int res = 0;
    for (; x > 0; x -= lowbit(x)) res += tree[x];
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    for (int i = 1; i <= n; i++) cin >> a[i];

    // 离散化：值域可能很大或含负数，映射到不超过 n 的排名
    vector<int> vals(a + 1, a + n + 1);
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    for (int i = 1; i <= n; i++)
        rk[i] = lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin() + 1;

    // 从右往左：严格小于 a[i] 才计入，query(rk[i]-1) 恰好排除相等的元素
    for (int i = n; i >= 1; i--) {
        cnt += query(rk[i] - 1);
        modify(rk[i]);
    }

    cout << cnt << '\n';
    return 0;
}
