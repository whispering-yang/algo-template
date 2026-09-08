# 算法竞赛模板项目指南

## 项目概述

这是一个 LaTeX 编写的**算法竞赛模板/板子汇总**文档。主文档 `main.tex` 通过 `\include{}` 导入 `sections/` 目录下的各章节子文件，使用 `xelatex` 编译，支持中文。

## 工作区结构

```
├── main.tex                      # 主文档入口（封面 + 目录 + include 子章节）
├── AGENTS.md                     # 本文件：AI 行为规则
│
├── styles/
│   └── mystyle.sty               # 可选的样式定制包
│
├── codes/                         # 外部代码文件（通过 \lstinputlisting 引入）
│   ├── 01-stdlib-01.cpp
│   ├── 02-datastructures-01.cpp   # 并查集(数组)
│   ├── 02-datastructures-02.cpp   # 并查集(map)
│   ├── ...                        # 命名规则：NN-topic-序号.后缀，NN 与所属章节文件编号一致
│   └── 10-optimization-01.cpp
│
└── sections/                     # 按主题拆分的子文档（每个文件对应一个 section）
    ├── 01-stdlib.tex             # 标准库方法
    ├── 02-datastructures.tex     # 数据结构
    ├── 03-sorting.tex            # 排序与分治
    ├── 04-strings.tex            # 字符串
    ├── 05-geometry.tex           # 计算几何
    ├── 06-graph.tex              # 图论
    ├── 07-math.tex               # 数学
    ├── 08-utilities.tex          # 实用工具
    ├── 09-debug.tex              # 本地调试
    └── 10-optimization.tex       # 常数优化
```

## 编译命令

```bash
# 在项目根目录执行（连跑两遍以生成目录）
xelatex -interaction=nonstopmode main.tex
xelatex -interaction=nonstopmode main.tex
```

编译产物输出到项目根目录。第二遍编译用于让目录（TOC）与页码正确生成。

## 添加新内容的规则

### 1. 新增算法/子主题

- **定位章节**：先判断新内容所属的主题类别，放入对应的 `sections/` 文件中。
- **命名格式**：使用 `\subsection{中文名 (English Name)}` 作为小节标题。
- **内容结构**：依次包含以下部分：
  1. **用途说明**：用 `\textbf{用途：}` 简述算法解决的问题。
  2. **核心概念/原理**：用 `\begin{itemize}` 列出关键概念。
  3. **算法流程**：若有复杂流程，用 `\begin{enumerate}` 分步说明。
  4. **时间复杂度**：用 `\textbf{时间复杂度：}` 强调。
  5. **代码模板**：在 `codes/` 目录下创建 `.cpp` 文件，用 `\lstinputlisting[language=C++]{codes/xx-name.cpp}` 引入。
- **外部代码同步**：所有代码块均存放在 `codes/` 目录中，修改 `.cpp` 文件后重新编译即可更新 PDF。
- **C++ 代码规范**：代码使用 C++17 风格，变量命名清晰，必要时添加注释。

### 2. 新增章节（新的 section）

- 在 `sections/` 目录下创建新文件，命名规则：`NN-topic.tex`（`NN` 为两位数字序号，需与现有编号顺序衔接）。
- 在 `main.tex` 的 `\begin{document}` 中添加对应的 `\include{sections/NN-topic}`。
- 文件第一行以 `\section{章节名}` 开头。

### 3. 修改现有内容

- **算法修正**：可直接编辑对应 `sections/` 文件中的代码块或文字说明。
- **排版调整**：切勿修改 `main.tex` 的 `\lstset`、`\geometry` 等全局设置，除非你确认有必要。
- **样式变更**：如需修改颜色、页眉页脚、标题样式，请编辑 `styles/mystyle.sty`。
- **删除内容**：删除后确保 `\include` 引用未被残留，且目录中无孤立引用。

## LaTeX 排版约定

- 行内代码：使用 `\code{...}` 自定义命令（等价于 `\texttt{...}`）。
- 数学公式：行内用 `$...$`，独立公式用 `\[...\]` 或 `$$...$$`。
- 高亮强调：使用 `\textbf{...}`，不使用 `\textit{...}`。
- 表格：优先使用 `\begin{tabular}`（简单表）或 `\begin{tabularx}`（自适应宽度表）。
- 引用/提示框：使用 `\begin{quote}` 配 `\textbf{注意：}`。
- 代码块：统一使用 `\lstinputlisting[language=C++]{codes/xx-name.cpp}` 引入外部文件，缩进 4 空格，不省略 `#include`。
- 英文术语：首次出现时给出中文名和英文名，如 `并查集 (Disjoint Set Union)`。
- 命令/函数名：使用 `\code{...}` 包裹，如 `\code{std::iota}`。

## 注意事项

- 所有 `sections/` 中的子文件**不应**包含 `\documentclass`、`\begin{document}` 或 `\end{document}`。
- 子文件中的 `\section` / `\subsection` 层级将与 `main.tex` 中的目录自动整合。
- 添加新文件后请执行一次编译以确认目录（TOC）能正确生成；若正文已更新但目录不变，参见「编译命令」一节中的 aux 残留问题。
