template <typename T>
struct SparseTable {
    static constexpr int M = 26;
    T merge(T a, T b) {
        return min<T>(a, b);
    }

    int n;
    std::vector<std::array<T, M>> st;

    SparseTable(const auto &list) : n(list.size()), st(n) {
        for (int i = 0; i < n; i++) {
            st[i][0] = list[i];
        }

        for (int j = 1; j < M; j++) {
            for (int i = 0; i + (1 << j) - 1 < n; i++) {
                st[i][j] = merge(st[i][j - 1], st[i + (1 << j - 1)][j - 1]);
            }
        }
    }

    T query(int l, int r) {
        int t = std::__lg(r - l + 1);
        return merge(st[l][t], st[r - (1 << t) + 1][t]);
    }
};
