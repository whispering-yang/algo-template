#include <bits/stdc++.h>
using namespace std;

struct SplitMix64 {
    uint64_t seed;
    SplitMix64() {
        uint64_t t = chrono::steady_clock::now().time_since_epoch().count();
        uint64_t s = chrono::system_clock::now().time_since_epoch().count();
        seed = (t * 131 + s) * 0x9e3779b97f4a7c15ULL ^ (uint64_t)(uintptr_t)this;
    }
    uint64_t operator()(uint64_t x) const {
        x += seed;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    unordered_map<long long, int, SplitMix64> frequency;
    unordered_set<long long, SplitMix64> seen;
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
