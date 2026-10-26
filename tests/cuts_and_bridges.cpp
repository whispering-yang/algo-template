// Run from the repository root:
// g++ -std=c++17 -O2 -Wall -Wextra -pedantic tests/cuts_and_bridges.cpp -o build/cuts_and_bridges_test.exe
#include <bits/stdc++.h>
using namespace std;

namespace cuts {
#define main cuts_example_main
#include "../codes/06-graph-03.cpp"
#undef main
}

namespace ebcc {
#define main ebcc_example_main
#include "../codes/06-graph-05.cpp"
#undef main
}

using Edge = pair<int, int>;
int cases = 0;

void require(bool ok, const string &message) {
    if (!ok) throw runtime_error(message + " at case " + to_string(cases));
}

// Independent oracle: rebuild connectivity after deleting vertices or edges.
vector<int> connectivity(int n, const vector<Edge> &edges, int removed_vertex,
                         const vector<bool> &removed_edges) {
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < (int)edges.size(); ++i) {
        auto [u, v] = edges[i];
        if (removed_edges[i] || u == removed_vertex || v == removed_vertex) continue;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<int> id(n + 1, -1);
    int count = 0;
    for (int start = 1; start <= n; ++start) {
        if (start == removed_vertex || id[start] != -1) continue;
        queue<int> pending;
        pending.push(start);
        id[start] = count++;
        while (!pending.empty()) {
            int u = pending.front();
            pending.pop();
            for (int v : adj[u]) {
                if (id[v] != -1) continue;
                id[v] = id[u];
                pending.push(v);
            }
        }
    }
    return id;
}

int component_count(const vector<int> &id) {
    return *max_element(id.begin(), id.end()) + 1;
}

Edge normalized(Edge e) {
    if (e.first > e.second) swap(e.first, e.second);
    return e;
}

void check(int n, const vector<Edge> &edges) {
    ++cases;
    vector<bool> removed(edges.size(), false);
    int original_count = component_count(connectivity(n, edges, 0, removed));
    vector<bool> expected_cut(n + 1, false), expected_bridge(edges.size(), false);
    for (int u = 1; u <= n; ++u) {
        expected_cut[u] = component_count(connectivity(n, edges, u, removed)) > original_count;
    }
    vector<Edge> expected_bridges;
    for (int i = 0; i < (int)edges.size(); ++i) {
        removed[i] = true;
        expected_bridge[i] = component_count(connectivity(n, edges, 0, removed)) > original_count;
        removed[i] = false;
        if (expected_bridge[i]) expected_bridges.push_back(normalized(edges[i]));
    }
    sort(expected_bridges.begin(), expected_bridges.end());
    vector<int> expected_component = connectivity(n, edges, 0, expected_bridge);

    cuts::n = ebcc::n = n;
    cuts::m = ebcc::m = (int)edges.size();
    cuts::adj.assign(n + 1, {});
    for (auto [u, v] : edges) {
        cuts::adj[u].push_back(v);
        cuts::adj[v].push_back(u);
    }
    // Vary adjacency order independently of input edge order.
    for (auto &neighbors : cuts::adj) reverse(neighbors.begin(), neighbors.end());
    ebcc::adj = cuts::adj;

    // Same graph twice checks clearing of flags, results and DFS/stack state.
    for (int run = 0; run < 2; ++run) {
        cuts::find_cuts_and_bridges();
        require(cuts::is_cut == expected_cut, "cut vertices");
        vector<Edge> actual_bridges;
        for (auto e : cuts::bridges) actual_bridges.push_back(normalized(e));
        sort(actual_bridges.begin(), actual_bridges.end());
        require(actual_bridges == expected_bridges, "bridges");

        ebcc::find_edge_bcc();
        require(ebcc::stk.empty(), "unconsumed EBCC stack");
        require((int)ebcc::components.size() == component_count(expected_component), "EBCC count");
        vector<int> occurrences(n + 1, 0);
        for (int id = 0; id < (int)ebcc::components.size(); ++id) {
            require(!ebcc::components[id].empty(), "empty EBCC");
            for (int u : ebcc::components[id]) {
                require(1 <= u && u <= n, "EBCC vertex bounds");
                ++occurrences[u];
                require(ebcc::component_id[u] == id, "component_id mapping");
            }
        }
        for (int u = 1; u <= n; ++u) {
            require(occurrences[u] == 1, "EBCC vertex partition");
            for (int v = 1; v <= n; ++v) {
                require((ebcc::component_id[u] == ebcc::component_id[v]) ==
                        (expected_component[u] == expected_component[v]), "EBCC connectivity");
            }
        }
    }
}

int main() {
    check(1, {});
    check(1, {{1, 1}, {1, 1}});
    check(2, {{1, 2}});
    check(2, {{1, 2}, {1, 2}});
    check(5, {{1, 2}, {2, 3}, {3, 4}, {4, 5}});
    check(5, {{1, 2}, {1, 3}, {1, 4}, {1, 5}});
    check(6, {{1, 2}, {2, 3}, {3, 4}, {4, 2}, {5, 5}});
    check(5, {{1, 2}, {2, 3}, {3, 1}, {1, 4}, {4, 5}, {5, 1}});
    check(6, {{1, 2}, {1, 2}, {2, 3}, {3, 3}, {4, 5}, {4, 5}});

    // Exhaust every simple graph through five vertices.
    for (int n = 1; n <= 5; ++n) {
        vector<Edge> possible;
        for (int u = 1; u <= n; ++u)
            for (int v = u + 1; v <= n; ++v) possible.emplace_back(u, v);
        for (int mask = 0; mask < (1 << possible.size()); ++mask) {
            vector<Edge> edges;
            for (int i = 0; i < (int)possible.size(); ++i)
                if ((mask >> i) & 1) edges.push_back(possible[i]);
            check(n, edges);
        }
    }
    mt19937 rng(20261026);
    for (int t = 0; t < 3000; ++t) {
        int n = 1 + rng() % 9, m = rng() % 25;
        vector<Edge> edges;
        for (int i = 0; i < m; ++i)
            edges.emplace_back(1 + rng() % n, 1 + rng() % n);
        shuffle(edges.begin(), edges.end(), rng);
        check(n, edges);
    }
    cout << "PASS: " << cases << " graphs, cuts/bridges/EBCC + repeated calls (seed 20261026)\n";
}
