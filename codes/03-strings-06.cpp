#include <bits/stdc++.h>
using namespace std;

// P3975 [TJOI2015] 弦论 —— 字典序第 k 小子串
// t = 0：相同子串只计一次；t = 1：相同子串按出现次数计。
// 转移边构成 DAG，sum[v] 为从状态 v 出发（含在 v 处结束）的子串总数，
// 按 len 降序（即 DAG 反拓扑序）递推：
//   sum[v][0] = 1 + sum[next]；sum[v][1] = cnt[v] + sum[next]（cnt 为 endpos 大小）。
// 两种口径同时算好，查询时直接选 sum[t]。
// 查询时按字符从小到大逐位确定：k 依次减去被跳过子树的 sum。

struct SAM {
    struct State {
        array<int64_t, 2> sum = {};
        int len = 0, link = -1, cnt = 0;
        array<int, 26> nxt = {};
    };

    int sz = 1, last = 0;
    vector<State> st = {State{}};

    int clone(int u, int v, int c) {
        int cur = sz++;
        st.emplace_back();

        st[cur].len = st[u].len + 1;
        st[cur].link = st[v].link;
        st[cur].nxt = st[v].nxt;
        st[v].link = cur;

        for (; u != -1 && st[u].nxt[c] == v; u = st[u].link) {
            st[u].nxt[c] = cur;
        }

        return cur;
    }

    void extend(int c) {
        int cur = sz++;
        st.emplace_back();

        st[cur].len = st[last].len + 1;
        st[cur].cnt = 1;

        int u = last;
        for (; u != -1 && st[u].nxt[c] == 0; u = st[u].link) {
            st[u].nxt[c] = cur;
        }

        if (u == -1) {
            st[cur].link = 0;
        } else {
            int v = st[u].nxt[c];
            if (st[v].len == st[u].len + 1) {
                st[cur].link = v;
            } else {
                st[cur].link = clone(u, v, c);
            }
        }

        last = cur;
    }

    void build_cnt_and_sum() {
        vector<int> bucket(sz), order(sz);
        for (int i = 0; i < sz; i++)
            bucket[st[i].len]++;
        for (int i = 1; i < sz; i++)
            bucket[i] += bucket[i - 1];
        for (int i = 0; i < sz; i++)
            order[--bucket[st[i].len]] = i;

        for (int i = sz - 1; i > 0; i--) {
            int u = order[i], v = st[u].link;
            st[v].cnt += st[u].cnt;
        }
        for (int i = sz - 1; i >= 0; i--) {
            int u = order[i];
            // 易错点：根状态（空串）不能有「在自身结束」的贡献
            if (u != 0) {
                st[u].sum[0] = 1;
                st[u].sum[1] = st[u].cnt;
            }
            for (auto v : st[u].nxt) {
                if (v == 0) continue;
                st[u].sum[0] += st[v].sum[0];
                st[u].sum[1] += st[v].sum[1];
            }
        }
    }

    optional<string> find_kth(int t, long long k) {
        int u = 0;
        string res = "";
        while (k > 0) {
            if (k > st[u].sum[t])
                return nullopt;
            // 易错点：根状态代表空串，不扣除贡献
            if (u != 0) {
                k -= t == 0 ? 1 : st[u].cnt;
                if (k <= 0)
                    return res;
            }
            for (int c = 0; c < 26; c++) {
                int v = st[u].nxt[c];
                if (v == 0) continue;
                if (k > st[v].sum[t]) {
                    k -= st[v].sum[t];
                } else {
                    res.push_back('a' + c);
                    u = v;
                    break;
                }
            }
        }
        return nullopt;
    }

    void show() {
        for (int i = 0; i < sz; i++) {
            cout << format("len={},link={}", st[i].len, st[i].link) << endl;
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
    // sam.show();
    sam.build_cnt_and_sum();

    int t;
    long long k;
    cin >> t >> k;
    cout << sam.find_kth(t, k).value_or("-1"s) << '\n';

    return 0;
}
