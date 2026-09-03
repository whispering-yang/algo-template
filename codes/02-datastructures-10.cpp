mt19937 rnd(random_device{}());

struct Node {
    uint32_t pri;
    int val, lc, rc, sz;
    Node() : pri(rnd()), val(0), lc(0), rc(0), sz(0) {}
    Node(int val) : pri(rnd()), val(val), lc(0), rc(0), sz(1) {}
    Node(const Node &rhs) : pri(rhs.pri), val(rhs.val),
        lc(rhs.lc), rc(rhs.rc), sz(rhs.sz) {}
};

constexpr int MAXN = 2e5 + 15;
array<int, MAXN> rts;               // 各版本的根节点
array<Node, 5 * MAXN * __lg(MAXN)> tree; // 预留足够空间
int node_cnt = 0, rt_cnt = 0;

int new_node(int val) {
    tree[++node_cnt] = Node(val);
    return node_cnt;
}

int new_node(const Node &rhs) {
    tree[++node_cnt] = Node(rhs);
    return node_cnt;
}

void split(int val, int u, int &x, int &y) {
    if (u == 0) { x = 0, y = 0; return; }
    u = new_node(tree[u]);          // 拷贝路径上的节点
    if (tree[u].val <= val) {
        split(val, tree[u].rc, tree[u].rc, y);
        x = u;
    } else {
        split(val, tree[u].lc, x, tree[u].lc);
        y = u;
    }
    tree[u].sz = tree[tree[u].lc].sz + tree[tree[u].rc].sz + 1;
}

int merge(int u, int v) {
    if (u == 0 || v == 0) return u | v;
    if (tree[u].pri > tree[v].pri) {
        u = new_node(tree[u]);
        tree[u].rc = merge(tree[u].rc, v);
        tree[u].sz = tree[tree[u].lc].sz + tree[tree[u].rc].sz + 1;
        return u;
    } else {
        v = new_node(tree[v]);
        tree[v].lc = merge(u, tree[v].lc);
        tree[v].sz = tree[tree[v].lc].sz + tree[tree[v].rc].sz + 1;
        return v;
    }
}

int insert(int val, int u) {
    int x, y;
    split(val, u, x, y);
    return merge(merge(x, new_node(val)), y);
}

int erase(int val, int u) {
    int x, y, z;
    split(val, u, x, z);
    split(val - 1, x, x, y);
    return merge(merge(x, merge(tree[y].lc, tree[y].rc)), z);
}

int get_rnk(int val, int u) {
    int rnk = 0;
    while (u) {
        if (tree[u].val < val) {
            rnk += tree[tree[u].lc].sz + 1;
            u = tree[u].rc;
        } else {
            u = tree[u].lc;
        }
    }
    return rnk + 1;
}

int find_kth(int k, int u) {
    while (u) {
        int lsz = tree[tree[u].lc].sz;
        if (k <= lsz) u = tree[u].lc;
        else if (k == lsz + 1) return tree[u].val;
        else k = k - lsz - 1, u = tree[u].rc;
    }
    return 0; // not found
}

int get_pre(int val, int u) {
    int ans = -INT_MAX;
    while (u) {
        if (tree[u].val < val) ans = tree[u].val, u = tree[u].rc;
        else u = tree[u].lc;
    }
    return ans;
}

int get_nxt(int val, int u) {
    int ans = INT_MAX;
    while (u) {
        if (tree[u].val <= val) u = tree[u].rc;
        else ans = tree[u].val, u = tree[u].lc;
    }
    return ans;
}
