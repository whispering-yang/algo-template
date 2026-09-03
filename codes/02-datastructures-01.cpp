std::array<int64_t, 100> dsu; // 此处设置范围，也可改用std::vector
int find(int u) {
    if (dsu.at(u) == u) return u;
    return dsu.at(u) = find(dsu.at(u));
}
void merge(int u, int v) {
    dsu[find(v)] = find(u);
}

// initialise
std::iota(dsu.begin(), dsu.end(), 0);
