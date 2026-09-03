// =============================================================================
// File:     06-math-06 — 线性基 (Linear Basis)
// =============================================================================

#include <bits/stdc++.h>
using namespace std;

// 线性基 — 一组数在 XOR（GF(2) 向量空间）意义下的张成空间的基
struct LinearBasis {
    static constexpr int LOG = 60;     // 值域位数，插入的元素需 < 2^60
    array<int64_t, LOG> basis;         // basis[i]：最高位为第 i 位的基向量（0 表示空）
    int basis_size;                    // 基的大小（线性无关的向量个数）
    bool zero_representable;           // 是否存在非空子集异或为 0（插入失败时置位）

    LinearBasis() { reset(); }

    void reset() {
        basis.fill(0);
        basis_size = 0;
        zero_representable = false;
    }

    // 插入一个数 x，返回是否插入成功（false 表示 x 已能被现有基表示）
    bool insert(int64_t x) {
        for (int i = LOG - 1; i >= 0; --i) {
            if (!(x >> i & 1)) continue;       // 跳过第 i 位为 0 的位
            if (!basis[i]) {                   // 该位还没有基向量：放入并结束
                basis[i] = x;
                ++basis_size;
                return true;
            }
            x ^= basis[i];                     // 用基向量消去最高位，继续降低
        }
        zero_representable = true;             // x 被完全消为 0：存在异或为 0 的非空子集
        return false;
    }

    // 判断 x 是否能被当前基表示（即 x 属于张成空间）
    bool contains(int64_t x) const {
        for (int i = LOG - 1; i >= 0; --i) {
            if (!(x >> i & 1)) continue;
            if (!basis[i]) return false;       // 无法消去这一位
            x ^= basis[i];
        }
        return true;
    }

    // 查询张成空间中的最大异或值：从高位到低位，能变大就异或
    int64_t query_max() const {
        int64_t result = 0;
        for (int i = LOG - 1; i >= 0; --i) {
            if ((result ^ basis[i]) > result) result ^= basis[i];
        }
        return result;
    }

    // 查询张成空间中的最小异或值（可为 0：当且仅当原数组线性相关）
    // 上三角形式下任意非空子集异或 >= 其最高位基向量的首位，故最小值即最小的基向量
    int64_t query_min() const {
        if (zero_representable) return 0;
        for (int i = 0; i < LOG; ++i) {
            if (basis[i]) return basis[i];
        }
        return 0;                              // 空基
    }

    // -------- 第 k 小查询 --------
    // 先将基化为"最简"形式：basis[i] 的最高位唯一为 i，且不出现在其他基向量中
    void rebuild() {
        for (int i = LOG - 1; i >= 0; --i) {
            if (!basis[i]) continue;
            for (int j = i - 1; j >= 0; --j) {
                if (basis[i] >> j & 1) basis[i] ^= basis[j];
            }
        }
    }

    // 查询张成空间中第 k 小的异或值（k 从 0 开始，第 0 小即 0）
    // 注意：需要预先调用 rebuild()
    int64_t kth_smallest(int64_t k) const {
        vector<int64_t> nonzero_basis;         // rebuild 后按下标升序 = 按数值升序
        for (int i = 0; i < LOG; ++i) {
            if (basis[i]) nonzero_basis.push_back(basis[i]);
        }
        if (k >= (int64_t(1) << nonzero_basis.size())) return -1;  // k 超出 2^m - 1
        int64_t result = 0;
        for (int i = 0; i < (int)nonzero_basis.size(); ++i) {
            if (k >> i & 1) result ^= nonzero_basis[i];          // 按 k 的二进制位选取
        }
        return result;
    }

    // -------- 合并两个线性基（结果能表示两者张成空间中的所有元素） --------
    LinearBasis merge(const LinearBasis& other) const {
        LinearBasis result = *this;
        for (int i = LOG - 1; i >= 0; --i) {
            if (other.basis[i]) result.insert(other.basis[i]);
        }
        return result;
    }

    // 获取基大小
    int size() const { return basis_size; }
};

// -------- 使用示例 (main) --------
// int main() {
//     LinearBasis lb;
//     vector<int64_t> nums = {5, 3, 7, 10};
//     for (auto x : nums) lb.insert(x);
//
//     // 最大异或值
//     cout << "Max: " << lb.query_max() << "\n";
//
//     // 第 k 小
//     lb.rebuild();
//     cout << "k=0: " << lb.kth_smallest(0) << "\n";   // 0
//     cout << "k=1: " << lb.kth_smallest(1) << "\n";   // 最小非零值
//     return 0;
// }
