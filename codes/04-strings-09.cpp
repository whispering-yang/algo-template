#include <bits/stdc++.h>
using namespace std;

class StringHash {
    static constexpr array<int, 2> mod{1000000007, 1000000009};
    vector<array<int, 2>> prefix, power;

public:
    explicit StringHash(const string &s)
        : prefix(s.size() + 1), power(s.size() + 1) {
        // 同一次运行中复用基数，允许比较不同字符串的子串。
        static const array<int, 2> base = [] {
            mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
            array<int, 2> result;
            for (int k = 0; k < 2; ++k) {
                result[k] = uniform_int_distribution<int>(257, mod[k] - 2)(rng);
            }
            return result;
        }();

        power[0] = {1, 1};
        for (size_t i = 0; i < s.size(); ++i) {
            int value = static_cast<unsigned char>(s[i]) + 1;
            for (int k = 0; k < 2; ++k) {
                prefix[i + 1][k] = (1LL * prefix[i][k] * base[k] + value) % mod[k];
                power[i + 1][k] = 1LL * power[i][k] * base[k] % mod[k];
            }
        }
    }

    // 查询 0 下标半开区间 [l, r)，比较子串时还须比较长度。
    array<int, 2> get(size_t l, size_t r) const {
        assert(l <= r && r < prefix.size());
        array<int, 2> result;
        for (int k = 0; k < 2; ++k) {
            int removed = 1LL * prefix[l][k] * power[r - l][k] % mod[k];
            result[k] = (prefix[r][k] - removed + mod[k]) % mod[k];
        }
        return result;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    int q;
    if (!(cin >> s >> q)) return 0;
    StringHash hash(s);
    while (q--) {
        size_t l1, r1, l2, r2;
        cin >> l1 >> r1 >> l2 >> r2;
        bool same = r1 - l1 == r2 - l2 && hash.get(l1, r1) == hash.get(l2, r2);
        cout << (same ? "Yes\n" : "No\n");
    }
    return 0;
}
