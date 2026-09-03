#include <iostream>
#include <optional>
#include <vector>
#include <algorithm>

// 扩展欧几里得：返回 gcd(a, b)，并求出 ax + by = gcd 的一组解
long long exgcd(long long a, long long b, long long& x, long long& y) {
    if (b == 0) { x = 1; y = 0; return a; }
    long long x1, y1;
    long long g = exgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// 求 a 在模 mod 意义下的逆元；当 gcd(a, mod) != 1 时逆元不存在
std::optional<long long> mod_inverse(long long a, long long mod) {
    long long x, y;
    if (exgcd(a, mod, x, y) != 1) return std::nullopt; // 无解：返回空
    return (x % mod + mod) % mod;                      // 有解：调整到 [0, mod)
}

// 返回有序数组中第一个 >= x 的元素；不存在则返回空
std::optional<int> first_ge(const std::vector<int>& a, int x) {
    auto it = std::lower_bound(a.begin(), a.end(), x);
    if (it == a.end()) return std::nullopt;
    return *it;
}

int main() {
    using std::cout; using std::endl;

    // 基本操作
    std::optional<int> empty;          // 默认构造为空
    std::optional<int> val = 42;       // 包含值 42
    cout << empty.has_value() << " " << val.has_value() << endl; // 0 1
    cout << empty.value_or(-1) << endl; // -1（空值时返回默认值）

    val.reset();                       // 置空，等价于 val = std::nullopt
    val.emplace(100);                  // 原地构造，避免临时对象

    // 作为函数返回值：if (opt) 利用向 bool 的显式类型转换
    std::vector<int> a = {1, 3, 5, 7};
    if (auto r = first_ge(a, 6)) cout << "first >= 6: " << *r << endl; // 7

    if (auto inv = mod_inverse(3, 10)) cout << "3^-1 mod 10 = " << *inv << endl; // 7
    if (auto inv = mod_inverse(2, 4)) cout << *inv << endl;   // 不执行
    else cout << "2^-1 mod 4 不存在" << endl;

    return 0;
}
