#include <bits/stdc++.h>
using namespace std;

struct Manacher {
    // 0-based：odd[i] 含中心，even[i] 的中心在 i-1 与 i 之间。
    vector<int> odd, even;

    explicit Manacher(const string &s) {
        int n = (int)s.size();
        odd.resize(n);
        even.resize(n);
        // [left, right] 为当前右端点最远的回文区间。
        for (int i = 0, left = 0, right = -1; i < n; ++i) {
            int radius = i > right ? 1
                : min(odd[left + (right - i)], right - i + 1);
            while (i - radius >= 0 && i + radius < n
                   && s[i - radius] == s[i + radius]) {
                ++radius;
            }
            odd[i] = radius;
            if (i + radius - 1 > right) {
                left = i - radius + 1;
                right = i + radius - 1;
            }
        }
        for (int i = 0, left = 0, right = -1; i < n; ++i) {
            int radius = i > right ? 0
                : min(even[left + (right - i) + 1], right - i + 1);
            while (i - radius - 1 >= 0 && i + radius < n
                   && s[i - radius - 1] == s[i + radius]) {
                ++radius;
            }
            even[i] = radius;
            if (i + radius - 1 > right) {
                left = i - radius;
                right = i + radius - 1;
            }
        }
    }

    // 返回 {起点, 长度}；并列取最左，空串返回 {0, 0}。
    pair<int, int> longest() const {
        int start = 0, length = 0;
        auto update = [&](int pos, int len) {
            if (len > length || (len == length && pos < start)) {
                start = pos;
                length = len;
            }
        };
        for (int i = 0; i < (int)odd.size(); ++i) {
            update(i - odd[i] + 1, 2 * odd[i] - 1);
            update(i - even[i], 2 * even[i]);
        }
        return {start, length};
    }

    // 按位置计数，相同内容在不同位置分别计入。
    long long count() const {
        return accumulate(odd.begin(), odd.end(), 0LL)
             + accumulate(even.begin(), even.end(), 0LL);
    }

    // 判断非空闭区间 [l, r]；下标非法时返回 false。
    bool is_palindrome(int l, int r) const {
        if (l < 0 || l > r || r >= (int)odd.size()) return false;
        int len = r - l + 1;
        int mid = l + len / 2;
        return len % 2 ? odd[mid] >= len / 2 + 1
                       : even[mid] >= len / 2;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;
    Manacher manacher(s);
    auto [start, length] = manacher.longest();
    cout << length << '\n';
    return 0;
}
