// g++ -std=c++17 -O2 -Wall -Wextra tests/integer_hash.cpp -o build/integer_hash_test.exe
#include <bits/stdc++.h>
using namespace std;

#define main integer_hash_example_main
#include "../codes/01-stdlib-05.cpp"
#undef main

void require(bool ok, const string &message) {
    if (!ok) throw runtime_error(message);
}

int main() {
    CustomHash hash;
    vector<long long> keys = {LLONG_MIN, LLONG_MAX, -1, 0, 1};
    mt19937_64 rng(20261027);
    unordered_map<long long, int, CustomHash> actual;
    unordered_set<long long, CustomHash> unique;
    map<long long, int> expected;
    actual.max_load_factor(0.7f);
    actual.reserve(50000);
    unique.max_load_factor(0.7f);
    unique.reserve(50000);
    // 按当前桶数构造等差键：默认恒等哈希下会落入同一桶。
    long long step = static_cast<long long>(actual.bucket_count());
    for (long long i = 0; i < 20000; ++i) keys.push_back(i * step);
    for (int i = 0; i < 20000; ++i) {
        long long x = static_cast<long long>(rng() >> 1);
        keys.push_back(i % 2 ? -x : x);
    }
    for (long long x : keys) {
        size_t before = hash(x);
        ++actual[x];
        ++expected[x];
        unique.insert(x);
        require(hash(x) == before && CustomHash{}(x) == before,
                "hash must stay stable across calls and hasher copies");
    }
    // 强制 rehash 后再次查询，检查哈希器状态是否稳定。
    actual.rehash(actual.bucket_count() * 2);
    unique.rehash(unique.bucket_count() * 2);
    require(actual.size() == expected.size(), "map size");
    require(unique.size() == expected.size(), "set size");
    for (auto [x, count] : expected) {
        require(actual.at(x) == count && unique.count(x) == 1,
                "signed boundary / random / arithmetic key lookup");
    }
    for (size_t i = 0; i < keys.size(); i += 3) {
        actual.erase(keys[i]);
        unique.erase(keys[i]);
        expected.erase(keys[i]);
    }
    for (long long x : keys) {
        require(actual.count(x) == expected.count(x), "erase map");
        require(unique.count(x) == expected.count(x), "erase set");
    }
    unordered_map<uint64_t, int, CustomHash> unsigned_keys;
    vector<uint64_t> extremes = {0, 1, uint64_t{1} << 63, UINT64_MAX};
    for (int i = 0; i < static_cast<int>(extremes.size()); ++i)
        unsigned_keys[extremes[i]] = i;
    for (int i = 0; i < static_cast<int>(extremes.size()); ++i)
        require(unsigned_keys.at(extremes[i]) == i, "unsigned 64-bit key");
    cout << "PASS: " << keys.size()
         << " signed keys, unsigned extremes, rehash and erase\n";
}
