---
layout: post
tags:
- bitset
- 平衡树
- 贪心
- 矩阵
- 表达式
- 基数排序
- 外部排序
title: 简单的测试题 题解
---

# 1.排序

## 原题

USACO07MAR Gold T2:Ranking the Cows

原题范围：$n\le1000,m\le10000$

[题目](https://daniu.luogu.org/problem/show?pid=2881)	[官方题解](http://train.usaco.org/TESTDATA/MAR07.ranking.htm)		[数据](http://train.usaco.org/TESTDATA/ranking.zip)

## 题解

应该可以发现，把一对关系转换成一条边后，问题就转换成求出有向图的传递闭包。做完传递闭包后，答案就等于$\frac {n*(n-1)/2}2$-联通的点对数。当有向图中出现环时，即为自相矛盾。

考虑如何计算传递闭包：

### Floyd

使用$f_{i,j}=f_{i,j}\lor(f_{i,k}\land f_{k,j})$，其中$k,i,j\in 1\ldots n$计算。当然，初始值为$$f_{i,j}=\begin{cases}true&i=j\\ false &i\ne j\end{cases}$$。

最后统计答案时注意减去自环，同时判断是否有环。

这样时间复杂度时$O(n^3)$的，可以得到30分。

 <!-- more -->

#### 压位优化

用$T_i^k$表示从i经过前k个点能到达的点的集合，则有

$$T_i=\begin{cases}T_i^{k-1}&k\notin T_i^{k-1}\\T_i^{k-1}\cup T_k^{k-1}&k\in T_i^{k-1}\end{cases}$$

使用std::bitset或者手动压位，时间复杂度为$O(\frac{n^3}w)$，可以得到60分。

### 拓扑序

如果原图不是DAG，则自相矛盾，否则进行拓扑排序。倒着枚举拓扑序中的点u，那么u的可达点集为u可以直接到达的点的可达点集取并。

实际上不需要求出拓扑排序，只要dfs就可以解决了，时间复杂度$O(nm)$，可以得到60分。

压位后时间复杂度为$O(\frac{nm}w)$，可以得到100分。

## 总结

预期难度：普及+/提高

作为T1还是比较简单的，因为省选讲过。

# 2.录制

## 原题

USACO14JAN Silver T3:Recording the Moolympics

原题范围：$n\le150,k=2$

[题目](http://www.usaco.org/index.php?page=viewproblem2&cpid=381)	[官方题解](http://www.usaco.org/current/data/sol_recording.html)		[数据](http://www.usaco.org/current/data/recording.zip)

## 题解

### k=1

当只有一台录像机时相对比较简单，你应该录制的下一个项目总是最早结束并且还没有开始的。

这样显然时最优的，因为这样有更大的机会能录制后面的项目。所以只要按结束时间排序后贪心即可，时间复杂度$O(n\log n)$。

当然也可以使用动态规划，方程$f_i=f_j+1$，其中$t_j\le s_i$，时间复杂度$O(n^2)$。

### k=2

很遗憾，如果直接运行两次上述贪心的答案并不正确。以下是一个简单的反例：

```
|-| |-| |---------|
|---------| |-| |-|
```

其中-表示项目的时间，第一次贪心选择了短的那些项目后，就不能同时选择两个长的项目了。而最优方案为上图描述。

但是仍然可以用动态规划，用二维的状态，并且强制选择的项目必须在当前状态的两个节目都没有开始前结束。这样可以避免重复，时间复杂度$O(n^3)$。

实际上，贪心也是可以做的，仍然按照结束时间排序，记录最后一次录制的项目结束时间。

- 如果一个项目在两台录像机的节目都没结束前开始，显然不能安排。
- 如果这个项目只能在一台录像机上录制，就在这台录像机上录制。
- 否则，贪心地选择在结束时间迟的录像机上录制。

因为选择排在后面的项目不会比当前项目更优，所以可以证明其正确性。

### 更普遍的情况

当$k\le n$时，仍然可以使用动态规划，只不过时间复杂度为$O(n^{k+1})$，空间复杂度$O(n^k)$，实际无法接受。

然而，贪心很容易推广到$k\le n$的情况。维护所有录像机上最后一次录制的项目结束时间，对于每个项目，找到所有录像机结束时间中**最大的小于等于**开始时间的，将其更新为结束时间。如果没有找到，则不能安排这个项目。

用平衡树来维护所有录像机的结束时间，可以在$O(\log k)$的时间内完成上述操作。使用std::multiset就很方便(结束时间可能有重复)，先用upper\_bound找到第一个大于开始时间的位置，再--就得到了**最后一个小于等于**开始时间的位置。把这个值删除，并插入新的值即可。这样时间复杂度为$n\log  n+n\log k=O(n\log n)$。

### 记录方案

使用动态规划很容易记录方案，只要在转移时同时维护方案即可。

贪心也很容易记录，在维护结束时间的同时，记录这个时间对应的录像机编号。在更新时把项目编号记录到对应的录像机编号中即可，用std::vector或边表。

## 总结

预期难度：普及+/提高

主要考察了贪心，以及STL的运用。实际上USACO银组使用set非常常见，虽然原题不用。

# 3.加法解释

## 原题

USACO17OPEN Platinum T3:COWBASIC

与原题完全相同

[题目](http://www.usaco.org/index.php?page=viewproblem2&cpid=746)	[官方题解](http://www.usaco.org/current/data/sol_cowbasic_platinum_open17.html)		[数据](http://www.usaco.org/current/data/cowbasic_platinum_open17.zip)

## 题解

对于没有嵌套的数据，直接模拟即可。而只有一个变量的情况也可以一些方法通过。下面考虑普遍的情况：

实际上，将一段代码重复运行若干次可以使用通项公式。但是这种方法逻辑上比较复杂，而且还需要逆元，具体请参见官方题解。

更简单的方法是使用矩阵，因为矩阵本来就用来解决这类问题。例如循环$x=ax+b​$，构造初始矩阵$$V=\begin{pmatrix}x\\1\end{pmatrix}​$$，转移矩阵$$M=\begin{pmatrix}a&b\\ 0&1\end{pmatrix}​$$，则$$MV=\begin{pmatrix}a*x+b*1\\0*x+1*1\end{pmatrix}​$$，既然$M​$表示一次循环，则$M^n​$就可以表示n次循环。而$M^n​$只要用矩阵快速幂求即可。

普通的赋值语句则可以用矩阵乘法实现，十分方便。这样，只要记录所有变量和1，按照程序构造矩阵，最后输出要求的变量的值。使用std::map来标记变量名的序号也很方便。

循环嵌套用栈来实现，保存临时矩阵和循环次数。当有新的循环时，压入一个单位矩阵；循环结束时，执行完矩阵快速幂后与下一层相乘。

每个循环最多计算$\log_210^5\sim17$次100x100的矩阵乘法，最多有50个循环。实际运算量不到1亿，注意以下常数问题就可以通过了。

另外，语法分析也比较麻烦，充分利用空格是很方便的。

## 总结

预期难度：提高+/省选-(洛谷的难度偏高)

这题还是比较巧妙的，因此在原题和这里都作为T3。需要认真思考，熟练应用矩阵，并且一定的代码实现能力。

# 4.新挑战\*

## 题解

### 思路

测试点#1直接std::sort就可以了。如果内存足够，那么测试点#2也只要如此就可以了。由于内存不够，我们需要使用**外部排序**。测试点#3内存足够开2个数组了，但是直接std::sort太慢了，我们需要使用**浮点数基数排序**。

**外部排序**就是把数据分成若干段，每段都在内存中排序后写入临时文件。然后执行多路归并，最好用堆维护最小值。这样时间复杂度仍然是$O(n\log n)$的，只是文件读写需要额外的时间。实际上，用二进制文件保存，总共只要读40MB写40MB，使用缓冲区技术，还是很快的。

**基数排序**就是用冬令营的*松式基排*，参考洛谷上的[题解](https://www.luogu.org/wiki/show?name=%E9%A2%98%E8%A7%A3+P1177)，排序2亿个32位整数都不在话下。关键在于如何把浮点数转换为整数。实际上，我们使用的浮点数都有一个很强的性质：如果符号相同，那么除去符号位外剩余的位解释成整数，其大小关系等价于原数的绝对值大小关系。也就是说，32位浮点数可以与32位整数一一对应，除了-0和NaN外。具体的，浮点数相当于整数原码，转为补码后把符号位取反就是32位**无符号**整数了。

实际上，这题考察了相当多的技巧：

### 动态分配内存

由于不同的测试点内存限制不同，就需要根据数据规模动态分配内存。实际上，C/C++/Pascal中都有相关的方法。另外，作为一种好习惯，用完这些内存后，应该在**堆**上释放空间。

C通常用`malloc`或`calloc`来分配内存，前者不初始化空间，而后者初始化为0.用`free`来释放空间。具体的介绍可以自己查阅资料。

```c
#include<stdlib.h>
void* malloc( size_t size );
void* calloc( size_t num, size_t size );
void free( void* ptr );
```

C++用`new`来分配内存，用`delete`来释放，比C更加方便。例如

```cpp
float *a=new float [n];
//do something
delete [] a;
```

Pascal除了用`new/dispose`来动态分配单个对象外，还可以用`GetMem/FreeMem`来分配内存块。

```pascal
procedure Getmem(out p: pointer; Size: PtrUInt);
procedure Freemem(p: pointer; Size: PtrUInt)
```

### 执行转换

在**浮点数基数排序**中，我们需要把浮点数转换为整数，或者反着转换，要求其位表示相同。除了用指针外，更加有趣的方法使用C/C++中的`union`或Pascal中的变体记录。

`union`与`struct`类似，只不过每次只能保存一种值，用来转换最为方便。例如

```c
union
{
	unsigned ul;
	float ft;
}u;
u.ft=readNext();
a[i]=u.ul;
//do something
u.ul=a[i];
writeNext(u.ft);
```

此外，C++中的关键字`reinterpret_cast`也可以转换，但好像有些问题，请不要使用。

变体记录与`union`类似，例如

```pascal
type
  union=record
  case boolean of
  false:(x:longint);
  true:(ft:single);
  end;
var
  u:union;
```

### 二进制文件

在**外部排序**中，我们需要用二进制文件来保存临时结果。

在Pascal中很方便，只要用`file of single`类型即可，而且可以直接用`read/write`，不过必须手动指定文件名。

而在C/C++中，用`fread/fwrite`更加方便，而且文件打开模式中必须有"b"，如`tmpfile`函数就使用了"wb+"模式。设置缓冲区也很有用，能减少I/O次数，提高效率。以下代码演示了临时文件的使用：

```cpp
#include<bits/stdc++.h>
using namespace std;
int main()
{
	FILE *f=tmpfile();
	if(!f)
		return 1;
	int n;
	cin>>n;
	for(int i=1;i<=n;i++)
		fwrite(&i,sizeof(i),1,f);
	rewind(f);
	while(!feof(f))
	{
		int x;
		fread(&x,sizeof(x),1,f);
		if(!feof(f))
			cout<<x<<' ';
	}
	cout<<endl;
	return 0;
}
```

### 使用STL中的堆算法

C++用std::priority\_queue可能会出现奇怪的问题，内存也可能会太大。建议使用STL中的堆算法。

```cpp
template< class RandomIt, class Compare >
void make_heap( RandomIt first, RandomIt last,
                Compare comp );
template< class RandomIt, class Compare >
void push_heap( RandomIt first, RandomIt last,
                Compare comp );
template< class RandomIt, class Compare >
void pop_heap( RandomIt first, RandomIt last, Compare comp );
```

具体的介绍可以自行查阅资料。

## 总结

预期难度：提高+/省选-(仅代码)

这题考察了对于综合性问题的解决，难度较大，需要阅读分析给定的资料。

这题的灵感来自《深入理解计算机系统》中的解释，其实网上也有类似的内容，不过我是根据资料自己推出公式的，从想法到最终实现用了大约一个星期。

> 这不是偶然的——IEEE格式如此设计就是为了浮点数能够使用整数排序函数进行排序。当处理负数时，有一个小的难点，因为它们有开头的1，并且它们是按照降序出现的，但是不需要浮点运算来进行比较也能解决这个问题（参见家庭作业2.84）。
>
> ——摘自《深入理解计算机系统》第2章：信息的表示和处理

