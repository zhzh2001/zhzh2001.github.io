---
layout: post
title: USACO06JAN 围栏 Corral the Cows
tags:
- 二分
- 扫描线
- 线段树
---

# 思路

一个正方形可以由左上角坐标和边长确定，只要枚举所有正方形就能找到答案。所以时间复杂度=枚举左上角坐标的时间\*枚举边长的时间\*统计正方形内的草场的时间。原题$N\le500$，实际上$\mathcal O(N^3)$就能过了。我们可以做二维前缀和，并二分枚举边长，并用单调性在$\mathcal O(N^2)$的时间内判断，总的复杂度是$\mathcal O(N^2\log N)$的。但是我一开始两次二分，所以时间复杂度$\mathcal O(N^2\log^2N)$，非常慢。

<!-- more -->

实际上，这个问题是可以做到更快的。二分仍然是必不可少的，而判定性问题可以转换成`USACO14MAR The Lazy Cow`(懒惰的牛)。可以发现，如果有至少一个方格被以草场为一角的正方形覆盖了达到$C$次就符合要求了。而这个问题可以用扫描线+线段树在$\mathcal O(N\log N)$内解决，因此总时间复杂度$\mathcal O(N\log^2N)$。官方题解中并没有给出这种方法，我是受到懒惰的牛(金组)的启发才想到的。

# 代码

```cpp
#include <iostream>
#include <algorithm>
using namespace std;
const int N = 505;
struct clover
{
	int x, y;
} a[N];
struct event
{
	int x, yl, yr, val;
	bool operator<(const event &rhs) const
	{
		return x < rhs.x;
	}
} e[N * 2];
int y[N * 2];
struct node
{
	int max, lazy;
} tree[1 << 11];
//注意离散化后有2N
void build(int id, int l, int r)
{
	tree[id].max = tree[id].lazy = 0;
	if (l < r)
	{
		int mid = (l + r) / 2;
		build(id * 2, l, mid);
		build(id * 2 + 1, mid + 1, r);
	}
}
inline void pushdown(int id, int l, int r)
{
	if (tree[id].lazy && l < r)
	{
		tree[id * 2].max += tree[id].lazy;
		tree[id * 2].lazy += tree[id].lazy;
		tree[id * 2 + 1].max += tree[id].lazy;
		tree[id * 2 + 1].lazy += tree[id].lazy;
		tree[id].lazy = 0;
	}
}
void update(int id, int l, int r, int L, int R, int val)
{
	if (L <= l && R >= r)
	{
		tree[id].lazy += val;
		tree[id].max += val;
	}
	else
	{
		pushdown(id, l, r);
		int mid = (l + r) / 2;
		if (L <= mid)
			update(id * 2, l, mid, L, R, val);
		if (R > mid)
			update(id * 2 + 1, mid + 1, r, L, R, val);
		tree[id].max = max(tree[id * 2].max, tree[id * 2 + 1].max);
	}
}
int main()
{
	ios::sync_with_stdio(false);
	int c, n;
	cin >> c >> n;
	for (int i = 1; i <= n; i++)
		cin >> a[i].x >> a[i].y;
	int l = 1, r = 1e4;
	while (l < r)
	{
		int mid = (l + r) / 2, en = 0, yn = 0;
		for (int i = 1; i <= n; i++)
		{
			e[++en].x = a[i].x - mid + 1;
			//以草场为右下角的正方形
			y[++yn] = e[en].yl = a[i].y - mid + 1;
			y[++yn] = e[en].yr = a[i].y;
			e[en].val = 1;
			e[++en].x = a[i].x + 1;
			e[en].yl = a[i].y - mid + 1;
			e[en].yr = a[i].y;
			e[en].val = -1;
		}
		sort(e + 1, e + en + 1);
		sort(y + 1, y + yn + 1);
		build(1, 1, yn);
		int now = 0;
		for (int i = 1; i <= en; i++)
		{
			if (e[i].x != e[i - 1].x)
				now = max(now, tree[1].max);
			//同一个x都更新再更新答案
			update(1, 1, yn, lower_bound(y + 1, y + yn + 1, e[i].yl) - y, lower_bound(y + 1, y + yn + 1, e[i].yr) - y, e[i].val);
		}
		if (now < c)
			l = mid + 1;
		else
			r = mid;
	}
	cout << r << endl;
	return 0;
}
```

### 题外话

这题虽然比较简单，但比较容易写错细节，很适合用于出题……我曾经T1出过$N\le3,000$，直接$\mathcal O(N^2\log N)$会被卡常，需要用官方的神奇优化或用$\mathcal O(N\log^2N)$的方法。当然，现在可以$N\le50,000$，作为T2？另外，实际上*std::lower_bound*确实比较慢，要求高时还是自己实现比较好。

有兴趣可以看一下我的题目，T3也是原题：https://zhzh2001.github.io/test/day2/

