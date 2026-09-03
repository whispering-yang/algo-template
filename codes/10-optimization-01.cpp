// 编译器优化指令：放在文件最开头（所有 #include 之前）
// 语法：#pragma GCC optimize("选项1,选项2,...")
#pragma GCC optimize("O3,unroll-loops,inline")

// 常用选项说明：
//   O2              常规优化（多数 OJ 默认开启）
//   O3              更强优化（含自动向量化等）
//   Ofast           激进优化（含 -ffast-math，可能降低浮点精度，慎用）
//   unroll-loops    循环展开，减少循环跳转开销
//   inline          强制内联短小函数
//   tree-vectorize  自动向量化（配合 O3 使用）
//   bmi, lzcnt, popcnt, avx2  启用特定指令集（需评测机 CPU 支持）

#include <bits/stdc++.h>
using namespace std;

// 注意：
//   1. 部分 OJ 会忽略 #pragma，需改用编译选项 -O2 / -Ofast / -march=native
//   2. 本地调试时建议去掉本行，以免影响断点与变量查看
//   3. 不要盲目上 O3/Ofast，与未定义行为结合可能产生错误结果
