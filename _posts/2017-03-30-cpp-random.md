---
layout: post
tags: 教程
title: C++中的随机数
---

# 使用C的随机数

## 实现

C中*随机数函数*非常简单，定义在`<stdlib.h>`中：

- ```c
  void srand(unsigned seed);
  ```

  将随机种子设为`seed` ；**相同的种子将获得相同的随机数序列**，只要在程序开始初始化即可。

- ```c
  int rand();
  ```

  返回[0,RAND_MAX]之间的**伪随机数**。

## 初始化种子

一般使用`<time.h>`中的`time()`函数

```c
time_t time(time_t* arg);
```

返回当前的*日历时间*，如果`arg`不为空，同时赋给`arg`。

```c
srand(time(NULL));
srand(time(0));
```

```cpp
srand(time(nullptr));
```

> 关于NULL,0,nullptr：在C中推荐使用`NULL`表示空指针，在C++中推荐使用`0`，在C++11中使用`nullptr`更好；在`time()`中可以任意使用。

## 问题

### 时间问题

- 在`Windows`下，通常`time()`更新为**每秒一次**
- 在`Linux`下，通常`time()`更新为**每毫秒一次**

当使用`Windows`对拍时，问题很严重：**数据每秒才更新一次**。当程序计算很快时，很多组数据无效。

### 范围问题

- 在`Windows`下，通常`RAND_MAX`为32767($2^{15}-1$)
- 在`Linux`下，通常`RAND_MAX`等于`INT_MAX`

### 演示代码

```cpp
//this code should be compiled <since> C++14
#include<bits/stdc++.h>
using namespace std;
int main()
{
	while(clock()<10*CLOCKS_PER_SEC)
	{
		srand(time(NULL));
		cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b                                                            \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
		for(int i=1;i<=10;i++)
			cout<<rand()<<' ';
		this_thread::sleep_for(100ms);
	}
	return 0;
}
```

**编译说明：编译器必须至少支持C++14！** *若不支持C++11/14，使用`sleep()`代替`this_thread::sleep_for(100ms)`*

该代码将运行大约10秒，展示随机数的更新情况以及随机数的范围。

# 在`Windows`下解决时间问题

## 高精度计时器

### 实现

在`Windows`下提供了一种高精度计时器，使用`QueryPerformanceCounter`和`QueryPerformanceFrequency`。

高精度计时器一个周期**通常小于1微秒**，该API最低系统要求为`Windows 2000`。

```cpp
BOOL WINAPI QueryPerformanceCounter(
  _Out_ LARGE_INTEGER *lpPerformanceCount
);
```

获取当前的高精度计时器计数，成功则返回非0值。

```cpp
BOOL WINAPI QueryPerformanceFrequency(
  _Out_ LARGE_INTEGER *lpFrequency
);
```

获取高精度计时器的频率。

其中`LARGE_INTEGER`定义为

```c
typedef union _LARGE_INTEGER {
  struct {
    DWORD LowPart;
    LONG  HighPart;
  };
  struct {
    DWORD LowPart;
    LONG  HighPart;
  } u;
  LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;
```

### 使用方法

```c
LARGE_INTEGER t;
QueryPerformanceCounter(&t);
srand(t.LowPart);
```

## 毫秒计时器（备选）

### 实现

除了高精度计时器外，`Windows`还提供一个与`clock()`类似的函数`GetTickCount`。

```c
DWORD WINAPI GetTickCount(void);
```

获取**从系统启动开始**经过的毫秒数，最高可达49.7天。

> 也就是说当系统连续运行49.7天后会从`UINT_MAX`降到0
>
> 什么，不可能连续运行49.7天？如果你启用快速启动(Windows 8+)，且不重新启动，系统实际上会在关机后继续"运行"。打开任务管理器-性能-CPU可以看到运行时间，轻松创造运行几天的纪录。

### 问题

虽然该函数返回值精确到毫秒，但实际上该计时器一般**10~16毫秒更新一次**。在程序运行非常快时，仍然会产生相同的数据；不过这种情况较为少见。

### 使用方法

```c
srand(GetTickCount());
```

# 在`Windows`下解决范围问题

## 简单方法

如果要产生32位伪随机数，可以使用

```c
int rand32()
{
	return (rand()<<16)+rand();
}
```

### 问题

- 得到的取值范围**不均匀**，如`rand32()&(1<<15)`总是为0

- 需要生成范围需要手工计算，设生成区间$[l,r]$之间的伪随机数

  ```c
  int r=rand()%(r-l+1)+l;
  ```

- 由于需要多次计算会影响效率

## 使用C++11的伪随机数

> C++11在CCF提供的环境下能基本使用，但不能以源代码提交

### 实现

C++11的随机数定义在`<random>`中，主要有2个概念

- 随机数引擎
  - `default_random_engine`：默认随机数引擎
  - `random_device`：随机数设备，类似于`rand()`
- 随机数范围
  - `uniform_int_distribution`：均匀分布的整数范围
  - `uniform_real_distribution`：均匀分布的实数范围
  - ` bernoulli_distribution`：按概率生成的布尔型

### 范例

```cpp
default_random_engine gen(time(NULL));
uniform_int_distribution<> d1(-233,123456789);
cout<<d1(gen)<<endl;
//output in [-233,123456789]
uniform_int_distribution<long long> d2(numeric_limits<long long>::min(),numeric_limits<long long>::max());
cout<<d2(gen)<<endl;
//output any number in long long
```

# 参考文献

1. <http://en.cppreference.com/w/cpp/numeric/random>
2. <https://msdn.microsoft.com/en-us/library/windows/desktop/ms644904(v=vs.85).aspx>;<https://msdn.microsoft.com/en-us/library/windows/desktop/ms644905(v=vs.85).aspx>
3. <https://msdn.microsoft.com/en-us/library/windows/desktop/ms724408(v=vs.85).aspx>