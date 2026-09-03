# ============================================================
#  GDB 命令行调试常用命令示例
#  编译: g++ -std=c++17 -g -Wall -O0 -o debug 09-debug-02.cpp
#  启动: gdb ./debug
# ============================================================

# -------------------- 启动与帮助 --------------------
(gdb) help                # 查看帮助
(gdb) help breakpoints    # 查看断点相关帮助

# -------------------- 断点设置 --------------------
(gdb) break main          # 在 main 函数入口设断点
(gdb) b divide            # 在 divide 函数设断点（b 是 break 简写）
(gdb) b 9                 # 在第 9 行设断点
(gdb) b 16 if i == 3      # 条件断点：i == 3 时触发
(gdb) info breakpoints    # 查看所有断点
(gdb) delete 1            # 删除编号为 1 的断点
(gdb) disable 2           # 禁用编号为 2 的断点
(gdb) enable 2            # 启用编号为 2 的断点

# -------------------- 运行程序 --------------------
(gdb) run                 # 运行程序（带断点）
(gdb) r                   # 简写
(gdb) run arg1 arg2       # 带命令行参数运行

# -------------------- 单步调试 --------------------
(gdb) step                # 步入函数内部（s）
(gdb) next                # 步过，不进入函数（n）
(gdb) finish              # 运行到当前函数返回
(gdb) until 20            # 运行到第 20 行
(gdb) continue            # 继续运行到下一断点（c）

# -------------------- 查看变量 --------------------
(gdb) print i             # 打印变量 i 的值（p）
(gdb) p arr               # 打印数组/容器
(gdb) p &arr[0]           # 打印地址
(gdb) p *ptr              # 打印指针指向的值
(gdb) display i           # 每次暂停都自动显示 i
(gdb) undisplay 1         # 取消自动显示
(gdb) info locals         # 查看所有局部变量
(gdb) info args           # 查看函数参数

# -------------------- 内存与数组 --------------------
(gdb) p pref              # 打印 vector（需 pretty-printer）
(gdb) p *pref._M_impl._M_start @ 5  # 打印从该地址起的 5 个元素
(gdb) x/10dw arr          # 用 examine 查看 10 个 int（d=十进制, w=4字节）
(gdb) x/20xb buf          # 查看 20 个字节（x=十六进制, b=1字节）

# -------------------- 调用栈 --------------------
(gdb) backtrace           # 查看调用栈（bt）
(gdb) frame 2             # 切换到第 2 层栈帧（f 2）
(gdb) up                  # 切换到上一层栈帧
(gdb) down                # 切换到下一层栈帧

# -------------------- 修改变量 --------------------
(gdb) set var i = 0       # 修改变量 i 的值
(gdb) set var arr[0] = 42 # 修改数组元素

# -------------------- 调试已崩溃的程序 --------------------
# 先运行程序直到崩溃:
(gdb) run
# 程序崩溃后（Segmentation fault / Floating point exception）:
(gdb) backtrace            # 查看崩溃位置的调用栈
(gdb) frame 0              # 切换到最内层栈帧
(gdb) info locals          # 查看局部变量
(gdb) list                 # 查看崩溃位置附近的源代码
(gdb) p a                  # 打印相关变量
(gdb) p b                  # 发现 b == 0，定位除零错误

# -------------------- 调试 Core Dump --------------------
# 编译后运行 ulimit -c unlimited，然后运行程序生成 core 文件
(gdb) gdb ./debug core     # 用 core 文件启动 GDB
(gdb) bt                   # 直接查看崩溃时的调用栈

# -------------------- 反向调试（需要 Record） --------------------
(gdb) record               # 开始记录执行轨迹
(gdb) run                  # 运行程序
(gdb) reverse-step         # 反向单步
(gdb) reverse-next         # 反向步过
(gdb) reverse-continue     # 反向继续

# -------------------- 多线程调试 --------------------
(gdb) info threads         # 查看所有线程
(gdb) thread 2             # 切换到线程 2
(gdb) thread apply all bt  # 查看所有线程的调用栈

# -------------------- 其他实用命令 --------------------
(gdb) list                 # 查看当前行附近的源代码（l）
(gdb) list 10,20           # 查看第 10~20 行
(gdb) disassemble          # 反汇编当前函数
(gdb) info registers       # 查看寄存器
(gdb) quit                 # 退出 GDB（q）
(gdb) shell clear          # 执行 shell 命令（不清除 GDB 状态）

# ============================================================
#  典型调试流程
# ============================================================
# 1. 编译含调试信息的程序
#    g++ -g -Wall -O0 -o program source.cpp
#
# 2. 启动 GDB
#    gdb ./program
#
# 3. 设置断点
#    (gdb) b main
#
# 4. 运行
#    (gdb) r
#
# 5. 单步跟踪
#    (gdb) s / n
#
# 6. 查看变量
#    (gdb) p variable
#
# 7. 定位崩溃
#    (gdb) r → bt → frame 0 → info locals → list
#
# 8. 退出
#    (gdb) q
# ============================================================
