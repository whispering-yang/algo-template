// =============================================================================
// File:     02-datastructures-11 — 归并排序求逆序对 (Inversion Count by Merge Sort)
// =============================================================================
// 逆序对：满足 i < j 且 a[i] > a[j] 的二元组 (i, j) 的个数。
// 在归并左右两个有序段时统计：取出右段元素 a[j] 时，
// 左段剩余元素 a[i..mid] 均大于 a[j]，一次性贡献 (mid - i + 1) 对。

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 5e5 + 5;

int n, a[MAXN], tmp[MAXN];
int64_t cnt = 0;    // 逆序对最多 n(n-1)/2，约 1.25e11，须用 int64_t

void merge_sort(int l, int r) {     // 对闭区间 [l, r] 排序并统计逆序对
    if (l >= r) return;
    int mid = (l + r) >> 1;
    merge_sort(l, mid);
    merge_sort(mid + 1, r);

    int i = l, j = mid + 1, k = l;
    while (i <= mid && j <= r) {
        if (a[i] <= a[j]) {         // 相等时优先取左段，保证相等元素不计入逆序对
            tmp[k++] = a[i++];
        } else {
            cnt += mid - i + 1;     // a[i..mid] 与 a[j] 均构成逆序对
            tmp[k++] = a[j++];
        }
    }
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= r) tmp[k++] = a[j++];
    for (int p = l; p <= r; p++) a[p] = tmp[p];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    for (int i = 1; i <= n; i++) cin >> a[i];

    merge_sort(1, n);

    cout << cnt << '\n';
    return 0;
}
