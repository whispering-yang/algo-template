#include <bits/stdc++.h>
using namespace std;

struct CustomHash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        // 整次运行固定；不能在每次查询时重新生成随机盐。
        static const uint64_t salt =
            chrono::steady_clock::now().time_since_epoch().count();
        return static_cast<size_t>(splitmix64(x + salt));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    unordered_map<long long, int, CustomHash> frequency;
    unordered_set<long long, CustomHash> seen;
    // 先设置负载因子，再按预计的不同键数量预留空间。
    frequency.max_load_factor(0.7f);
    seen.max_load_factor(0.7f);
    frequency.reserve(n);
    seen.reserve(n);

    for (int i = 0; i < n; ++i) {
        long long x;
        cin >> x;
        ++frequency[x];
        seen.insert(x);
    }
    cout << seen.size() << '\n';
    return 0;
}
