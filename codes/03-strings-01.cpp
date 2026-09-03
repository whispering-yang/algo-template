#include <bits/stdc++.h>
using namespace std;

// 计算前缀函数 π
// π[i]：以 i 结尾的最长真前缀后缀的长度
vector<int> buildPi(const string &s) {
    int n = s.size();
    vector<int> pi(n);
    for (int i = 1; i < n; ++i) {
        int len = pi[i - 1];
        while (len > 0 && s[i] != s[len]) {
            len = pi[len - 1];
        }
        if (s[i] == s[len]) {
            pi[i] = len + 1;
        }
    }
    return pi;
}

// KMP 匹配：构造 combined = pattern + '#' + text，
// 对其计算前缀函数，当 π[i] == |pattern| 时匹配成功
// 返回所有匹配的起始位置（0-based）
vector<int> kmp(const string &text, const string &pattern) {
    string combined = pattern + '#' + text;
    int m = pattern.size();
    vector<int> pi = buildPi(combined);
    vector<int> matches;
    for (int i = m + 1; i < (int)combined.size(); ++i) {
        if (pi[i] == m) {
            matches.push_back(i - 2 * m);
        }
    }
    return matches;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string text, pattern;
    cin >> text >> pattern;

    vector<int> result = kmp(text, pattern);
    for (int pos : result) {
        cout << pos + 1 << '\n'; // 输出 1-based 位置
    }

    return 0;
}
