// =============================================================================
// File:     06-math-19 — Extended Euclidean Algorithm (扩展欧几里得)
// =============================================================================

#include <bits/stdc++.h>
using namespace std;

// 求 a*x + b*y = gcd(a, b) 的一组整数特解 (x, y)，返回 gcd(a, b)
// 递归回代：由 b*x1 + (a mod b)*y1 = g 展开得 x = y1, y = x1 - (a/b)*y1
// 时间复杂度 O(log min(a, b))
int64_t exgcd(int64_t a, int64_t b, int64_t& x, int64_t& y) {
    if (b == 0) { x = 1; y = 0; return a; }
    int64_t x1, y1;
    int64_t g = exgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// 求 a 在模 m 下的乘法逆元（要求 gcd(a, m) = 1）
// 与费马小定理不同，不要求 m 为质数
int64_t mod_inverse(int64_t a, int64_t m) {
    int64_t x, y;
    exgcd(a, m, x, y);                  // 解方程 a*x + m*y = gcd(a, m)
    // 若 exgcd 的返回值 gcd != 1，则逆元不存在
    return (x % m + m) % m;             // 将特解平移到 [0, m) 内
}

// -------- 使用示例 (main) --------
// int main() {
//     int64_t x, y;
//     int64_t g = exgcd(6, 9, x, y);
//     cout << g << " " << x << " " << y << "\n"; // 3 -1 1: 6*(-1) + 9*1 = 3
//     cout << mod_inverse(3, 7) << "\n";         // 5: 3*5 = 15 = 1 (mod 7)
//     return 0;
// }
