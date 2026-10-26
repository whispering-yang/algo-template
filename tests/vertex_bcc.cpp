// g++ -std=c++17 -O2 -Wall -Wextra -pedantic tests/vertex_bcc.cpp -o build/vertex_bcc_test.exe
#include <bits/stdc++.h>
using namespace std;

#define main vertex_bcc_example_main
#include "../codes/06-graph-04.cpp"
#undef main

int cases = 0;

void require(bool ok, const string &message) {
    if (!ok) throw runtime_error(message + " at case " + to_string(cases));
}

// Independent oracle: enumerate induced subgraphs, test connectivity after each
// vertex deletion, and retain only maximal sets. Bridges count as two-point blocks.
vector<int> oracle(int size, const vector<pair<int, int>> &edges) {
    vector<int> neighbors(size, 0), valid, maximal;
    for (auto [u, v] : edges) {
        if (u == v) continue;
        neighbors[u - 1] |= 1 << (v - 1);
        neighbors[v - 1] |= 1 << (u - 1);
    }
    auto connected = [&](int mask) {
        if (mask == 0) return true;
        int seen = mask & -mask, previous = 0;
        while (previous != seen) {
            previous = seen;
            for (int u = 0; u < size; ++u) {
                if ((seen >> u) & 1) seen |= neighbors[u] & mask;
            }
        }
        return seen == mask;
    };
    for (int mask = 1; mask < (1 << size); ++mask) {
        bool ok = connected(mask);
        for (int u = 0; u < size && ok; ++u) {
            if ((mask >> u) & 1) ok = connected(mask ^ (1 << u));
        }
        if (ok) valid.push_back(mask);
    }
    for (int mask : valid) {
        bool contained = false;
        for (int other : valid) {
            if (mask != other && (mask & other) == mask) contained = true;
        }
        if (!contained) maximal.push_back(mask);
    }
    return maximal;
}

void check(int size, const vector<pair<int, int>> &edges) {
    ++cases;
    n = size;
    adj.assign(n + 1, {});
    for (auto [u, v] : edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<int> expected = oracle(n, edges);
    for (int repeat = 0; repeat < 2; ++repeat) {
        find_vertex_bcc();
        vector<int> actual;
        for (const auto &component : components) {
            int mask = 0;
            require(!component.empty(), "empty component");
            for (int u : component) {
                require(u >= 1 && u <= n, "vertex out of range");
                require(!(mask & (1 << (u - 1))), "duplicate vertex");
                mask |= 1 << (u - 1);
            }
            actual.push_back(mask);
        }
        sort(actual.begin(), actual.end());
        require(actual == expected, "component mismatch");
        require(stk.empty(), "stack not cleared");
        for (auto &neighbors : adj) reverse(neighbors.begin(), neighbors.end());
    }
}

int main() {
    check(0, {});
    for (int size = 1; size <= 6; ++size) {
        vector<pair<int, int>> possible;
        for (int u = 1; u <= size; ++u) {
            for (int v = u + 1; v <= size; ++v) possible.emplace_back(u, v);
        }
        for (int mask = 0; mask < (1 << possible.size()); ++mask) {
            vector<pair<int, int>> edges;
            for (int i = 0; i < (int)possible.size(); ++i) {
                if ((mask >> i) & 1) edges.push_back(possible[i]);
            }
            check(size, edges);
        }
    }
    mt19937 rng(20261026);
    for (int test = 0; test < 2000; ++test) {
        int size = 1 + rng() % 8;
        vector<pair<int, int>> edges;
        int count = rng() % 35;
        for (int i = 0; i < count; ++i) {
            edges.emplace_back(1 + rng() % size, 1 + rng() % size);
        }
        check(size, edges);
    }
    // Beyond the old fixed capacity, with shallow recursion to isolate storage.
    n = 500100;
    adj.assign(n + 1, {});
    for (int u = 2; u <= n; ++u) {
        adj[1].push_back(u);
        adj[u].push_back(1);
    }
    find_vertex_bcc();
    require((int)components.size() == n - 1, "large star count");
    vector<bool> seen(n + 1, false);
    for (const auto &component : components) {
        require(component.size() == 2 && component[0] == 1, "large star block");
        require(!seen[component[1]], "duplicate leaf");
        seen[component[1]] = true;
    }
    require(stk.empty(), "large star stack");
    check(1, {{1, 1}});
    check(0, {});
    cout << "PASS: " << cases << " oracle graphs (repeated builds), "
         << "500100-vertex star, empty graphs, loops and parallel edges\n";
}
