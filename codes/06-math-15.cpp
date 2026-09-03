// =============================================================================
// File:     06-math-15  — Classic Impartial Games (经典组合博弈结论判定)
// =============================================================================

#include <bits/stdc++.h>
using namespace std;

// ---------- 巴什博弈 (Bash Game) ----------
// 一堆 n 颗石子, 双方轮流取 1..m 颗, 取走最后一颗者胜
// 先手必胜 <=> n % (m + 1) != 0
bool bash_first_win(int64_t n, int64_t m) {
    return n % (m + 1) != 0;
}

// ---------- 威佐夫博弈 (Wythoff Game) ----------
// 两堆 a, b 颗石子, 每次从某一堆取任意颗, 或从两堆取相同颗数, 取走最后一颗者胜
// 先手必败 <=> (a, b) 为奇异局势: min(a, b) == floor(|a - b| * phi), phi = (sqrt(5)+1)/2
// 注: 浮点实现适用于 0 <= a, b < 1e15; 范围更大时改用整数平方根精确判定
bool wythoff_first_lose(int64_t a, int64_t b) {
    if (a > b) swap(a, b);
    int64_t d = b - a;
    return a == (int64_t)(d * (sqrtl(5.0L) + 1.0L) / 2.0L);
}

// ---------- 阶梯博弈 (Staircase Nim) ----------
// 台阶 0..n-1, 台阶 i 上有 a[i] 颗石子 (0 号为地面)
// 每次选台阶 i >= 1, 将任意颗石子移到 i - 1; 移到地面即消失; 无棋可走者败
// 结论: 等价于只由奇数号台阶组成的 Nim —— 先手必败 <=> a[1]^a[3]^a[5]^... == 0
bool staircase_first_lose(const vector<int64_t> &a) {
    int64_t x = 0;
    for (size_t i = 1; i < a.size(); i += 2) x ^= a[i];
    return x == 0;
}

// ---------- Anti-Nim (反 Nim) ----------
// n 堆石子, 每次从一堆取至少一颗, 【取走最后一颗者输】
// 先手必胜 <=> (所有堆都恰好 1 颗 且 异或和 == 0)
//           或 (存在 > 1 颗的堆 且 异或和 != 0)
bool anti_nim_first_win(const vector<int64_t> &a) {
    bool all_one = all_of(a.begin(), a.end(), [](int64_t v) { return v == 1; });
    int64_t x = 0;
    for (int64_t v : a) x ^= v;
    if (all_one) return x == 0;
    return x != 0;
}

// -------- 使用示例 (main) --------
// 多组询问: 每组输入 n 与 n 堆石子数, 输出 Anti-Nim 先手是否必胜
// int main() {
//     ios::sync_with_stdio(false);
//     cin.tie(nullptr);
//     int n;
//     while (cin >> n) {
//         vector<int64_t> a(n);
//         for (auto &v : a) cin >> v;
//         cout << (anti_nim_first_win(a) ? "Win" : "Lose") << '\n';
//         // 其余判定: bash_first_win(n, m) / wythoff_first_lose(a, b) / staircase_first_lose(a)
//     }
//     return 0;
// }
