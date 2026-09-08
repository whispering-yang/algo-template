std::string gen() {
    using namespace std;

    int n = rnd(2, 10); // 随机生成节点数 n

    vector<int> nums(n);
    iota(nums.begin(), nums.end(), 1);

    vector<pair<int, int>> out;

    for (int i = 0; i < n - 1; i++) {
        // 选择左、右端点
        int l = rnd(0, i);
        int r = rnd(i + 1, n - 1);

        // 添加一条连接 nums[l] 和 nums[r] 的边
        out.emplace_back(nums[l], nums[r]);

        // 以 50% 概率交换边的两个端点顺序（使输出方向随机）
        if (rnd(0, 1)) swap(out[i].first, out[i].second);

        swap(nums[i + 1], nums[r]);
    }

    // 对所有生成的边进行随机打乱，使输出顺序不可预测
    shuffle(out.begin(), out.end(), rnd.rnd);

    string res = to_string(n) + "\n";
    for (auto [u, v] : out) {
        res += to_string(u) + " " + to_string(v) + "\n";
    }

    return res;
}
