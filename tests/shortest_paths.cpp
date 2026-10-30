// Run from the repository root:
// g++ -std=c++17 -O2 -Wall -Wextra tests/shortest_paths.cpp -o build/shortest_paths.exe
#include <bits/stdc++.h>
using namespace std;

namespace sp {
#include "../codes/06-graph-18.cpp"
}
namespace bf {
#include "../codes/06-graph-19.cpp"
}
namespace nc {
#include "../codes/06-graph-20.cpp"
}
namespace dc {
#include "../codes/06-graph-21.cpp"
}
namespace zb {
#include "../codes/06-graph-22.cpp"
}
namespace fw {
#include "../codes/06-graph-11.cpp"
}
namespace hd {
#include "../codes/06-graph-10.cpp"
}
namespace nd {
#include "../codes/06-graph-23.cpp"
}

using Edge = tuple<int, int, long long>;
constexpr long long LIM = 1LL << 55;
int weighted_cases = 0, binary_cases = 0, nonnegative_cases = 0;

void require(bool ok, const string &message) {
    if (!ok) throw runtime_error(message);
}

// Independent all-pairs oracle, including reachability and negative cycles.
vector<vector<long long>> oracle(int n, const vector<Edge> &edges) {
    vector<vector<long long>> d(n + 1, vector<long long>(n + 1, LIM));
    for (int u = 1; u <= n; u++) d[u][u] = 0;
    for (auto [u, v, w] : edges) d[u][v] = min(d[u][v], w);
    for (int k = 1; k <= n; k++)
        for (int u = 1; u <= n; u++)
            for (int v = 1; v <= n; v++)
                if (d[u][k] != LIM && d[k][v] != LIM)
                    d[u][v] = min(d[u][v], d[u][k] + d[k][v]);
    return d;
}

void check_weighted(int n, const vector<Edge> &edges) {
    weighted_cases++;
    auto d = oracle(n, edges);
    sp::adj.assign(n + 1, {});
    nc::adj.assign(n + 1, {});
    dc::adj.assign(n + 1, {});
    sp::dist.resize(n + 1);
    bf::dist.resize(n + 1);
    bf::edges.clear();
    for (auto [u, v, w] : edges) {
        sp::adj[u].emplace_back(v, w);
        nc::adj[u].emplace_back(v, w);
        dc::add_constraint(u, v, w);
        bf::edges.push_back({u, v, w});
    }
    bool negative = false;
    for (int u = 1; u <= n; u++) negative |= d[u][u] < 0;
    require(nc::has_negative_cycle() == negative, "global negative cycle");
    require(dc::difference_constraints() == !negative, "constraint feasibility");
    if (!negative)
        for (auto [u, v, w] : edges)
            require(dc::x[v] - dc::x[u] <= w, "constraint witness");

    // Multiple calls also check that distances and queue state reset correctly.
    for (int s = 1; s <= n; s++) {
        bool reachable_negative = false;
        for (int u = 1; u <= n; u++)
            reachable_negative |= d[s][u] != LIM && d[u][u] < 0;
        require(sp::spfa(s) == !reachable_negative, "SPFA negative cycle");
        require(bf::bellman_ford(s) == !reachable_negative, "Bellman-Ford negative cycle");
        if (!reachable_negative)
            for (int v = 1; v <= n; v++) {
                long long expected = d[s][v] == LIM ? sp::INF : d[s][v];
                require(sp::dist[v] == expected, "SPFA distance");
                require(bf::dist[v] == expected, "Bellman-Ford distance");
            }
    }
    if (!negative) {
        fw::n = n;
        for (int u = 1; u <= n; u++) {
            fill(fw::d[u].begin(), fw::d[u].end(), fw::INF);
            fw::d[u][u] = 0;
        }
        for (auto [u, v, w] : edges) fw::d[u][v] = min(fw::d[u][v], w);
        fw::floyd();
        for (int u = 1; u <= n; u++)
            for (int v = 1; v <= n; v++)
                require(fw::d[u][v] == (d[u][v] == LIM ? fw::INF : d[u][v]),
                        "Floyd distance / unreachable state");
    }
}

void check_binary(int n, const vector<Edge> &edges) {
    binary_cases++;
    auto d = oracle(n, edges);
    zb::adj.assign(n + 1, {});
    zb::dist.resize(n + 1);
    for (auto [u, v, w] : edges) zb::adj[u].emplace_back(v, (int)w);
    for (int s = 1; s <= n; s++) {
        zb::bfs01(s);
        for (int v = 1; v <= n; v++)
            require(zb::dist[v] == (d[s][v] == LIM ? zb::INF : d[s][v]),
                    "0-1 BFS distance");
    }
}

void check_nonnegative(int n, const vector<Edge> &edges) {
    nonnegative_cases++;
    auto d = oracle(n, edges);
    hd::adj.assign(n + 1, {});
    nd::adj.assign(n + 1, {});
    hd::dist.resize(n + 1);
    nd::dist.resize(n + 1);
    nd::done.resize(n + 1);
    for (auto [u, v, w] : edges) {
        hd::adj[u].emplace_back(v, w);
        nd::adj[u].emplace_back(v, w);
    }
    // Multiple calls also check that distances, done marks and heap state reset correctly.
    for (int s = 1; s <= n; s++) {
        hd::dijkstra(s);
        nd::dijkstra(s);
        for (int v = 1; v <= n; v++) {
            long long expected = d[s][v] == LIM ? hd::INF : d[s][v];
            require(hd::dist[v] == expected, "heap Dijkstra distance");
            require(nd::dist[v] == expected, "naive Dijkstra distance");
        }
    }
}

int main() {
    check_weighted(1, {});
    check_weighted(1, {{1, 1, -1}});
    check_weighted(1, {{1, 1, 0}});
    check_weighted(4, {{1, 2, 3}, {3, 4, -2}, {4, 3, 1}});  // unreachable cycle
    check_weighted(4, {{1, 2, -5}});  // INF + negative must remain unreachable
    check_weighted(4, {{1, 2, 0}, {2, 3, -2}, {3, 2, 2}, {3, 4, 5}});
    check_weighted(4, {{1, 2, 2000000000000LL}, {2, 3, -1000000000000LL},
                       {3, 4, 2000000000000LL}});  // 64-bit distances
    vector<Edge> parallel;
    for (int w = 20; w >= -20; w--) parallel.emplace_back(1, 2, w);
    check_weighted(2, parallel);  // many relaxations do not imply a negative cycle
    check_binary(4, {{1, 2, 1}, {1, 3, 0}, {3, 2, 0}, {2, 4, 0}, {4, 3, 0}});
    check_binary(1, {});
    check_nonnegative(1, {});
    check_nonnegative(2, {{1, 2, 0}});
    check_nonnegative(4, {{1, 1, 5}, {1, 2, 3}, {1, 2, 4}, {2, 4, 2000000000000LL}, {3, 4, 7}});  // self loop, parallel edges, 64-bit distance
    check_nonnegative(3, {{2, 3, 0}, {3, 2, 0}});  // unreachable zero cycle

    mt19937 rng(20261014);
    for (int t = 0; t < 2000; t++) {
        int n = 1 + rng() % 9, m = rng() % (2 * n * n + 1);
        vector<Edge> edges;
        for (int i = 0; i < m; i++) {
            int u = 1 + rng() % n, v = 1 + rng() % n;
            edges.emplace_back(u, v, (int)(rng() % 21) - 10);
        }
        check_weighted(n, edges);
    }
    // Potential-generated graphs guarantee no negative cycles despite negative edges.
    for (int t = 0; t < 500; t++) {
        int n = 1 + rng() % 9, m = rng() % (2 * n * n + 1);
        vector<int> h(n + 1);
        for (int u = 1; u <= n; u++) h[u] = (int)(rng() % 41) - 20;
        vector<Edge> edges;
        for (int i = 0; i < m; i++) {
            int u = 1 + rng() % n, v = 1 + rng() % n;
            edges.emplace_back(u, v, (int)(rng() % 11) + h[v] - h[u]);
        }
        check_weighted(n, edges);
    }
    for (int t = 0; t < 1000; t++) {
        int n = 1 + rng() % 12, m = rng() % (2 * n * n + 1);
        vector<Edge> edges;
        for (int i = 0; i < m; i++) {
            int u = 1 + rng() % n, v = 1 + rng() % n;
            edges.emplace_back(u, v, rng() % 2);
        }
        check_binary(n, edges);
    }
    for (int t = 0; t < 2000; t++) {
        int n = 1 + rng() % 9, m = rng() % (2 * n * n + 1);
        vector<Edge> edges;
        for (int i = 0; i < m; i++) {
            int u = 1 + rng() % n, v = 1 + rng() % n;
            edges.emplace_back(u, v, rng() % 21);  // weights in [0, 20]
        }
        check_nonnegative(n, edges);
    }
    cout << "PASS: " << weighted_cases << " weighted graphs, " << binary_cases
         << " binary graphs, " << nonnegative_cases
         << " nonnegative graphs (all sources, seed 20261014)\n";
}
