---
layout: post
tags:
- 扫描线
- 线段树
- 树套树
title: USACO16JAN 割草场 Mowing the Field
---

# 本文约定

$\left\vert S\right\vert$:集合$S$的元素数量

$$\{x\vert p(x)\}$$:符合$p(x)$的元素组成的集合

$\land$:逻辑与

$\lor$:逻辑或

<!-- more -->

# 思路

首先根据题意(以及官方题解)，一个交点应该只会出现一次，所以只要考虑时间在范围外的交点数即可。也就是说，与第$i$条竖直线段相交的线段

$$c_i=\left\vert\{j\vert x_j^l<x_i<x_j^r\land y_i^l<y_j<y_i^r\land\left\vert t_i-t_j\right\vert\ge T\}\right\vert$$

当然，这个可以$O(N^2)$枚举。如果把绝对值拆开，就变成$t_j+T\le t_i\le t_j+T$，这时就变成经典的三维偏序问题了，可以在$O(N\log^2N)$内解决。

我们可以把竖直线段作为查询操作。按照x坐标排序，用扫描线维护水平线段$i$，保存点$(t_i,y_i)$。对于竖直线段$i$，查询$$\left\vert\{(t_j,y_j)\vert(t_j\le t_i-T\lor t_j\ge t_i+T)\land(y_i^l<y_j<y_i^r)\}\right\vert$$即为答案。注意，线段端点相交不计入答案，在处理x坐标时也要注意。

至于数据结构，我用了官方的树状数组套线段树。

# 代码

```cpp
#include <iostream>
#include <algorithm>
using namespace std;
const int N = 100005, LOGN = 32;
struct node
{
	int sum, ls, rs;
} tree[N * LOGN * 4];
int cc;
//线段树
void modify(int &id, int l, int r, int x, int val)
{
	if (!id)
		id = ++cc;
	if (l == r)
		tree[id].sum += val;
	else
	{
		int mid = (l + r) / 2;
		if (x <= mid)
			modify(tree[id].ls, l, mid, x, val);
		else
			modify(tree[id].rs, mid + 1, r, x, val);
		tree[id].sum = tree[tree[id].ls].sum + tree[tree[id].rs].sum;
	}
}
int query(int id, int l, int r, int L, int R)
{
	if (!id)
		return 0;
	if (L <= l && R >= r)
		return tree[id].sum;
	int mid = (l + r) / 2;
	if (R <= mid)
		return query(tree[id].ls, l, mid, L, R);
	if (L > mid)
		return query(tree[id].rs, mid + 1, r, L, R);
	return query(tree[id].ls, l, mid, L, R) + query(tree[id].rs, mid + 1, r, L, R);
}
int n, t;
//树状数组
struct BIT
{
	int root[N];
	void modify(int t, int x, int val)
	{
		for (; t <= n; t += t & -t)
			::modify(root[t], 0, 1e9, x, val);
		//  ^  全局名称空间
	}
	int query(int t, int l, int r)
	{
		int ans = 0;
		for (; t; t -= t & -t)
			ans += ::query(root[t], 0, 1e9, l, r);
		return ans;
	}
} T;
struct event
{
	int x, y, t, val;
	event() {}
	event(int x, int y, int t, int val) : x(x), y(y), t(t), val(val) {}
	bool operator<(const event &rhs) const
	{
		return x < rhs.x;
	}
} E[N * 2];
struct query_t
{
	int x, yl, yr, t;
	query_t() {}
	query_t(int x, int yl, int yr, int t) : x(x), yl(yl), yr(yr), t(t) {}
	bool operator<(const query_t &rhs) const
	{
		return x < rhs.x;
	}
} Q[N];
int main()
{
	ios::sync_with_stdio(false);
	int px, py;
	cin >> n >> t >> px >> py;
	int en = 0, qn = 0;
	for (int i = 2; i <= n; i++)
	{
		int x, y;
		cin >> x >> y;
		if (y == py)
		{
			E[++en] = event(min(px, x) + 1, y, i, 1);
			E[++en] = event(max(px, x), y, i, -1);
			//本来应该为[l,r+1]，去除端点为[l+1,r]
		}
		else
			Q[++qn] = query_t(x, min(py, y) + 1, max(py, y) - 1, i);
		px = x;
		py = y;
	}
	sort(E + 1, E + en + 1);
	sort(Q + 1, Q + qn + 1);
	long long ans = 0;
	for (int i = 1, j = 1; i <= qn; i++)
	{
		for (; j <= en && E[j].x <= Q[i].x; j++)
			T.modify(E[j].t, E[j].y, E[j].val);
		if (Q[i].t - t > 0)
			ans += T.query(Q[i].t - t, Q[i].yl, Q[i].yr);
		if (Q[i].t + t <= n)
			ans += T.query(n, Q[i].yl, Q[i].yr) - T.query(Q[i].t + t - 1, Q[i].yl, Q[i].yr);
	}
	cout << ans << endl;
	return 0;
}
```

