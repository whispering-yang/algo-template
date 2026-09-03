std::string gen() {
    using namespace std;

    int n = rnd(2, 10); // 随机生成节点数 n

    vector<int> nums(n);
    iota(nums.begin(), nums.end(), 1);

    vector<pair<int, int>> out(n - 1);

    shuffle(nums.begin(), nums.end(), rnd.rnd); // 连接边随机

    for (int i = 0; i < n - 2; i++) {
        auto &[u, v] = out[i];
        u = nums[i], v = nums[i + 1];
        if (rnd(0, 1)) swap(u, v); // 方向随机
    }

    shuffle(out.begin(), out.end(), rnd.rnd); // 边输出顺序随机

    // 将单链转换为字符串
    string res = to_string(n) + "\n";
    for (auto [u, v] : out) {
        res += to_string(u) + " " + to_string(v) + "\n";
    }

    return res;
}
