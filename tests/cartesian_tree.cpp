// Run from the repository root:
// g++ -std=c++17 -O2 -Wall -Wextra -pedantic tests/cartesian_tree.cpp -o build/cartesian_tree.exe
#include "../codes/02-datastructures-16.cpp"
#include "../codes/02-datastructures-17.cpp"

int cases = 0;

void require(bool ok, const string &message) {
    if (!ok) throw runtime_error(message);
}

void check(const vector<long long> &a) {
    n = (int)a.size() - 1;
    w = a;
    build();
    calc_subtrees();
    vector<int> expected_ls(n + 1), expected_rs(n + 1), expected_fa(n + 1);
    vector<int> expected_L(n + 1), expected_R(n + 1), expected_siz(n + 1);
    // Independent oracle: choose the leftmost extreme by scanning each interval.
    function<int(int, int)> oracle = [&](int l, int r) {
        if (l > r) return 0;
        int u = l;
        for (int i = l + 1; i <= r; i++)
            if (a[i] < a[u]) u = i;
        expected_L[u] = l;
        expected_R[u] = r;
        expected_siz[u] = r - l + 1;
        expected_ls[u] = oracle(l, u - 1);
        expected_rs[u] = oracle(u + 1, r);
        if (expected_ls[u]) expected_fa[expected_ls[u]] = u;
        if (expected_rs[u]) expected_fa[expected_rs[u]] = u;
        return u;
    };
    require(root == oracle(1, n), "root");
    require(ls == expected_ls && rs == expected_rs && fa == expected_fa, "tree links");
    require(L == expected_L && R == expected_R && siz == expected_siz, "subtree ranges");

    // Every interval must agree with a direct scan, including ties.
    for (int l = 1; l <= n; l++) {
        vector<bool> ancestor(n + 1);
        for (int u = l; u; u = fa[u]) ancestor[u] = true;
        int best = l;
        for (int r = l; r <= n; r++) {
            if (a[r] < a[best]) best = r;
            int u = r;
            while (!ancestor[u]) u = fa[u];
            require(u == best, "RMQ/LCA");
        }
    }
}

void check_applications(const vector<long long> &a) {
    int n = (int)a.size() - 1;
    long long sum = 0, expected_sum = 0;
    long long area = 0, expected_area = 0;
    bool nonnegative = true;
    for (int u = 1; u <= n; u++) {
        sum += a[u] * (u - L[u] + 1) * (R[u] - u + 1);
        area = max(area, a[u] * siz[u]);
        nonnegative &= a[u] >= 0;
    }
    for (int l = 1; l <= n; l++) {
        long long minimum = a[l];
        for (int r = l; r <= n; r++) {
            minimum = min(minimum, a[r]);
            expected_sum += minimum;
            expected_area = max(expected_area, minimum * (r - l + 1));
        }
    }
    require(sum == expected_sum, "sum of subarray minima");
    if (nonnegative) require(area == expected_area, "histogram area");
}

int main() {
    auto run = [&](const vector<long long> &a, bool applications = true) {
        check(a);
        if (applications) check_applications(a);
        cases++;
    };
    // Exhaust all sequences of length <= 8 over {0, 1, 2}.
    for (int n = 0, count = 1; n <= 8; n++, count *= 3) {
        for (int mask = 0; mask < count; mask++) {
            vector<long long> a(n + 1);
            int code = mask;
            for (int i = 1; i <= n; i++, code /= 3) a[i] = code % 3;
            run(a);
        }
    }
    mt19937 rng(20261021);
    for (int test = 0; test < 1500; test++) {
        int n = rng() % 41;
        vector<long long> a(n + 1);
        for (int i = 1; i <= n; i++) a[i] = (int)(rng() % 21) - 10;
        run(a);
    }
    // No value sentinels: comparison is valid at both integer extremes.
    run({0, LLONG_MAX, LLONG_MIN, LLONG_MIN, 0, LLONG_MAX}, false);
    n = 0;
    w.clear();
    build();
    calc_subtrees();
    require(root == 0 && top == 0 && siz == vector<int>{0}, "empty tree");

    // Long chains must build and collect subtree statistics without recursion.
    n = 200000;
    vector<long long> a(n + 1);
    for (int mode = 0; mode < 3; mode++) {
        for (int i = 1; i <= n; i++)
            a[i] = mode == 0 ? i : mode == 1 ? n - i : 7;
        w = a;
        build();
        calc_subtrees();
        auto chain = [&](bool root_at_left) {
            require(root == (root_at_left ? 1 : n), "chain root");
            for (int u = 1; u <= n; u++) {
                require(ls[u] == (root_at_left ? 0 : u - 1), "chain ls");
                require(rs[u] == (root_at_left && u < n ? u + 1 : 0), "chain rs");
                int parent = root_at_left ? (u == 1 ? 0 : u - 1)
                                          : (u == n ? 0 : u + 1);
                require(fa[u] == parent, "chain fa");
                require(L[u] == (root_at_left ? u : 1), "chain L");
                require(R[u] == (root_at_left ? n : u), "chain R");
                require(siz[u] == (root_at_left ? n - u + 1 : u), "chain size");
            }
        };
        chain(mode != 1);
    }
    cout << "PASS: " << cases << " oracle cases (tree, RMQ, applications), "
         << "3 chains of 200000 nodes, repeated/empty builds\n";
}
