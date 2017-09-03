---
layout: post
tags: 题解
title: USACO13NOV 挤奶牛 Crowded Cows
---

# 思路

这题其实有很多方法，但是我最先想到的是按照奶牛的高度排序处理。而且这种方法也可以做被困在干草堆（金）。当然按照位置排序也是可以的。

首先按照奶牛的高度降序排序，在处理一只奶牛时，把大于等于它的高度的2倍的奶牛的位置放进set中，再判断一下set中的位置在当前奶牛两边的位置是否小于等于d，计入答案即可。

时间复杂度$O(N\log N)$，与其他方法相同，但常数比单调队列大。因为后者只要用到sort，比set常数小得多。

 <!-- more -->

# 代码

```cpp
#include<bits/stdc++.h>
using namespace std;
const int N=50005;
pair<int,int> cow[N];
int main()
{
	int n,d;
	cin>>n>>d;
	for(int i=1;i<=n;i++)
		cin>>cow[i].second>>cow[i].first;
	sort(cow+1,cow+n+1);
	set<int> pos;
	int j=n,ans=0;
	for(int i=n;i;i--)
	{
		for(;j&&cow[j].first>=cow[i].first*2;j--)
			pos.insert(cow[j].second);
      //插入高度大于等于当前高度2倍的奶牛
		set<int>::iterator it=pos.lower_bound(cow[i].second);
      //找到右边的奶牛
		ans+=it!=pos.end()&&*it-cow[i].second<=d&&it--!=pos.begin()&&cow[i].second-*it<=d;
      //判断两边是否满足条件
	}
	cout<<ans<<endl;
	return 0;
}
```

