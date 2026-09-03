#include <bits/stdc++.h>
using namespace std;

int n, node_count = 1;                       // 根节点 0 已存在
vector<array<int, 26>> trie(1, {0});        // AC 自动机的 trie 图
vector<int> fail;                           // fail 指针
vector<vector<int>> fail_children;          // fail 树的邻接表（父 -> 子）

// 向 trie 中插入一个模式串，返回终止节点编号
int insert(const string &str) {
    int u = 0;
    for (char ch : str) {
        int c = ch - 'a';
        if (trie[u][c] == 0) {
            trie[u][c] = node_count++;
            trie.push_back({0});
        }
        u = trie[u][c];
    }
    return u;
}

// 构建 fail 指针，并将 trie 补全为 trie 图
void build_fail() {
    fail.assign(node_count, 0);
    queue<int> q;

    // 第一层节点入队
    for (int c = 0; c < 26; ++c) {
        if (trie[0][c]) q.push(trie[0][c]);
    }

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int c = 0; c < 26; ++c) {
            if (trie[u][c]) {
                // 子节点存在：设置 fail 并入队
                fail[trie[u][c]] = trie[fail[u]][c];
                q.push(trie[u][c]);
            } else {
                // 子节点不存在：直接跳转，形成 trie 图
                trie[u][c] = trie[fail[u]][c];
            }
        }
    }

    // 建立 fail 树（用于统计出现次数）
    fail_children.assign(node_count, {});
    for (int i = 1; i < node_count; ++i) {
        fail_children[fail[i]].push_back(i);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    vector<int> queries(n);               // 保存每个模式串的终止节点
    for (int i = 0; i < n; ++i) {
        string str;
        cin >> str;
        queries[i] = insert(str);
    }

    build_fail();

    string text;
    cin >> text;

    // 在 trie 图上遍历文本串，统计每个节点被经过的次数
    int u = 0;
    vector<long long> freq(node_count, 0);
    for (char ch : text) {
        int c = ch - 'a';
        u = trie[u][c];
        freq[u]++;
    }

    // 在 fail 树上做一次自底向上的累加，
    // 使得每个节点最终保存该节点代表的前缀在文本中的总出现次数
    function<void(int)> dfs = [&](int u) {
        for (int v : fail_children[u]) {
            dfs(v);
            freq[u] += freq[v];
        }
    };
    dfs(0);

    // 输出每个模式串的出现次数
    for (int i = 0; i < n; ++i) {
        cout << freq[queries[i]] << '\n';
    }

    return 0;
}
