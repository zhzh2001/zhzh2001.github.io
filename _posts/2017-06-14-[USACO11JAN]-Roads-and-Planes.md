---
title: USACO11JAN 道路和飞机Roads and Planes
layout: post
tags: 题解
---

# 思路

这题要求的是带负权边的最短路，显然不能直接用Dijkstra。然而Bellman-Ford或SPFA的时间复杂度最坏$O(NM)$，而且众所周知，USACO总是卡SPFA的。尽管这题数据比较老，因此SPFA+SLF可以水过，但是正解并不是如此。

顺便说一下，用优先队列优化SPFA并不有效(来自[模板]单源最短路-题解)，在这题比普通SPFA更慢。然而在某些边权为正的卡SPFA的图中，几乎和Dijkstra不相上下。

可以发现，图有一个很强的性质：对于无向边，边权总是正的，因此每个无向边联通块是强联通的。而可能的负权边保证不能反向联通，因此把无向边联通块缩点后，得到的就是DAG。这样就可以在块内使用Dijkstra，块间利用拓扑排序更新答案。时间复杂度$O(M\log N)$。

# 代码

```cpp
#include<bits/stdc++.h>
using namespace std;
const int N=25005,INF=0x3f3f3f3f;
typedef pair<int,int> pii;
vector<pii> roads[N],planes[N];
vector<int> comp[N];
//联通分量中的点
int belong[N],indeg[N],d[N];
//belong[]表示每个点所属的联通分量，indeg[]表示联通分量的入度
bool vis[N];
void dfs(int k,int num)
{
    belong[k]=num;
    comp[num].push_back(k);
    for(auto e:roads[k])
        if(!belong[e.first])
            dfs(e.first,num);
}
//dfs洪水填充
int main()
{
    int n,m,p,s;
    cin>>n>>m>>p>>s;
    while(m--)
    {
        int u,v,w;
        cin>>u>>v>>w;
        roads[u].push_back(make_pair(v,w));
        roads[v].push_back(make_pair(u,w));
    }
    while(p--)
    {
        int u,v,w;
        cin>>u>>v>>w;
        planes[u].push_back(make_pair(v,w));
    }
    int c=0;
    for(int i=1;i<=n;i++)
        if(!belong[i])
            dfs(i,++c);
    for(int i=1;i<=n;i++)
        for(auto e:planes[i])
            indeg[belong[e.first]]++;
    fill(d+1,d+n+1,INF);
    d[s]=0;
    queue<int> Q;
    for(int i=1;i<=c;i++)
        if(!indeg[i])
            Q.push(i);
    while(!Q.empty())
    {
        int k=Q.front();Q.pop();
        priority_queue<pii,vector<pii>,greater<pii>> PQ;
        for(auto i:comp[k])
            if(d[i]<INF)
                PQ.push(make_pair(d[i],i));
        while(!PQ.empty())
        {
            pii k=PQ.top();PQ.pop();
            if(vis[k.second])
                continue;
            vis[k.second]=true;
            for(auto e:roads[k.second])
                if(d[k.second]+e.second<d[e.first])
                    PQ.push(make_pair(d[e.first]=d[k.second]+e.second,e.first));
            for(auto e:planes[k.second])
                d[e.first]=min(d[e.first],d[k.second]+e.second);
        }
        for(auto i:comp[k])
            for(auto e:planes[i])
                if(--indeg[belong[e.first]]==0)
                    Q.push(belong[e.first]);
    }
  //拓扑排序和Dijkstra
    for(int i=1;i<=n;i++)
        if(d[i]==INF)
            cout<<"NO PATH\n";
        else
            cout<<d[i]<<endl;
    return 0;
}
```

# 拓展：生成数据

这题很有趣，因此我思考了如何生成数据。直接随机显然不符合题意，需要一定的构造。我的方法是：可以先确定每个点所属的连通分量，然后在连通分量中生成树保证连通，再在连通分量内随机添加边。然后在连通分量间生成树保证连通，再随机加边。只要规模足够大，用这种方法生成的数据足以卡掉SPFA了。

附代码供参考(C++11)：

```cpp
#include<bits/stdc++.h>
using namespace std;
ofstream fout("roadplane.in");
const int n=100000,m=300000,w=50000,cn=1000,W=100000;
int belong[n+5];
vector<int> comp[cn+5];
int f[n+5];
int getf(int x)
{
	return f[x]==x?x:f[x]=getf(f[x]);
}
int main()
{
	minstd_rand gen(time(NULL));
	for(int i=1;i<=n;i++)
	{
		uniform_int_distribution<> dc(1,cn);
		belong[i]=dc(gen);
		comp[belong[i]].push_back(i);
	}
	assert(comp[1].size());
	uniform_int_distribution<> d(0,comp[1].size()-1);
	fout<<n<<' '<<m<<' '<<w<<' '<<comp[1][d(gen)]<<endl;
	for(int i=1;i<=cn;i++)
	{
		int cnt=0;
		for(int j=0;j<comp[i].size();j++)
			f[j]=j;
		while(cnt+1<comp[i].size())
		{
			uniform_int_distribution<> did(0,comp[i].size()-1);
			int u=did(gen),v=did(gen),ru=getf(u),rv=getf(v);
			if(ru!=rv)
			{
				f[ru]=rv;
				cnt++;
				uniform_int_distribution<> dw(0,W);
				fout<<comp[i][u]<<' '<<comp[i][v]<<' '<<dw(gen)<<endl;
			}
		}
	}
	for(int i=n-cn+1;i<=m;i++)
	{
		uniform_int_distribution<> dc(1,cn);
		int c=dc(gen);
		if(!comp[c].size())
		{
			i--;
			continue;
		}
		uniform_int_distribution<> did(0,comp[c].size()-1),dw(0,W);
		fout<<comp[c][did(gen)]<<' '<<comp[c][did(gen)]<<' '<<dw(gen)<<endl;
	}
	for(int i=1;i<=cn;i++)
		f[i]=i;
	for(int i=1;i<cn;)
	{
		uniform_int_distribution<> dc(1,cn-1);
		int cu=dc(gen);
		uniform_int_distribution<> dc1(cu+1,cn);
		int cv=dc1(gen),rcu=getf(cu),rcv=getf(cv);
		if(rcu==rcv)
			continue;
		f[rcu]=rcv;
		if(!comp[cu].size()||!comp[cv].size())
			continue;
		uniform_int_distribution<> didu(0,comp[cu].size()-1),didv(0,comp[cv].size()-1),dw(-W,W);
		fout<<comp[cu][didu(gen)]<<' '<<comp[cv][didv(gen)]<<' '<<dw(gen)<<endl;
		i++;
	}
	for(int i=cn;i<=w;)
	{
		uniform_int_distribution<> dc(1,cn-1);
		int cu=dc(gen);
		uniform_int_distribution<> dc1(cu+1,cn);
		int cv=dc1(gen);
		if(!comp[cu].size()||!comp[cv].size())
			continue;
		uniform_int_distribution<> didu(0,comp[cu].size()-1),didv(0,comp[cv].size()-1),dw(-W,W);
		fout<<comp[cu][didu(gen)]<<' '<<comp[cv][didv(gen)]<<' '<<dw(gen)<<endl;
		i++;
	}
	return 0;
}
```

