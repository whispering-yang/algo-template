#include <bits/stdc++.h>
using namespace std;

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

    int n;
    cin >> n;
    string s;
    cin >> s;

    for (auto c : s) {
        sam.extend(c - 'a');
    }

    int64_t ans = 0LL;
    for (int i = 1; i < sam.sz; i++) {
        ans += sam.st[i].len - sam.st[sam.st[i].link].len;
    }
    cout << ans;

    return 0;
}