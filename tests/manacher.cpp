// Run from the repository root:
// g++ -std=c++17 -O2 -Wall -Wextra tests/manacher.cpp -o build/manacher_test.exe
#include <bits/stdc++.h>
using namespace std;

#define main manacher_example_main
#include "../codes/04-strings-08.cpp"
#undef main

int cases = 0;

void require(bool ok, const string &message) {
    if (!ok) throw runtime_error(message);
}

// Independent oracle: compare every substring from both ends.
bool brute_palindrome(const string &s, int l, int r) {
    while (l < r) {
        if (s[l++] != s[r--]) return false;
    }
    return true;
}

void check(const string &s) {
    ++cases;
    int n = (int)s.size();
    Manacher m(s);
    vector<int> odd(n), even(n);
    pair<int, int> best = {0, 0};
    long long total = 0;
    for (int l = 0; l < n; ++l) {
        for (int r = l; r < n; ++r) {
            bool palindrome = brute_palindrome(s, l, r);
            require(m.is_palindrome(l, r) == palindrome, "interval query");
            if (!palindrome) continue;
            ++total;
            int len = r - l + 1;
            if (len > best.second) best = {l, len};
            int mid = l + len / 2;
            if (len % 2) odd[mid] = max(odd[mid], len / 2 + 1);
            else even[mid] = max(even[mid], len / 2);
        }
    }
    require(m.odd == odd, "odd radii");
    require(m.even == even, "even radii");
    require(m.longest() == best, "longest / leftmost tie");
    require(m.count() == total, "substring count");
    require(!m.is_palindrome(-1, 0), "negative index");
    require(!m.is_palindrome(0, n), "past end");
    require(!m.is_palindrome(1, 0), "empty interval");
    // Repeated queries must leave the preprocessed radii intact.
    require(m.longest() == best && m.count() == total, "repeated calls");
}

int main() {
    for (int n = 0; n <= 12; ++n) {
        for (int mask = 0; mask < (1 << n); ++mask) {
            string s(n, 'a');
            for (int i = 0; i < n; ++i) s[i] += (mask >> i) & 1;
            check(s);
        }
    }
    mt19937 rng(20261025);
    // Includes whitespace, NUL and typical sentinel characters.
    const string alphabet = string("ab#^$ ") + '\0' + char(255);
    for (int t = 0; t < 1500; ++t) {
        string s(rng() % 65, ' ');
        for (char &c : s) c = alphabet[rng() % alphabet.size()];
        check(s);
    }
    // Large repeated input checks 64-bit counts and linear preprocessing.
    const int n = 200000;
    Manacher repeated(string(n, 'a'));
    require(repeated.longest() == make_pair(0, n), "large longest");
    require(repeated.count() == 1LL * n * (n + 1) / 2, "64-bit count");
    for (int i = 0; i < n; ++i) {
        require(repeated.odd[i] == min(i + 1, n - i), "large odd");
        require(repeated.even[i] == min(i, n - i), "large even");
    }
    cout << "PASS: " << cases << " oracle cases + " << n
         << "-character stress case (seed 20261025)\n";
}
