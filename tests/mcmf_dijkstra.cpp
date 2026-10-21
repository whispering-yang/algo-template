// g++ -std=c++17 -O2 -Wall -Wextra -pedantic tests/mcmf_dijkstra.cpp -o build/mcmf_dijkstra_test.exe
#define main mcmf_template_main
#include "../codes/07-networkflow-07.cpp"
#undef main

struct InputEdge {
    int u, v;
    int64_t cap, cost;
};
int cases = 0;

void require(bool ok, const string &message) {
    if (!ok) throw runtime_error(message);
}

// Independent oracle: enumerate every assignment of integral edge flows.
pair<int64_t, int64_t> oracle(int n, int s, int t,
                            const vector<InputEdge> &edges, int64_t limit) {
    vector<int64_t> net(n + 1);
    pair<int64_t, int64_t> best = {0, 0};
    function<void(int, int64_t)> dfs = [&](int i, int64_t cost) {
        if (i == int(edges.size())) {
            for (int v = 1; v <= n; v++)
                if (v != s && v != t && net[v] != 0) return;
            int64_t flow = net[s];
            if (flow < 0 || flow > limit || net[t] != -flow) return;
            if (flow > best.first || (flow == best.first && cost < best.second))
                best = {flow, cost};
            return;
        }
        auto [u, v, cap, w] = edges[i];
        for (int64_t f = 0; f <= cap; f++) {
            net[u] += f;
            net[v] -= f;
            dfs(i + 1, cost + f * w);
            net[u] -= f;
            net[v] += f;
        }
    };
    dfs(0, 0);
    return best;
}

void check(int n, int s, int t, const vector<InputEdge> &edges) {
    cases++;
    for (int64_t limit : {0LL, 1LL, 3LL, 100LL}) {
        MCMFDijkstra mc(n);
        for (auto [u, v, cap, w] : edges) mc.add_edge(u, v, cap, w);
        auto expected = oracle(n, s, t, edges, limit);
        auto actual = mc.mcmf(s, t, limit);
        require(actual == expected, "oracle mismatch at case " + to_string(cases));
        vector<int64_t> net(n + 1);
        int64_t cost = 0;
        for (int i = 0; i < int(edges.size()); i++) {
            const auto &e = edges[i];
            int64_t f = e.cap - mc.edg[2 * i].cap;
            require(0 <= f && f <= e.cap, "capacity constraint");
            require(mc.edg[2 * i + 1].cap == f, "reverse capacity");
            net[e.u] += f;
            net[e.v] -= f;
            cost += f * e.cost;
        }
        for (int v = 1; v <= n; v++)
            require(net[v] == (v == s ? actual.first : v == t ? -actual.first : 0),
                    "flow conservation");
        require(cost == actual.second, "reported cost");

        // Resume on the same residual graph, including active negative reverse edges.
        auto extra = mc.mcmf(s, t);
        auto full = oracle(n, s, t, edges, 100);
        require(actual.first + extra.first == full.first &&
                actual.second + extra.second == full.second, "resumed call");
    }
}

int main() {
    check(2, 1, 2, {});
    check(3, 1, 3, {{1, 2, 0, -9}, {2, 3, 2, 1}});
    check(4, 1, 4, {{1, 2, 2, -4}, {2, 4, 2, 3}, {3, 4, 1, -10}});
    check(3, 1, 3, {{1, 2, 2, 4}, {1, 2, 1, 0}, {2, 1, 1, 3},
                     {2, 3, 2, 1}, {2, 2, 1, 0}});
    // Second augmentation must undo the first a->x assignment through a reverse edge.
    check(6, 1, 6, {{1, 2, 1, 0}, {1, 3, 1, 0}, {2, 4, 1, 0},
                     {2, 5, 1, 2}, {3, 4, 1, 1}, {3, 5, 1, 10},
                     {4, 6, 1, 0}, {5, 6, 1, 0}});
    // Arbitrary source/sink; a positive-cost self-loop must never enter pre[].
    check(4, 3, 2, {{3, 1, 2, -2}, {1, 2, 2, 4}, {1, 1, 1, 5}});

    mt19937 rng(20261021);
    for (int mode = 0; mode < 3; mode++) {
        for (int trial = 0; trial < 500; trial++) {
            int n = 2 + rng() % 4, m = rng() % 10;
            vector<int64_t> potential(n + 1);
            for (int v = 1; v <= n; v++) potential[v] = int(rng() % 13) - 6;
            vector<InputEdge> edges;
            for (int i = 0; i < m; i++) {
                int u = 1 + rng() % n, v = 1 + rng() % n;
                int64_t w = rng() % 8;
                if (mode == 1) {  // Signed DAG.
                    if (u == v) continue;
                    if (u > v) swap(u, v);
                    w = int(rng() % 15) - 7;
                }
                if (mode == 2) w += potential[v] - potential[u];
                // Mode 2 has signed cyclic edges but nonnegative cycle costs.
                edges.push_back({u, v, int64_t(rng() % 3), w});
            }
            check(n, 1, n, edges);
        }
    }
    MCMFDijkstra large(3);
    large.add_edge(1, 2, 3'000'000'000LL, -2);
    large.add_edge(2, 3, 3'000'000'000LL, 5);
    require(large.mcmf(1, 3) == make_pair(3'000'000'000LL, 9'000'000'000LL),
            "64-bit flow/cost");
    cout << "OK: " << cases << " exhaustive-oracle graphs x 4 limits, resumed calls, "
         << "capacity/conservation checks, and 64-bit case\n";
}
