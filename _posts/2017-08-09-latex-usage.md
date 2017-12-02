---
layout: post
tags:
- LaTeX
- 教程
- intro
title: LaTeX小结
---

# 前言

有时我们需要出题，用什么好呢？

- MS Word?我从来不用，虽然是WYSIWYG[^wysiwyg]
  - 细节格式调整复杂，结构不清晰
  - 商业格式，并且跨平台效果差
  - 数学公式不太美观和方便
  - 导出PDF不太完善
- Markdown?以前一直用，虽然方便快捷也有缺点(用Typora)
  - 表格支持差，不能合并单元格
  - 数学公式无法复制，显示为图片
  - 无法显式分页，写博客没问题，但是出题就不太好了
- $\LaTeX$!
  - 支持复杂的表格
  - 美观的数学公式
  - 结构清晰，能生成PDF书签

当然，缺点也很多：

- 安装、学习较为复杂
- 需要时间编译，Word和Typora都是WYSIWYG
- 中文需要一定的配置，虽然现在已经比较简单
- 写文章也需要debug……

<!-- more -->

# 安装

推荐使用跨平台的TeX Live，下载当年的版本，最好是ISO。

在Windows下，使用`install-tl-advanced.bat`，更改安装位置，可以删除不需要的组件，比如空间巨大的字体。

然后就慢慢等好了。参考<https://liam0205.me/texlive/>

# 使用

## 工具

推荐使用TeXworks作为编辑器，当然Notepad++之类的通用编辑器也是可以的。如果使用TeX Live，很容易在安装目录下找到TeXworks。TeXworks与一般的Markdown编辑器一样，使用左右对照。

排版工具用XeLaTeX，其实pdfLaTeX也可以。在TeXworks中可以在首选项中修改默认排版工具。

## hello,world

```latex
\documentclass[UTF8]{ctexart}
\begin{document}
你好，世界！
\end{document}
```

以上代码可以输出`你好，世界！`到PDF文件，也可以试一下pdfLaTeX。

以\\开始的行为命令，如果有begin，就要有对应的end，这样的命令称为**环境**。{}中是必要的参数，[]中是可选参数，用逗号分割。

上述代码第一行表示文档类为`ctexart`，使用UTF8编码。使用`ctexart`没有中文问题。接下来就是文章的主要部分了。

## 文档类选项

### 生成PDF书签

在文档类选项中加入`hyperref`即可。但是需要注意，第一次生成`*.out`时不会加入书签，必须再次编译。以前这样做是为了节省内存，于是一直保留至今。

### 改变文章字号

在文档类选项中加入如`12pt`即可，默认为`10pt`。

## 其他设置

在`\documentclass`和`\begin{document}`之间可以进行设置。`\usepackage`类似于C的`#include`，用于添加特定的功能。

### 标题

```latex
\title{Title}
\author{Author}
\date{}

\begin{document}
\maketitle
% 显示标题
\end{document}
```

注意，{}中不填将不会显示对应项目。

### 标题格式

```latex
\usepackage{titling}
\pretitle{\begin{center}\fontsize{30pt}{30pt}\selectfont}
\posttitle{\end{center}}
```

### 页边距

```latex
\usepackage{geometry}
\geometry{left=1in,right=1in,top=1in,bottom=1in}
```

### 显式分页

```latex
\newpage
```

## 正文

### 注释

```latex
% comment
```

单行注释用%，注意如果要输入%应该用`\%`。多行注释用`comment`环境。

### 层次结构

与Markdown六级标题不同，各个层次有特定的名称：

```latex
\section {...}       % Level 1
\subsection {...}    % Level 2
\subsubsection {...} % Level 3
\paragraph {...}     % Level 4
\subparagraph {...}  % Level 5
```

### 数学公式

```latex
$n!=\prod_{i=1}^n n\;\;\forall n\ge1$
```

使用`amsmath`包。单行公式可以用Markdown的`$...$`，多行注释用`equation`环境。

至于内容，和在Markdown中的完全相同。

### 列表

```latex
注意事项：
\begin{enumerate}
\item 注意编译选项，避免未定义行为或编译错误。
\item 代码长度限制为100KB。
\item 注意代码常数和I/O造成的效率影响。
\end{enumerate}

部分分：
\begin{itemize}
\item 40\%
\item 60\%
\end{itemize}
```

有序列表用`enumerate`环境，无序列表用`itemize`环境。每个项目用`\item`开始。

### 原样输出

```latex
使用\verb|printf|函数。

输入样例：
\begin{verbatim}
sample input
\end{verbatim}
```

单行用`\verb|...|`，多行用`verbatim`环境。

### 脚注和超链接

```latex
WYSIWYG\footnote{参见\url{https://zh.wikipedia.org/zh-hans/\%E6\%89\%80\%E8\%A6\%8B\%E5\%8D\%B3\%E6\%89\%80\%E5\%BE\%97}}
```

必须加入`hyperref`选项，用`\footnote{}`命令建立脚注，用`\url{}`命令创建超链接，注意转义。

### 表格

```latex
\begin{center}
\begin{tabular}{|p{100pt}|p{100pt}|p{100pt}|p{100pt}|}
\hline
题目名称 & A & B & C\\
\hline
目录 & \verb|A| & \verb|B| & \verb|C|\\
\hline
可执行文件名 & \verb|A| & \verb|B| & \verb|C|\\
\hline
输入文件名 & \verb|A.in| & \verb|B.in| & \verb|C.in|\\
\hline
输出文件名 & \verb|A.out| & \verb|B.out| & \verb|C.out|\\
\hline
时间限制 & 1s & 1s & 1s\\
\hline
空间限制 & 512MB & 512MB & 512MB\\
\hline
测试点数量 & 20 & 20 & 20\\
\hline
测试点分数 & 5 & 5 & 5\\
\hline
比较方式 & SPJ & 全文 & 全文\\
\hline
部分分 & 有 & 无 & 无\\
\hline
\end{tabular}
\end{center}
```

表格是相当复杂的，一般使用`tabular`环境。表格通常居中显示，因此用`center`环境。

#### 设置

在`tabular`开始处，需要设定宽度、居中等。\|表示竖直线，`p{...}`用于设置宽度。有时既需要居中有需要设置宽度，可以加入以下设置：

```latex
\usepackage{array}
\newcolumntype{P}[1]{>{\centering\arraybackslash}p{#1}}
```

这样用`P{...}`即可。

#### 横向分割线

用`\hline`可以绘制完整的横向分割线，用`\cline{l-r}`可以绘制$l\dots r$列的横向分割线。

使用`makecell`包，用`\Xhline{x\arrayrulewidth}`可以绘制宽度为普通分割线$x$倍的分割线，一般用于首行、次行和末行。我用的参数分别为$x=3,2,3$。最好不要用`booktabs`包中的`\toprule`，`\midrule`和`\bottomrule`，因为有一些问题，也可以试一下。

#### 数据行

用&分割两个单元格，用`\\`表示换行(在正文中也一样)。

#### 合并单元格

一般常用的方式是多行单元格，用`multirow`包中的`\multirow{x}{*}{...}`。表示合并接下来的$x$行，内容为`...`，\*用于默认宽度。

注意，此时一定要用`\cline`代替`\hline`，可能要用多个。另外，在接下来$x$行内对应位置，不能输入任何内容，但仍要占位，如`&&`。

### 强调

```latex
\underline{\textbf{强调}}
\emph{强调2}
```

在题目中，一般常用的格式为<u>**强调**</u>，使用`\underline{\textbf{...}}`。而`\emph{...}`通常会换一种字体，适合用于定义词语。

### 伪代码

```latex
\begin{algorithm}
\begin{algorithmic}[1]
\Procedure{Sort}{$A,l,r$}\Comment{对$A[l\dots r]$排序}
\State{$i\gets l$}
\State{$j\gets r$}
\State{$x\gets A[(l+r)/2]$}\Comment{$x$为\emph{基准值}}
\Repeat
\While{$A[i]<x$}
\State{$i\gets i+1$}
\EndWhile
\While{$x<A[j]$}
\State{$j\gets j-1$}
\EndWhile
\If{$i\le j$}
\State{\Call{Swap}{$A[i],A[j]$}}
\State{$i\gets i+1$}
\State{$j\gets j-1$}
\EndIf
\Until{$i>j$}
\If{$l<j$}
\State{\Call{Sort}{$l,j$}}
\EndIf
\If{$i<r$}
\State{\Call{Sort}{$i,r$}}
\EndIf
\EndProcedure
\end{algorithmic}
\end{algorithm}
```

需要`algorithm`和`algpseudocode`包，依次用`algorithm`和`algorithmic`环境。其语法与`Pascal`较为类似，命令首字母大写。

[^wysiwyg]: **W**hat **Y**ou **S**ee **I**s **W**hat **Y**ou **G**et