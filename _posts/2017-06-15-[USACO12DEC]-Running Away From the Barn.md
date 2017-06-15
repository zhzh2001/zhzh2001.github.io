---
layout: post
tags: 题解
---

## 思路

这题也有很多方法，其实并不用可并堆。类似于上一种方法，计算出dfs序和每个点的深度，然后按深度降序排序。对于每个点，需要删除深度相差超过L的点，并加入当前点，在子树中统计答案。而这些用树状数组就可以了（单点修改+区间查询）。时间复杂度$O(N\log N)$

当然，还有一种更加巧妙的方法。用倍增求出$2^i$步祖先，然后就可以找到第一个距离超过L的祖先，用差分区间加，最后就能算出答案。时间复杂度也是$O(N\log N)$，下面提供了官方题解中的这种方法的代码供参考。

## 代码

```cpp
#include<bits/stdc++.h>
using namespace std;
const int N=200005;
int n,id[N],r[N],now,ans[N];
pair<long long,int> a[N];
struct BIT
{
	int tree[N];
	void modify(int x,int val)
	{
		for(;x<=n;x+=x&-x)
			tree[x]+=val;
	}
	int query(int x)
	{
		int ans=0;
		for(;x;x-=x&-x)
			ans+=tree[x];
		return ans;
	}
}T;
//树状数组模板
vector<pair<int,long long>> mat[N];
void dfs(int k)
{
	id[k]=++now;
  //dfs序
	for(auto e:mat[k])
	{
		a[e.first]=make_pair(a[k].first+e.second,e.first);
		dfs(e.first);
	}
	r[k]=now;
  //结束时间戳
}
int main()
{
	long long l;
	cin>>n>>l;
	for(int i=2;i<=n;i++)
	{
		int p;
		long long w;
		cin>>p>>w;
		mat[p].push_back(make_pair(i,w));
	}
	a[1]=make_pair(0ll,1);
	dfs(1);
	sort(a+1,a+n+1);
	int j=n;
	for(int i=n;i;i--)
	{
		for(;a[j].first-a[i].first>l;j--)
			T.modify(id[a[j].second],-1);
      //删除超过距离超过L的点
		T.modify(id[a[i].second],1);
      //插入当前点
		ans[a[i].second]=T.query(r[a[i].second])-T.query(id[a[i].second]-1);
      //统计子树答案
	}
	for(int i=1;i<=n;i++)
		cout<<ans[i]<<endl;
	return 0;
}
```

官方题解中的代码：

```pascal
var ans,d:array[0..200333]of int64;
    i,n,j,v:longint;
    len:int64;
    p:array[0..200333,0..19]of longint;
  begin
    assign(input,'runaway.in');reset(input);
    assign(output,'runaway.out');rewrite(output);
    read(n,len);
    ans[1]:=1;
    for i:=2 to n do
      begin
        read(p[i,0]);
        read(d[i]);
        d[i]:=d[i]+d[p[i,0]];
        //计算深度
        for j:=1 to 18 do
          p[i,j]:=p[p[i,j-1],j-1];
          //倍增求祖先
        v:=i;
        for j:=18 downto 0 do
          if d[i]-d[p[v,j]]<=len then v:=p[v,j];
          //找到最远的距离不超过L的祖先
        inc(ans[i]);
        dec(ans[p[v,0]]);
        //差分
      end;
    for i:=n downto 1 do
      ans[p[i,0]]:=ans[p[i,0]]+ans[i];
    for i:=1 to n do
      Writeln(ans[i]);
  end.
```

