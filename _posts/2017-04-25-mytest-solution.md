---
layout: post
tags: 题解
title: 我的创新题目 题解和标程
---

# 1.数列

## 概述

维护多个数列，要求每个数列支持修改首部、尾部，支持随机访问。而且时间复杂度应该为$O(1)$或$O(\log n)$(需要卡常？)

## 非正解

### 测试点#1~#3

如果只有一个数列，那么问题就非常简单了。

只要维护一个头指针和尾指针，开始时在数组中间。这样可以轻松地插入、删除、随机访问了。

*期望得分:30*

### 离线方法

有了处理一个数列的方法，我们只需对所有操作排序，依次处理每个数列即可。

### 链表维护

我们可以维护$n$个链表，那么插入、删除操作都是$O(1)$的，但是随机访问是$O(n)$的。

可以使用std::list，也可以自己实现链表。

*期望得分:40*

### std::vector维护

std::vector也可以用来维护数列，其中修改尾部和随机访问操作是$O(1)$的，修改首部操作是$O(n)$的(但是比数组快，有人认为接近于$\sqrt n$)。

*期望得分:60*(其中测试点#8就利用了std::vector的快速修改首部操作)

### 分段骗分

综合一个数列、链表和std::vector，按照测试点特征选择方法，其中有的测试点可以用多种方法通过。

*期望得分:80*(很良心吧)

 <!-- more -->

## 正解(欢迎补充)

### std::deque

这其实是我最初的目的，数列维护的其实就是deque(double end queue,双端队列)。要求的所有操作都是$O(1)$的，实际运行也最快。

### std::map

考虑最早的一个数列的解法，由于没有空间开所有的数列，所以不能做多个数列的情况。

而我们可以用std::map来作为数组，以节省空间。这样所有操作都是$O(\log n)$的，但是可以通过本题，虽然是最慢的。

### hash

既然可以用平衡树，那么也可以用hash来作为数组，除了手写以外，还有一下方法：

- std::unordered_map(c++11) 速度较快

- __gnu_pbds::cc_hash_table 速度很快，但是gp_hash_table无法通过本题

- __gnu_cxx::hash_map(deprecated) 不建议使用


理论所有操作的时间复杂度也是$O(1)$的，但是比std::deque慢。

## 关于I/O

### 数据规模

这题的输入和输出数据都很大，有$4*10^6$个操作，输入文件最大有78.6MiB，输出文件最大有21.8MiB。

### 解决方法

必须使用I/O优化，可以使用逐字符读写(可能无法用std::map卡过)，也可以用`fread/fwrite`更加高效(标程使用)。函数原型如下：

```cpp
std::size_t fread( void* buffer, std::size_t size, std::size_t count, std::FILE* stream );
std::size_t fwrite( const void* buffer, std::size_t size, std::size_t count, std::FILE* stream );
```

其中buffer为I/O缓冲区，对于文本文件size应该为1，count为缓冲区大小，stream为待操作的文件流。

##### Pascal

Free Pascal中也有类似的过程`BlockRead/BlockWrite`

```pascal
procedure BlockRead(var f: File; var Buf; count: Int64;var Result: Int64)
procedure BlockRead(var f: File; var Buf; count: LongInt;var Result: LongInt)
procedure BlockRead(var f: File; var Buf; count: Cardinal;var Result: Cardinal)
procedure BlockRead(var f: File; var Buf; count: Word; var Result: Word)
procedure BlockRead(var f: File; var Buf; count: Word;var Result: Integer)
procedure BlockRead(var f: File; var Buf; count: Int64)

procedure BlockWrite(var f: File; const Buf; Count: Int64;var Result: Int64)
procedure BlockWrite(var f: File; const Buf; Count: LongInt;var Result: LongInt)
procedure BlockWrite(var f: File; const Buf; Count: Cardinal;var Result: Cardinal)
procedure BlockWrite(var f: File; const Buf; Count: Word;var Result: Word)
procedure BlockWrite(var f: File; const Buf; Count: Word;var Result: Integer)
procedure BlockWrite(var f: File; const Buf; Count: LongInt)
```

其实这两个过程在Turbo Pascal中就存在，但是没有Result。其中f必须为无类型文件，Result保存成功的大小。必须指定Result，或者使用{$I-}。

##### 获取输入文件大小

有时我们无法计算输入文件大小，可以用以下的方法来获取：

```cpp
fseek(fin,0,SEEK_END);//将文件流定位到结尾处
long fsize=ftell(fin);//获取当前位置
pin=bufin=new char [fsize+1];//分配缓冲区 pin为输入指针 bufin为输入缓冲区起始位置
rewind(fin);//将文件流定位到开始处
```

## 标程

```cpp
#include<cstdio>
#include<deque>
#include<cctype>
using namespace std;
FILE *fin,*fout;
char *bufin,*bufout,*pin,*pout;
inline void read(int& x)
{
	for(;isspace(*pin);pin++);
	int sign=1;
	if(*pin=='-')
		sign=-1,pin++;
	x=0;
	for(;isdigit(*pin);pin++)
		x=x*10+*pin-'0';
	x*=sign;
}
int d[15];
inline void writeln(int x)
{
	if(x<0)
		*pout++='-',x=-x;
	int cnt=0;
	do
		d[++cnt]=x%10;
	while(x/=10);
	for(;cnt;cnt--)
		*pout++=d[cnt]+'0';
	*pout++='\n';
}
int main()
{
	fin=fopen("list.in","r");
	fout=fopen("list.out","w");
	fseek(fin,0,SEEK_END);
	long fsize=ftell(fin);
	pin=bufin=new char [fsize+1];
	rewind(fin);
	fread(bufin,1,fsize,fin);
	bufin[fsize]='\0';
	int n,m;
	read(n);read(m);
	pout=bufout=new char [m*12];
	deque<int> *a=new deque<int> [n+1];
	int lastans=0;
	while(m--)
	{
		int opt,i,j;
		read(opt);
		switch(opt)
		{
			case 1:
				read(i);read(j);
				a[i^lastans].push_back(j^lastans);
				break;
			case 2:
				read(i);
				a[i^lastans].pop_back();
				break;
			case 3:
				read(i);read(j);
				a[i^lastans].push_front(j^lastans);
				break;
			case 4:
				read(i);
				a[i^lastans].pop_front();
				break;
			case 5:
				read(i);read(j);
				writeln(lastans=a[i^lastans][(j^lastans)-1]);
				break;
			case 6:
				read(i);
				writeln(lastans=a[i^lastans].back());
				break;
		}
	}
	delete [] a;
	fwrite(bufout,1,pout-bufout,fout);
	fclose(fin);fclose(fout);
	delete [] bufin;
	delete [] bufout;
	return 0;
}
```

# 2.斐波那契数的长度

## 概述

计算斐波那契数的位数。

## 非正解

### 测试点#1~#5

使用题目提示的类型，最高为扩展精度浮点数。

*期望得分:50*

### 矩阵快速幂

我实现了一种用于计算位数的浮点数bigfloat(C++)

```cpp
struct bigfloat
{
    static const double LIMIT=1e150;
    static const int DIGITS=150;
    double d;
    long long overcnt;
    bigfloat()
    {
        d=overcnt=0;
    }
    bigfloat(double x,long long oc=0):d(x),overcnt(oc){};
    inline void adjust()
    {
        if(d>LIMIT)
        {
            overcnt++;
            d/=LIMIT;
        }
    }
    bigfloat operator+(const bigfloat& b)const
    {
        if(overcnt>b.overcnt)
            return *this;
        if(overcnt<b.overcnt)
            return b;
        bigfloat res(d+b.d,overcnt);
        res.adjust();
        return res;
    }
    bigfloat operator+=(const bigfloat& b)
    {
        return *this=*this+b;
    }
    bigfloat operator*(const bigfloat& b)const
    {
        bigfloat res(d*b.d,overcnt+b.overcnt);
        res.adjust();
        return res;
    }
};
```

由于双精度浮点数的运算速度比扩展精度快，所以使用double保存。d保存尾数和部分阶码，而overcnt保存溢出部分的计数。表示的数的位数等于log10(d)+1+overcnt*bigfloat::DIGITS

然而这种方法存在严重的精度问题，主要由矩阵快速幂引起。

*期望得分:50*

## 正解

### 公式

可以得出斐波那契数列的通项公式，参考<https://zh.wikipedia.org/wiki/%E6%96%90%E6%B3%A2%E9%82%A3%E5%A5%91%E6%95%B0%E5%88%97>:

$$f_n=\frac{(\frac{1+\sqrt5}2)^n-(\frac{1-\sqrt5}2)^n}{\sqrt5}$$

当计算位数时，可以简化为$f_n=\frac{(\frac{1+\sqrt5}2)^n}{\sqrt5}$

因此，答案为$\left\lfloor(\lg\frac{1+\sqrt5}2)*n-\lg\sqrt5\right\rfloor+1$

### 精度问题

请使用扩展精度浮点数以避免精度问题，其实浮点数精度问题并不玄学，可以这样解释(假设一般使用双精度浮点数):

- 双精度浮点数尾数有52位，可以连续精确表示的最大整数为$2^{53}-1\approx9*10^{15}$

- 扩展精度浮点数尾数有63位，可以连续精确表示的最大整数为$2^{64}-1\approx1.8*10^{19}$

此外，单精度浮点数只有23位尾数，而Turbo Pascal中的real有40位尾数。

## 标程

### C++

```cpp
#include<fstream>
#include<cmath>
using namespace std;
ifstream fin("fiblen.in");
ofstream fout("fiblen.out");
int main()
{
	long long n;
	fin>>n;
	fout.precision(0);
	fout<<fixed<<floor(log10((1.0L+sqrt(5.0L))/2.0L)*n-log10(sqrt(5.0L)))+1<<endl;
	return 0;
}
```

浮点数后缀L表示long double类型。

### C

```c
#define __USE_MINGW_ANSI_STDIO 1
#include<stdio.h>
#include<math.h>
int main()
{
	FILE *fin=fopen("fiblen.in","r");
	FILE *fout=fopen("fiblen.out","w");
	long long n;
	fscanf(fin,"%lld",&n);
	fprintf(fout,"%.0Lf\n",floorl(log10l((1.0L+sqrtl(5.0L))/2.0L)*n-log10l(sqrtl(5.0L)))+1.0L);
	fclose(fin);
	fclose(fout);
	return 0;
}
```

注意定义\_\_USE\_MINGW\_ANSI\_STDIO宏是为了在Windows下使用%lld和%Lf，默认不支持；在其他平台没有影响。注意对于long double，必须使用带l后缀的数学函数(C++有函数重载)。

### Pascal

```pascal
program fiblen;
uses
  math;
var
  n:int64;
begin
  assign(input,'fiblen.in');
  reset(input);
  assign(output,'fiblen.out');
  rewrite(output);
  read(n);
  writeln(trunc(log10((1+sqrt(5))/2)*n-log10(sqrt(5)))+1);
  close(input);
  close(output);
end.
```

Free Pascal默认以extended作为浮点数类型。

## 计算更高的精度

### 使用__float128

在i386和x86\_64体系下，GCC支持四精度浮点数\_\_float128，包括数学函数和I/O函数。\_\_float128有112位尾数。

这些函数定义在`<quadmath.h>`中，数学函数以q为后缀；I/O函数包含strtoflt128和quadmath_snprintf。使用这些函数必须连接libquadmath.\_\_float128常量使用Q后缀。

```c
#include<stdio.h>
#include<quadmath.h>
int main()
{
	__float128 n;
	char buf[50];
	scanf("%s",buf);
	n=strtoflt128(buf,NULL);
	quadmath_snprintf(buf,sizeof(buf),"%.0Qf",floorq(log10q((sqrtq(5.0Q)+1.0Q)/2.0Q)*n-log10q(sqrtq(5.0Q)))+1.0Q);
	printf("%s\n",buf);
	return 0;
}
```

### 使用GNU MPFR

MPFR是一个*多精度浮点数库*，基于GMP库。以下的示例使用了一种C++接口MPFR C++：

```cpp
#include<bits/stdc++.h>
#include<mpreal.h>
using namespace std;
using namespace mpfr;
int main()
{
	string s;
	cin>>s;
	mpreal::set_default_prec(digits2bits(s.length()+10));
	mpreal n(s),sqrt5(sqrt(mpreal(5.0)));
	cout.precision(0);
	cout<<fixed<<floor(log10((sqrt5+1.0)/2.0)*n-log10(sqrt5))+1<<endl;
	return 0;
}
```

# 3.区间质数

## 概述

求出区间的质数个数。

## 非正解

### 朴素判断质数

时间复杂度为$O(n\sqrt n)$

*期望得分:50*

### 筛数

使用埃氏筛法或欧拉筛法，能通过测试点#6，使用bitset可以通过测试点#7。

*期望得分:70*(在前一种方法的基础上)

## 正解

分块打表，选择适当的块大小和适当的判断质数的方法。

标程选择块大小为$5*10^5$，那么每次最多需要判断$10^6$个数是否为质数。显然使用朴素方法无法通过，所以使用Miller Rabin算法，能在大约$O(\log n)$的时间内判断一个数是否为质数。

但是如果选择不当，该算法有一定概率出错。但是对于题目要求的范围，只要判断2,7,61就可以保证正确性了。

## 标程

```cpp
#include<fstream>
using namespace std;
ifstream fin("prime.in");
ofstream fout("prime.out");
const int k=500000,ans[]={41538,...};
//因长度所限，请自行打表
int qmod(int a,int b,int p)
{
	long long ans=1,now=a;
	do
	{
		if(b&1)
			ans=(ans*now)%p;
		now=(now*now)%p;
	}
	while(b>>=1);
	return ans;
}
bool mr_test(int n,int a)
{
	if(n==2)
		return true;
	if(!(n&1))
		return false;
	int d=n-1;
	while(!(d&1))
		d>>=1;
	long long t=qmod(a,d,n);
	while(d!=n-1&&t!=n-1&&t!=1)
	{
		t=t*t%n;
		d<<=1;
	}
	return t==n-1||(d&1);
}
bool miller_rabin(int x)
{
	if(x<2)
		return false;
	int a[]={2,7,61};
	//enough for unsigned int
	for(int i=0;i<3;i++)
	{
		if(x==a[i])
			return true;
		if(!mr_test(x,a[i]))
			return false;
	}
	return true;
}
int cntrange(int l,int r)
{
	int ans=0;
	for(int i=l;i<=r;i++)
		ans+=miller_rabin(i);
	return ans;
}
int main()
{
	int l,r;
	fin>>l>>r;
	int cnt=0;
	for(int i=l/k;i<=r/k;i++)
		cnt+=ans[i];
	fout<<cnt-cntrange(l/k*k,l-1)-cntrange(r+1,r/k*k+k)<<endl;
	return 0;
}
```

注意标程中块的范围为$[kx,kx+k)$

