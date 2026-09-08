# algo-template

算法竞赛模板/板子汇总，使用 LaTeX 编写，支持中文，通过 [XeLaTeX](https://www.tug.org/xetex/) 编译为 PDF。

## 项目结构

```
├── main.tex        # 主文档入口（封面 + 目录 + include 子章节）
├── styles/
│   └── mystyle.sty # 样式定制包（颜色、页眉页脚、标题样式）
├── codes/          # C++ 代码模板（通过 \lstinputlisting 引入）
└── sections/       # 按主题拆分的章节
    ├── 01-stdlib.tex          # 标准库方法
    ├── 02-datastructures.tex  # 数据结构
    ├── 03-sorting.tex         # 排序与分治
    ├── 04-strings.tex         # 字符串
    ├── 05-geometry.tex        # 计算几何
    ├── 06-graph.tex           # 图论
    ├── 07-math.tex            # 数学
    ├── 08-utilities.tex       # 实用工具
    ├── 09-debug.tex           # 本地调试
    └── 10-optimization.tex    # 常数优化
```

## 编译

在项目根目录执行（连跑两遍以生成目录）：

```bash
xelatex -interaction=nonstopmode main.tex
xelatex -interaction=nonstopmode main.tex
```

编译产物输出到项目根目录，第二遍编译用于让目录（TOC）与页码正确生成。

## 贡献 / 添加新内容

详见 [AGENTS.md](AGENTS.md)，要点如下：

- **新增算法**：放入对应 `sections/` 文件，使用 `\subsection{中文名 (English Name)}` 命名，依次包含用途说明、核心概念、算法流程、时间复杂度与代码模板。
- **代码模板**：在 `codes/` 目录下创建 `.cpp` 文件（C++17 风格），用 `\lstinputlisting[language=C++]{codes/xx-name.cpp}` 引入。
- **新增章节**：在 `sections/` 下创建 `NN-topic.tex`，并在 `main.tex` 中添加 `\include{sections/NN-topic}`。
- **样式调整**：修改 `styles/mystyle.sty`，不要改动 `main.tex` 的全局设置。
