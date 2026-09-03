mt19937 rnd(random_device{}());

struct Node {
    int val, lc, rc, sz;
    uint32_t pri;
    Node() : val(0), lc(0), rc(0), sz(0), pri(rnd()) {}
    Node(int val) : val(val), lc(0), rc(0), sz(1), pri(rnd()) {}
};

constexpr int MAXN = 1e5 + 15;

int node_cnt = 0, root = 0;
array<Node, MAXN> tree;

int add_node(int val) {
    tree[++node_cnt] = Node(val);
    return node_cnt;
}

void push_up(int u) {
    tree[u].sz = tree[tree[u].lc].sz + tree[tree[u].rc].sz + 1;
}

void split(int u, int val, int &x, int &y) {
    if (u == 0) {
        x = 0, y = 0;
        return;
    }
    if (tree[u].val <= val) {
        x = u;
        split(tree[u].rc, val, tree[u].rc, y);
    } else {
        y = u;
        split(tree[u].lc, val, x, tree[u].lc);
    }
    push_up(u);
}

int merge(int u, int v) {
    if (u == 0 || v == 0) return u | v;
    if (tree[u].pri > tree[v].pri) {
        tree[u].rc = merge(tree[u].rc, v);
        push_up(u);
        return u;
    } else {
        tree[v].lc = merge(u, tree[v].lc);
        push_up(v);
        return v;
    }
}

void insert(int val) {
    int x, y;
    split(root, val, x, y);
    root = merge(merge(x, add_node(val)), y);
}

void erase(int val) {
    int x, y, z;
    split(root, val, x, z);
    split(x, val - 1, x, y);
    y = merge(tree[y].lc, tree[y].rc);
    root = merge(merge(x, y), z);
}

int get_rnk(int val) {
    int x, y;
    split(root, val - 1, x, y);
    int ans = tree[x].sz + 1;
    root = merge(x, y);
    return ans;
}

int find_kth(int k) {
    int u = root;
    while (u != 0) {
        if (tree[tree[u].lc].sz >= k) u = tree[u].lc;
        else if (tree[tree[u].lc].sz + 1 == k) return tree[u].val;
        else k -= tree[tree[u].lc].sz + 1, u = tree[u].rc;
    }
    return 0;
}

int get_pre(int val) {
    int x, y;
    split(root, val - 1, x, y);
    int u = x;
    while (tree[u].rc != 0) u = tree[u].rc;
    root = merge(x, y);
    return tree[u].val;
}

int get_nxt(int val) {
    int x, y;
    split(root, val, x, y);
    int u = y;
    while (tree[u].lc != 0) u = tree[u].lc;
    root = merge(x, y);
    return tree[u].val;
}
