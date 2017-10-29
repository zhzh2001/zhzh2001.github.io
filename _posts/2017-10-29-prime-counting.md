---
layout: post
title: 质数计数
tags: 数学
---

# 前言

很久以前，我出了[区间质数]({% post_url 2017-02-15-mytest %})，当然求$l\dots r$之间的质数等于$\pi(r)-\pi(l-1)$，其中$\pi(n)$为质数计数函数，表示$1\dots n$中质数的个数。

我最初给出的方法是分段打表+Miller Rabin素性判定，可以解决$n\le10^9$规模，但是更大的规模无法打表，因为内存不够……最近看到洛谷P3912后，zyy提供了LibreOJ #6235，求$\pi(n),n\le10^{11}$。对此我想大致了解一下质数计数算法。

# 筛法

计算$\pi(n)$最简单的想法就是筛法，这里同时来比较一下埃氏筛和欧拉筛的实际效率，以及*bitset*优化的效率。与算法竞赛不同的是，代码避免使用静态数组，而是根据输入规模动态分配内存。因此与静态数组的性能有一些差异。

## 埃氏筛

### 原始版本(era.cpp)

把$\sqrt n$内的质数的倍数筛掉，时间复杂度$O(n\log\log n)$，空间复杂度$O(n)$。一个小的优化是直接忽略偶数(除了2)。这里假设$n\ge2$。

```c++
#include <iostream>
#include <algorithm>
#include <ctime>
using namespace std;
typedef unsigned int_t;
int main()
{
	int_t n;
	cin >> n;
	clock_t start = clock();
	bool *p = new bool[n + 1];
	fill(p + 2, p + n + 1, true);
	for (int_t i = 3; i * i <= n; i += 2)
		if (p[i])
			for (int_t j = i * i; j <= n; j += i + i)
				p[j] = false;
	int_t cnt = 1;
	for (int_t i = 3; i <= n; i += 2)
		cnt += p[i];
	cout << cnt << endl;
	delete[] p;
	cout << clock() - start << endl;
	return 0;
}
```

### *bitset*(era_bitset.cpp)

值得注意的是，*std::bitset*是静态的模板，因此我实际上用了*std::vector\<bool\>*。时间复杂度不变，但是常数小，因为空间局部性吧。空间复杂度$O(\frac n8)$。

```c++
#include <iostream>
#include <vector>
#include <ctime>
using namespace std;
typedef unsigned int_t;
int main()
{
	int_t n;
	cin >> n;
	clock_t start = clock();
	vector<bool> p(n + 1, true);
	for (int_t i = 3; i * i <= n; i += 2)
		if (p[i])
			for (int_t j = i * i; j <= n; j += i + i)
				p[j] = false;
	int_t cnt = 1;
	for (int_t i = 3; i <= n; i += 2)
		cnt += p[i];
	cout << cnt << endl;
	cout << clock() - start << endl;
	return 0;
}
```

### 分段筛(era_segmented.cpp)

我最早是在某篇论文上看到的，但是<http://primesieve.org/segmented_sieve.html>的介绍和*代码*很容易理解。先生成$\sqrt n$内的质数表，然后分段筛，段长度接近L1数据缓存。能很大程度提高效率，因为增加了缓存命中率。空间复杂度$O(\pi(\sqrt n)+B)$,$B$为段长度。

另外，这可以计算较小的区间内的质数，如$9.999*10^{11}\dots10^{12}$。这样取段大小$10^8$，应该可以通过LOJ那题。

```c++
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
using namespace std;
typedef unsigned int_t;
const int_t seg_size = 32768;
int main()
{
	int_t n;
	cin >> n;
	clock_t start = clock();
	int_t m = sqrt(n);
	bool *p = new bool[m + 1];
	fill(p + 2, p + m + 1, true);
	for (int_t i = 2; i * i <= m; i++)
		if (p[i])
			for (int_t j = i * i; j <= m; j += i)
				p[j] = false;
	bool *buf = new bool[seg_size];
	vector<int_t> primes;
	vector<int_t> nxt;
	int_t cnt = 1;
	for (int_t low = 0, s = 3, now = 3; low <= n; low += seg_size)
	{
		fill(buf, buf + seg_size, true);
		int_t high = min(low + seg_size - 1, n);
		for (; s * s <= high; s += 2)
			if (p[s])
			{
				primes.push_back(s);
				nxt.push_back(s * s - low);
			}
		for (size_t i = 0; i < primes.size(); i++)
		{
			int_t j = nxt[i];
			for (int_t k = primes[i] * 2; j < seg_size; j += k)
				buf[j] = false;
			nxt[i] = j - seg_size;
		}
		for (; now <= high; now += 2)
			cnt += buf[now - low];
	}
	cout << cnt << endl;
	delete[] p;
	delete[] buf;
	cout << clock() - start << endl;
	return 0;
}
```

## 欧拉筛

### 原始版本(euler.cpp)

```c++
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;
typedef unsigned int_t;
int main()
{
	int_t n;
	cin >> n;
	clock_t start = clock();
	bool *p = new bool[n + 1];
	fill(p + 2, p + n + 1, true);
	vector<int_t> primes;
	for (int_t i = 3; i <= n; i += 2)
	{
		if (p[i])
			primes.push_back(i);
		for (int_t j : primes)
		{
			if (i * j > n)
				break;
			p[i * j] = false;
			if (i % j == 0)
				break;
		}
	}
	cout << primes.size() + 1 << endl;
	delete[] p;
	cout << clock() - start << endl;
	return 0;
}
```

### *bitset*(euler_bitset.cpp)

```c++
#include <iostream>
#include <vector>
#include <ctime>
using namespace std;
typedef unsigned int_t;
int main()
{
	int_t n;
	cin >> n;
	clock_t start = clock();
	vector<bool> p(n + 1, true);
	vector<int_t> primes;
	for (int_t i = 3; i <= n; i += 2)
	{
		if (p[i])
			primes.push_back(i);
		for (int_t j : primes)
		{
			if (i * j > n)
				break;
			p[i * j] = false;
			if (i % j == 0)
				break;
		}
	}
	cout << primes.size() + 1 << endl;
	cout << clock() - start << endl;
	return 0;
}
```

## 效率

开O2优化，用*clock*测量，取最小值，单位：毫秒。

| $n$    | $\pi(n)$     | era      | era_bitset | era_seg | euler   | euler_bitset |
| ------ | ------------ | -------- | ---------- | ------- | ------- | ------------ |
| $10^7$ | $664,579$    | $79$     | $20$       | $12$    | $53$    | $44$         |
| $10^8$ | $5,761,455$  | $1,014$  | $538$      | $91$    | $621$   | $492$        |
| $10^9$ | $50,847,534$ | $11,752$ | $7,131$    | $1,194$ | $6,533$ | $5,309$      |

