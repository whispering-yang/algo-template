struct Fenwick {
    int N;
    vector<int> tree;

    int lowbit(int x) {
        return x & -x;
    }

    void build() {
        N = tree.size() - 1;
        for (int i = 1; i <= N; i++) {
            int j = i + lowbit(i);
            if (j <= N) tree[j] += tree[i];
        }
    }

    void modify(int x, int d) {
        while (x <= N) {
            tree[x] += d;
            x += lowbit(x);
        }
    }

    int query(int x) {
        int res = 0;
        while (x > 0) {
            res += tree[x];
            x -= lowbit(x);
        }
        return res;
    }

    // 倍增找第 k 小
    // 返回 N + 1，说明 k 过大，未找到
    int findk(int k) {
        int cur = 1 << (__lg(N) + 1), tot = 0, idx = 0;
        while (cur > 0) {
            if (idx + cur <= N && tot + tree[idx + cur] < k) {
                idx += cur;
                tot += tree[idx];
            }
            cur >>= 1;
        }
        return idx + 1;
    }
};
