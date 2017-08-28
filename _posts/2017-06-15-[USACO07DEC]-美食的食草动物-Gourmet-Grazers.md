---
layout: post
tags: 题解
---

# 思路

应该可以发现，这题可以用贪心。有两个值需要满足比较麻烦，我们考虑对询问和牧草按照口感（绿色值）排序。这样，对于一个询问(Ai,Bi)，只要从口感大于等于Bi的牧草中选出价格最小的且大于等于Ai的牧草，这样可以证明是最优的。如果没有符合条件的牧草，就是无解。

用multiset来维护牧草的价格很方便，支持所有操作，当然价格可以重复。时间复杂度$O(N\log N)$

# 代码

```cpp
#include<bits/stdc++.h>
using namespace std;
const int N=100005;
pair<int,int> cow[N],grass[N];
int main()
{
	int n,m;
	cin>>n>>m;
	for(int i=1;i<=n;i++)
		cin>>cow[i].second>>cow[i].first;
	sort(cow+1,cow+n+1);
	for(int i=1;i<=m;i++)
		cin>>grass[i].second>>grass[i].first;
	sort(grass+1,grass+m+1);
	multiset<int> S;
	int j=m;
	long long ans=0;
  //注意用64位整数保存结果
	for(int i=n;i;i--)
	{
		for(;j&&grass[j].first>=cow[i].first;j--)
			S.insert(grass[j].second);
      //插入口感满足要求的牧草
		multiset<int>::iterator it=S.lower_bound(cow[i].second);
      //找到第一个价格的大于等于Ai的牧草
		if(it==S.end())
		{
			cout<<-1<<endl;
			return 0;
		}
		ans+=*it;
		S.erase(it);
      //别忘了删除
	}
	cout<<ans<<endl;
	return 0;
}
```

