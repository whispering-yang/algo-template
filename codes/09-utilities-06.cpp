struct Timer {
    std::ostream &oss;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;

    Timer(std::ostream &oss = std::cout)
        : oss(oss), start(std::chrono::high_resolution_clock::now()) {}

    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        oss << (long double)ms / 1000 << " ms\n";
    }
};
