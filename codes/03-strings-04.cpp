#include <bits/stdc++.h>
using namespace std;

// P3804 【模板】后缀自动机 (SAM)
// 求所有出现次数 > 1 的子串中，出现次数 × 子串长度的最大值。
// 每个状态的出现次数即其 endpos 集合大小：
//   extend 新建的状态恰代表一个前缀，初始 cnt = 1；clone 状态不代表前缀，cnt = 0；
//   按 len 从大到小（即 parent 树从叶到根的拓扑序）沿 link 累加即可。

struct SAM {
    struct State {
        int len = 0, link = -1;
        array<int, 26> nxt = {};
    };

    int sz = 1, last = 0;
    vector<State> st = {State{}};
    vector<long long> cnt = {0};    // endpos 大小（该状态所代表子串的出现次数）

    int clone(int p, int q, int c) {
        int cur = sz++;
        st.emplace_back();
        cnt.push_back(0);           // clone 状态不代表任何前缀

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
        cnt.push_back(1);           // 新状态代表一个前缀

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

    // 桶排序按 len 降序处理，把 cnt 沿 link 上传，无需显式建 parent 树
    void buildCnt() {
        vector<int> bucket(sz, 0), order(sz);
        for (int i = 0; i < sz; i++) {
            bucket[st[i].len]++;
        }
        for (int i = 1; i < sz; i++) {
            bucket[i] += bucket[i - 1];
        }
        for (int i = 0; i < sz; i++) {
            order[--bucket[st[i].len]] = i;
        }

        for (int i = sz - 1; i >= 1; i--) {
            int v = order[i];
            cnt[st[v].link] += cnt[v];
        }
    }
} sam;

int main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    string s;
    cin >> s;

    for (auto c : s) {
        sam.extend(c - 'a');
    }
    sam.buildCnt();

    // 同一状态的所有子串出现次数相同，取最长子串 len 即可最大化乘积
    long long ans = 0;
    for (int i = 1; i < sam.sz; i++) {
        if (sam.cnt[i] > 1) {
            ans = max(ans, sam.cnt[i] * sam.st[i].len);
        }
    }
    cout << ans << '\n';

    return 0;
}
