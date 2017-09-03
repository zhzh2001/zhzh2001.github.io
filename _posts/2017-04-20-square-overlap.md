---
layout: post
tags: 题解 平衡树
title: USACO13JAN 方块重叠 Square Overlap
---

# 思路

## 整体

首先，我们可以知道，要满足题意，两个正方形的坐标$(x_i,y_i)(x_j,y_j)$必须满足$\vert x_i-x_j\vert<k$并且$\vert y_i-y_j\vert<k$。如果有且仅有一组正方形满足条件，那么重叠部分的面积$ans=\vert k-(x_i-x_j)\vert\times\vert k-(y_i-y_j)\vert$。

最简单的方法是直接暴力扫描，时间复杂度为$O(n^2)$。

我本来打算写一个二维线段树的，但好像有些大材小用了，难度才提高啊。

根据[官方题解](http://www.usaco.org/current/data/sol_squares.html)，先对所有点排序，然后维护与当前点横坐标差值小于k的所有点的集合，每次查找点集中纵坐标最接近当前点的点，更新答案即可。

如何维护所有满足条件的点呢？因为点是有序的，满足$x_i\le x_{i+1}$，每次只需将无效的点删除即可。

## 具体实现

使用`set`维护点集比较方便，删除和插入操作均为log级的。其中`set::insert`会返回一个`pair<iterator,bool>`，第一个为插入后的迭代器，第二个为插入是否成功。(参考http://en.cppreference.com/w/cpp/container/set/insert)

> cppreference是一个不错的C++参考网站

另外，答案应该用64位整数保存。

# 代码清单

```cpp
#include<bits/stdc++.h>
using namespace std;
const int N=50005;
typedef pair<int,int> pii;
pii p[N];
int main()
{
	int n,k;
	cin>>n>>k;
	for(int i=1;i<=n;i++)
		cin>>p[i].first>>p[i].second;
  //first表示横坐标 second表示纵坐标
	sort(p+1,p+n+1);
  //横坐标为第一关键字 纵坐标为第二关键字排序
	set<pii> S;
  //first表示纵坐标 second表示点的标号 其实只保存点的标号也可以 但必须自定义比较函数了
	long long ans=0;
	for(int i=1,j=1;i<=n;i++)
	{
		for(;j<i&&p[i].first-p[j].first>=k;j++)
			S.erase(make_pair(p[j].second,j));
      //删除不满足条件的点
		set<pii>::iterator it1=S.insert(make_pair(p[i].second,i)).first,it2=it1;
      //it1和it2均为当前点的迭代器
		if(it1--!=S.begin()&&p[i].second-it1->first<k)
			if(!ans)
				ans=(long long)(k-abs(p[i].first-p[it1->second].first))*(k-(p[i].second-it1->first));
			else
				ans=-1;
		if(++it2!=S.end()&&it2->first-p[i].second<k)
			if(!ans)
				ans=(long long)(k-abs(p[i].first-p[it2->second].first))*(k-(it2->first-p[i].second));
			else
				ans=-1;
      //注意set::iterator只能进行自增自减 不能进行+1/-1
	}
	cout<<ans<<endl;
	return 0;
}
```

时间复杂度为$O(n\log n)$

