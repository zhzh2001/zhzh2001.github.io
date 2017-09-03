---
layout: post
tags: 题解
title: 使用压位计算传递闭包
---

> 参考ZJOI2017 Day2讲课《动态传递闭包问题的探究》

题目：洛谷P2881 [USACO07MAR]排名的牛Ranking the Cows

# 总体思路

## 分析

给定n($n\le1000$)个数的m($m\le10000$)个大小关系，求出最少增加几个大小关系才可以给这些数排序。

很明显，如果没有任何已知关系，答案为$C_n^2=\frac{n*(n-1)}{2}$。要计算已知的关系能使答案减小的值，可以使用传递闭包，一般常用Floyd。

但是由于$n\le1000$，$O(n^3)$不能通过，那么怎么做呢？

## 改进Floyd

*以下来自课件*

$t_{i,j}^{(k)}$表示i和j经过前k个点是否连通。

> 定义集合$T_i^{(k)}=\{j\vert t_{i,j}^{(k)}=1\}$

对于$k\ge1$，有

$$T_i^{(k)}=\begin{cases}T_i^{(k-1)}&k\notin T_i^{(k-1)} \\ T_i^{(k-1)}\cup T_k^{(k-1)}&k\in T_i^{(k-1)}\end{cases}$$

用bitset或手动压位，可以在$O(\frac{n^3}{w})​$求出传递闭包，其中w表示字长，为64或32。

## 使用拓扑序

对于DAG，从后往前枚举拓扑序中的点u，那么u的可达点集为u可以直接到达的点的可达点集取并。

相信大家都会在$O(n+m)$求解拓扑序，其实不需要求出拓扑序。只需dfs，在回溯时取并即可。

时间复杂度为$O(\frac{nm}{w})$，即使不压位也能通过，应该是正解。

# 程序清单

## 改进Floyd

```cpp
#include<bits/stdc++.h>
using namespace std;
const int N=1005;
bitset<N> mat[N];
int main()
{
	int n,m;
	cin>>n>>m;
	for(int i=1;i<=n;i++)
		mat[i][i]=true;
	while(m--)
	{
		int u,v;
		cin>>u>>v;
		mat[u][v]=true;
	}
	for(int k=1;k<=n;k++)
		for(int i=1;i<=n;i++)
			if(mat[i][k])
				mat[i]|=mat[k];
	int ans=0;
	for(int i=1;i<=n;i++)
		ans+=mat[i].count();
	cout<<n*(n-1)/2-ans+n<<endl;
	return 0;
}
```

bitset::count()返回true的位计数，类似于popcount。

由于开始设置了自环，在计算答案时应当去掉。

## 拓扑序

```cpp
#include<bits/stdc++.h>
using namespace std;
const int N=1005,M=10005;
int head[N],v[M],nxt[M],e;
bitset<N> vis,mat[N];
inline void add_edge(int u,int v)
{
	::v[++e]=v;
	nxt[e]=head[u];
	head[u]=e;
}
void dfs(int k)
{
	mat[k][k]=vis[k]=true;
	for(int i=head[k];i;i=nxt[i])
	{
		if(!vis[v[i]])
			dfs(v[i]);
		mat[k]|=mat[v[i]];
	}
}
int main()
{
	int n,m;
	cin>>n>>m;
	while(m--)
	{
		int u,v;
		cin>>u>>v;
		add_edge(u,v);
	}
	for(int i=1;i<=n;i++)
		if(!vis[i])
			dfs(i);
	int ans=0;
	for(int i=1;i<=n;i++)
		ans+=mat[i].count();
	cout<<n*(n-1)/2-ans+n<<endl;
	return 0;
}
```

# 总结

这里介绍了DAG上的两种求静态传递闭包的方法。对于稀疏图使用拓扑序更快，而稠密图则用Floyd更为简单。

如果有环则需要Tarjan缩点后处理，较为复杂。

