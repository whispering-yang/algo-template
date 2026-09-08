#include <bits/stdc++.h>
using namespace std;

// SP1811 LCS —— 两个字符串的最长公共子串
// 对 S 建 SAM，令 T 在自动机上匹配：维护当前状态 v 与匹配长度 l，
// 失配时沿 link 跳到更短后缀的等价类（endpos 更大，更可能存在转移），
// 匹配过程中 l 的最大值即为答案。

struct SAM {
    struct State {
        int len = 0, link = -1;
        array<int, 26> nxt = {};
    };

    int sz = 1, last = 0;
    vector<State> st = {State{}};

    int clone(int p, int q, int c) {
        int cur = sz++;
        st.emplace_back();

        st[cur].len = st[p].len + 1;
        st[cur].link = st[q].link;
        st[cur].nxt = st[q].nxt;

        st[q].link = cur;

        for (; p != -1 && st[p].nxt[c] == q; p = st[p].link) {
            st[p].nxt[c] = cur;
        }

        return cur;
    }

    void extend(int c) {
        int cur = sz++;
        st.emplace_back();
        st[cur].len = st[last].len + 1;

        int p = last;
        for (; p != -1 && st[p].nxt[c] == 0; p = st[p].link) {
            st[p].nxt[c] = cur;
        }

        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].nxt[c];
            if (st[q].len == st[p].len + 1) {
                st[cur].link = q;
            } else {
                st[cur].link = clone(p, q, c);
            }
        }

        last = cur;
    }
} sam;

int main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    string s, t;
    cin >> s >> t;

    for (auto c : s) {
        sam.extend(c - 'a');
    }

    int v = 0, l = 0, best = 0;
    for (auto ch : t) {
        int c = ch - 'a';
        while (v != 0 && sam.st[v].nxt[c] == 0) {
            v = sam.st[v].link;
            l = sam.st[v].len;
        }
        if (sam.st[v].nxt[c] != 0) {
            v = sam.st[v].nxt[c];
            l++;
        }
        // 根也无 c 的转移（S 不含字符 c）时 v = 0、l = 0，重新开始匹配
        best = max(best, l);
    }
    cout << best << '\n';

    return 0;
}
