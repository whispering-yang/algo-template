#include <bits/stdc++.h>
using namespace std;

// P6139 【模板】广义后缀自动机 (GSAM)
// 给定 n 个字符串，输出所有串本质不同的子串总数与 GSAM 的状态数。
// 建法（离线法）：所有串先插入同一棵 Trie，再按 BFS 序把每条 Trie 边插入 SAM。
// insert 与单串 extend 的唯一区别：先检查转移 (p, c) 是否已存在——
//   B. 已存在且 len[q] == len[p] + 1：直接复用 q；
//   C. 已存在但不连续：clone 分裂 q，返回 clone（不是 q）；
//   A. 不存在：与在线 extend 完全相同。
// 出现次数：Trie 合并了公共前缀，不能给每个新状态 cnt = 1，
// 要用 Trie 节点权重 tw[u]（经过节点 u 的字符串条数）挂到对应状态上。

struct Trie {
    struct Node {
        array<int, 26> ch = {};
    };

    vector<Node> t = {Node{}};
    vector<long long> tw = {0};     // 先作结尾标记，累加后为「经过该节点的串数」

    void insert(const string &s) {
        int p = 0;
        for (auto c : s) {
            int x = c - 'a';
            if (t[p].ch[x] == 0) {
                t[p].ch[x] = (int)t.size();
                t.emplace_back();
                tw.push_back(0);
            }
            p = t[p].ch[x];
        }
        tw[p]++;
    }
} trie;

struct GSAM {
    struct State {
        int len = 0, link = -1;
        array<int, 26> nxt = {};
    };

    int sz = 1;
    vector<State> st = {State{}};
    vector<long long> cnt;          // endpos 大小（状态所代表子串的总出现次数）

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

    // 在状态 p 后接字符 c，返回这条 Trie 边对应的 SAM 状态
    int insert(int p, int c) {
        if (st[p].nxt[c] != 0) {                // 转移已存在：复用，不新建
            int q = st[p].nxt[c];
            if (st[q].len == st[p].len + 1) {
                return q;                       // 情况 B：连续，直接复用 q
            }
            return clone(p, q, c);              // 情况 C：分裂，返回 clone
        }

        int cur = sz++;                         // 情况 A：同在线 extend
        st.emplace_back();
        st[cur].len = st[p].len + 1;

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
        return cur;
    }

    // 所有串已插入 Trie 后调用：按 BFS 序建自动机并统计权重。
    // 必须按 BFS（层）序插入：处理节点时其父状态已确定，且状态 len
    // 沿 Trie 深度严格 +1，均摊复杂度分析才成立；DFS 序可被卡到平方级。
    void build(Trie &tr) {
        int n = (int)tr.t.size();
        vector<int> fa(n, -1), fc(n, 0), bfs;
        bfs.reserve(n);
        bfs.push_back(0);
        for (size_t h = 0; h < bfs.size(); h++) {
            int v = bfs[h];
            for (int c = 0; c < 26; c++) {
                int u = tr.t[v].ch[c];
                if (u != 0) {
                    fa[u] = v;
                    fc[u] = c;
                    bfs.push_back(u);
                }
            }
        }

        // BFS 序逆序累加：tw[u] 从结尾标记变为「经过节点 u 的字符串条数」
        for (int i = (int)bfs.size() - 1; i >= 1; i--) {
            int u = bfs[i];
            tr.tw[fa[u]] += tr.tw[u];
        }

        // 按序插入每条 Trie 边，并记录节点对应的状态
        vector<int> tstate(n, 0);
        for (size_t h = 1; h < bfs.size(); h++) {
            int u = bfs[h];
            tstate[u] = insert(tstate[fa[u]], fc[u]);
        }

        // 权重挂到状态上：parent 树求和后即为状态所代表子串的总出现次数
        cnt.assign(sz, 0);
        for (size_t h = 1; h < bfs.size(); h++) {
            int u = bfs[h];
            cnt[tstate[u]] += tr.tw[u];
        }
    }

    // parent 树按 len 降序累加（同单串 SAM）
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

    long long distinct() {
        long long d = 0;
        for (int i = 1; i < sz; i++) {
            d += st[i].len - st[st[i].link].len;
        }
        return d;
    }
} gsam;

int main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        trie.insert(s);
    }

    gsam.build(trie);
    gsam.buildCnt();

    cout << gsam.distinct() << '\n';
    cout << gsam.sz << '\n';

    return 0;
}
