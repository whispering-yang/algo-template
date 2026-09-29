#include <bits/stdc++.h>
using namespace std;

const long double PI = acosl(-1.0L);

int n, m;
constexpr int MAXN = 2e6;
// C++ 17 以上可以使用 <bit> 库的 bit_ceil()
constexpr int MAXL = 1 << (32 - __builtin_clz(unsigned(MAXN)));
array<complex<long double>, MAXL> fs, gs;
array<int, MAXL> rev;

// len 必须是 2 的幂次
void bit_reversal(auto &f, int len) {
    for (int i = 0; i < len; i++) {
        rev[i] = rev[i >> 1] >> 1;
        if (i & 1) rev[i] |= len >> 1;
    }
    for (int i = 0; i < len; i++) {
        if (i < rev[i]) swap(f[i], f[rev[i]]);
    }
}

// 不需要 rev 数组的位逆序置换
void bit_reverse(vector<int>& a) {
    int n = (int)a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
}

// d = 1 时为 fft, d = -1 时为 ifft
void fft(auto &f, int len, int d) {
    bit_reversal(f, len);
    for (int h = 2; h <= len; h <<= 1) {
        auto wn = complex<long double>(cosl(2 * PI / h), sinl(d * 2 * PI / h));
        for (int j = 0; j < len; j += h) {
            auto wk = complex<long double>(1, 0);
            for (int k = j; k < j + h / 2; k++) {
                auto u = f[k];
                auto t = wk * f[k + h / 2];
                f[k] = u + t;
                f[k + h / 2] = u - t;
                wk = wk * wn;
            }
        }
    }
    if (d == -1) {
        for (int i = 0; i < len; i++) {
            f[i] /= len;
        }
    }
}
