std::map<int, int64_t> dsu;
int find(int u) {
    if (!dsu.count(u) || dsu.at(u) == u) return u;
    return dsu.at(u) = find(dsu.at(u));
}
void merge(int u, int v) {
    dsu[find(v)] = find(u);
}
