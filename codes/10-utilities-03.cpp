struct Rnd {
    std::mt19937_64 rnd;
    Rnd() : rnd(std::random_device{}()) {}
    int64_t operator()() {
        return rnd();
    }
    int64_t operator()(int64_t l, int64_t r) {
        if (r < l) std::swap(l, r);
        return std::uniform_int_distribution<int64_t>(l, r)(rnd);
    }
} rnd;
