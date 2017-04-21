---
layout: default
---

## 前言

> 为什么要写这篇文章？

1. Pascal将进一步被弃用，从2020年起除NOIp，2022年起所有NOI竞赛将不能使用Pascal。实际上，现在使用Pascal/Delphi开发应用程序已经非常少见了。谨以此文来纪念Pascal。
2. 我喜欢研究软件的历史，当别人正研究Windows的历史时，我决定研究一下曾经著名的Borland Turbo Pascal。
3. 同时介绍一下DOS以及PC模拟器PCem。

### 实验平台

| 项目     | 版本号        | 备注     |
| ------ | ---------- | ------ |
| 操作系统   | Windows 10 | 1703   |
| PCem   | v12        |        |
| 模拟PC   | IBM AT     | 286/6  |
| 显卡     | VGA        |        |
| 内存     | 2MB        | 需要时可增加 |
| 硬盘     | 31MB       |        |
| MS-DOS | 5.0        |        |

## Turbo Pascal 概述

> 以下翻译自《Turbo Pascal 2.0 Reference Manual》

### Pascal语言

Pascal是一种通用高级语言，最初由Niklaus Wirth教授设计。名称来自于法国17世纪的哲学家和数学家Blaise Pascal。

Wirth教授对于Pascal语言的定义，与1971年公开，是为了帮助程序设计的教学，尤其是为了介绍*结构化程序设计*。Pascal从此被用于在几乎任何计算机上完成几乎任何任务。Pascal在今天不管应用在教育还是专业编程上，都是最著名的高级语言之一。

### TURBO Pascal

TURBO Pascal是为满足各个领域的用户而设计的：它为学生提供一个友好的交互环境，能帮助学习进程；在程序员手中它成为了非常高效的开发工具，同时提供编译和运行。

TURBO Pascal接近由K.Jensen和N.Wirth在*Pascal User Manual and Report*中定义的标准Pascal。它提供一些扩展功能，包括：

- 绝对地址变量
- 直接访问CPU，内存和数据端口
- 动态字符串
- 在程序定义部分随意交换顺序
- 操作系统设备的完全支持
- 内嵌机器代码
- include文件支持
- 整数的逻辑运算
- 随机访问数据文件
- 结构化常量
- 类型转换函数

> 以下翻译自winworldpc

Borland Turbo Pascal是一个带有IDE的Pascal语言编译器，定位于爱好者和入门市场。有CP/M和DOS的版本。有一段时间，它和他们的专业的"Borland Pascal"产品线一起卖。在他们的早期版本中，Borland也提供了一些"工具箱"库。

## 开始前的准备

1. 下载`PCem`以及所需的rom，可以搜索`PCem roms`来找到，我用的是一个v11的rom包。把能找到的rom放在/roms下。
2. 下载`DOS`安装盘，我使用的是`MS-DOS 5.0`。
3. 启动`PCem`，配置硬件，创建硬盘镜像，并安装`DOS`。
4. 熟悉`DOS`环境，最好把时间调整到现在。

### 关于PCem的硬件

- 机器类型：最好286+，否则键盘可能有兼容性问题。


- 显卡：可以用CGA/EGA，VGA也是可以的。
- 内存：不要太多，过多的内存将使开机自检时间很长，`DOS`可能也无法充分利用大内存。
- 声卡：可以不用。
- 软盘：最好按照安装镜像大小调整，不调整有时也能正常工作。

### 关于DOS

- 版本：2.0+，更高的版本使用更加方便，但空间占用更多。
- 从软盘启动和安装到硬盘都可以，我安装到硬盘。
- 使用5.0不要开机启动shell。

## Turbo Pascal 1.0

目前找不到1.0的DOS版本，只有CP/M-80和CP/M-86版，而且找不到用户手册/参考手册，因此没有详细研究。

## Turbo Pascal 2.0(1984)

### 来源

软件：1张5.25英寸软盘，180KB(来自winworldpc)

手册：Reference Manual(来自bitsavers.org)

### 新功能

> 来自参考手册附录P5

- 覆盖支持
- 实现了Dispose,MaxAvail,FreeMem来管理堆
- 增加一些屏幕绘图过程
- 增加颜色常量

### 与标准Pascal的差异

- 动态变量
  - 推荐使用Mark&Release代替Dispose。
  - New不能接受*变体记录*，需要使用GetMem。
- 没有实现Get/Put，因为完全可以用Read/Write实现，而且更加高效。
- goto语句不能离开当前代码块。
- 没有实现Page，因为CP/M没有有关的字符。
- 压缩变量
  - 关键字**packed**在`TURBO Pascal`中没有作用，但是还是允许的。
  - 没有实现Pack/Unpack，因为只要可能，压缩变量就会自动进行。
- 过程和函数不能被当作参数传送。

### 示例功能

#### include

##### /SRC/TP2/TOUPPER.PAS

```pascal
type
  AnyString=string[255];
//字符串没有默认长度
function toupper(s:Anystring):Anystring;
var
  i:integer;
begin
  for i:=1 to length(s) do
    s[i]:=upcase(s[i]);
  toupper:=s;
end;
```

##### /SRC/TP2/INCL.PAS

```pascal
program include_example;
var
  s:string[50];
{$I TOUPPER.PAS}
begin
  readln(s);
  writeln(toupper(s));
end.
```

以上代码演示了`Turbo Pascal`增强功能：使用`{$I}`编译指令来加入源文件。可以部分实现*单元*的功能，但每次必须全部重新编译。

类似于C的`#include<>`预处理指令。

#### 获取可用内存

##### /SRC/TP2/MEM.PAS

```pascal
program memory;
var
  cnt:real;
begin
  cnt:=MaxAvail;
  //返回值为integer类型，单位为段(16字节)
  if cnt<0 then
    cnt:=cnt+65536.0;
  //如果可用内存超过32767*16=~0.5MB，将返回负数
  //8088寻址空间为1MB
  writeln('Free memory:',cnt*16.0:0:0,' bytes');
  readln;
end.
```

也许你会觉得特别奇怪：`MaxAvail`为什么不直接返回可用字节数，返回类型为32位整数呢？原因很简单，**当时没有32位整数**，也没有16位无符号整数。

实际上，16位的8086/8088只能寻址64KB，但因为使用了段寻址而增加到1MB。因此一段自然是16字节了。

##### 结果

在我的环境下，编译到内存可用内存接近540KB，编译到文件接近580KB，相差的40KB应该是IDE占用的。

即使把最大动态内存改为1MB，实际可用的也不到600KB，这是为什么呢？

因为硬件做了手脚：就像今天大多数32位操作系统不能完全使用4GB内存一样，当时`DOS`下最多可以访问的内存也只有640KB，被称为*常规内存*。多出来的384KB为硬件保留，也为以后的`EMS`提供了可行方案。

而640-580=60KB内存则被`DOS`和程序可执行文件占用。

##### 有关内存的讨论

看到这里，你应该知道`Turbo Pascal`能使用的内存非常少的原因。其实还有更多的限制，即使是常规内存也不能直接寻址。原因很简单，8088简单寻址只有64KB，因此2.0有以下限制：

- 代码段不能超过64KB
- 数据段不能超过64KB
- 栈大小不能超过64KB
- 可执行文件不能超过64KB

#### 动态内存分配

##### /SRC/TP2/MEMALLOC.PAS

```pascal
program memalloc;
var
  sz:integer;
  p:^integer;
  cnt:real;
begin
  read(sz);
  GetMem(p,sz);
  //获取sz字节的内存
  
  cnt:=MaxAvail;
  if cnt<0 then
    cnt:=cnt+65536.0;
  writeln('Free memory:',cnt*16.0:0:0,' bytes');
  readln;
end.
```

`GetMem`的第二个参数是integer类型，因此实际分配的字节数等于与其等价的无符号整数。例如-1可以获取64KB内存，这也是单次允许分配的最大值。

综合静态、动态内存分配，结论是2.0中单个数组的最大空间不能超过64KB。那么如何使用更多的内存呢？

1. 将待处理的数据分为几段，每段不超过64KB，分段存储操作。
2. 使用new/dispose分配动态数据结构，如链表、树等。

##### 内存不足时的错误

如果可用内存不足以分配请求的大小，会发生什么？

为了实现这一点，我们选择编译到文件，并把最大动态内存改为小于64KB，如0400，然后进行实验。

- 输入16000，可用空间只有112字节了，好神奇！
- 输入-1，产生运行时错误FF(堆/栈溢出)，PC=2853

###### 找出运行时错误位置

在主菜单下输入OF2853，并按下ESC，找到错误发生位置：

第8行：`GetMem(p,sz);`

相对来说还是很麻烦的：

- 退出IDE
- 运行程序并输入
- 记录RE代码和PC
- 查阅RE原因
- 启动IDE
- 输入PC查找错误位置

不过在当时应该算很先进了。

#### 覆盖

覆盖功能能使不在一起使用的代码在使用时加载到内存中的同一区域，以节省内存，允许运行超过可用内存的程序。

在函数或过程前加上**overlay**关键字即可实现覆盖功能，同一区域内的覆盖函数或过程不能相互调用，也不能使用**forward**定义，不能递归调用。总之在一个时刻，只能运行一个覆盖函数或过程。

##### /SRC/TP2/PRIME.PAS

```pascal
program generate_prime;
{$I TIMER.PAS}
const
  check_maxn=1000;
var
  n:integer;
  t1,t2,dur:timeRec;
overlay procedure check(n:integer);
var
  i,ans:integer;

  function prime(n:integer):boolean;
  var
    i:integer;
    flag:boolean;
  begin
    i:=2;
    flag:=true;
    while flag and (i<=trunc(sqrt(n))) do
    begin
      if n mod i=0 then
        flag:=false;
      i:=i+1;
    end;
    prime:=flag;
  end;

begin
  ans:=0;
  for i:=2 to n do
    if prime(i) then
      ans:=ans+1;
  writeln(ans);
end;
overlay procedure eratos(n:integer);
var
  bl:array[2..MaxInt]of boolean;
  i,j,ans:Integer;
begin
  fillchar(bl,sizeof(bl),true);
  for i:=2 to trunc(sqrt(n)) do
    if bl[i] then
      for j:=2 to n div i do
        bl[i*j]:=false;
  ans:=0;
  for i:=2 to n do
    if bl[i] then
      ans:=ans+1;
  writeln(ans);
end;
overlay procedure eratos_plus(n:integer);
const
  maxn=2048;
var
  bl:array[0..maxn]of integer;
  i,j,ans:Integer;
begin
  fillchar(bl,sizeof(bl),$ff);
  for i:=2 to trunc(sqrt(n)) do
    if(bl[i shr 4] and (1 shl (i and 15)))<>0 then
    begin
      j:=i*i;
      while (j>0)and(j<=n) do
      begin
        bl[j shr 4]:=bl[j shr 4] and not(1 shl (j and 15));
        j:=j+i;
      end;
    end;
  ans:=0;
  for i:=2 to n do
    if(bl[i shr 4] and (1 shl (i and 15)))<>0 then
      ans:=ans+1;
  writeln(ans);
end;
overlay procedure euler(n:integer);
const
  maxn=2048;
  maxpn=4000;
var
  bl:array[0..maxn]of integer;
  p:array[1..maxpn]of integer;
  i,j,ans:Integer;
  flag:boolean;
begin
  ans:=0;
  fillchar(bl,sizeof(bl),$ff);
  for i:=2 to n do
  begin
    if(bl[i shr 4] and (1 shl (i and 15)))<>0 then
    begin
      ans:=ans+1;
      p[ans]:=i;
    end;
    j:=1;
    flag:=true;
    while flag and(j<=ans)and(i*p[j]>0)and(i*p[j]<=n) do
    begin
      bl[(i*p[j]) shr 4]:=bl[(i*p[j])shr 4] and not (1 shl ((i*p[j])and 15));
      flag:=i mod p[j] <> 0;
      j:=j+1;
    end;
  end;
  writeln(ans);
end;
begin
  read(n);

  if n<=check_maxn then
  begin
    getTime(t1);
    check(n);
    getTime(t2);
    diffTime(t1,t2,dur);
    writeln('using check:',dur.h,':',dur.m,':',dur.s,'.',dur.s100);
  end else
    writeln(n,' is too big to check');

  getTime(t1);
  eratos(n);
  getTime(t2);
  diffTime(t1,t2,dur);
  writeln('using eratos:',dur.h,':',dur.m,':',dur.s,'.',dur.s100);

  getTime(t1);
  eratos_plus(n);
  getTime(t2);
  diffTime(t1,t2,dur);
  writeln('using eratos plus:',dur.h,':',dur.m,':',dur.s,'.',dur.s100);

  getTime(t1);
  euler(n);
  getTime(t2);
  diffTime(t1,t2,dur);
  writeln('using euler plus:',dur.h,':',dur.m,':',dur.s,'.',dur.s100);
  writeln('done');
  readln;
end.
```

测试各种生成质数的方法：

###### IBM AT(286/6)

| 名称       | 理论时间复杂度         | n=1000 | n=32767 |
| -------- | --------------- | ------ | ------- |
| 逐个判断质数   | $$O(n\sqrt n)$$ | 46.57s | -       |
| 埃氏筛法     | $$O(n \log n)$$ | 0.11s  | 1.59s   |
| 埃氏筛法位运算版 | $$O(n \log n)$$ | 0.11s  | 3.63s   |
| 欧拉筛法位运算版 | $$O(n)$$        | 0.22s  | 5.66s   |

###### IBM XT(8088/4.77)

| 名称       | n=255  | n=32767 |
| -------- | ------ | ------- |
| 逐个判断质数   | 21.26s | -       |
| 埃氏筛法     | 0.22s  | 5.6s    |
| 埃氏筛法位运算版 | 0.11s  | 9.94s   |
| 欧拉筛法位运算版 | 0.17s  | 20.49s  |

#### 测量时间

2.0没有提供`DOS`系统调用的封装，必须手动调用。

##### /SRC/TP2/TIMER.PAS

```pascal
type
  regfile=record
    AX,BX,CX,DX,BP,SI,DI,DS,ES,Flags:integer;
  end;
  timeRec=record
    h,m,s,s100:integer;
  end;

procedure getTime(var t:timeRec);
var
  reg:regfile;
begin
  reg.AX:=$2C shl 8;
  MsDos(reg);
  t.h:=reg.CX shr 8;
  t.m:=reg.CX and 255;
  t.s:=reg.DX shr 8;
  t.s100:=reg.DX and 255;
end;

procedure diffTime(t1,t2:timeRec;var res:timeRec);
var
  under:integer;
begin
  res.s100:=t2.s100-t1.s100;
  under:=0;
  if res.s100<0 then
  begin
    under:=1;
    res.s100:=res.s100+100;
  end;

  res.s:=t2.s-t1.s-under;
  under:=0;
  if res.s<0 then
  begin
    under:=1;
    res.s:=res.s+60;
  end;

  res.m:=t2.m-t1.m-under;
  under:=0;
  if res.m<0 then
  begin
    under:=1;
    res.m:=res.m+60;
  end;

  res.h:=t2.h-t1.h-under;
  if res.h<0 then
  begin
    writeln('Warning:t1>t2!Unable to determinate actual differece!');
    res.h:=res.h+24;
  end;
end;
```

查阅`DOS`参考手册可知，获取系统时间的系统调用为0x2c，流程如下：

```assembly
;以下为伪代码
mov $2c,ah
int $21
mov ch,hour;0~23
mov cl,min;0~59
mov dh,sec;0~59
mov dl,sec100;0~99
```

hour:min:sec.sec100对应与时:分:秒.百分之一秒，我将这些封装到一个记录`timeRec`中，并提供获取时间、计算时间差的功能，只需要`{$I TIMER.PAS}`即可使用这些功能。

##### /SRC/SORTALGO.PAS

```pascal
program sort_algo;
{$I TIMER.PAS}
const
  maxn=10000;
var
  n:integer;
  a,b,tmp:array[1..maxn]of integer;
  t1,t2,dur:timeRec;
procedure init;
var
  i:integer;
begin
  for i:=1 to n do
    a[i]:=random(MaxInt);
end;
procedure qsort(l,r:integer);
var
  i,j,x,t:integer;
begin
  i:=l;j:=r;
  x:=b[l+(r-l) div 2];
  repeat
    while b[i]<x do i:=i+1;
    while x<b[j] do j:=j-1;
    if i<=j then
    begin
      t:=b[i];b[i]:=b[j];b[j]:=t;
      i:=i+1;j:=j-1;
    end;
  until i>j;
  if l<j then qsort(l,j);
  if i<r then qsort(i,r);
end;
procedure mergesort(l,r:integer);
var
  i,j,mid,p:integer;
begin
  if l<r then
  begin
    mid:=l+(r-l) div 2;
    mergesort(l,mid);
    mergesort(mid+1,r);
    i:=l;j:=mid+1;
    p:=1;
    while (i<=mid) and (j<=r) do
      if b[i]<b[j] then
      begin
        tmp[p]:=b[i];
        p:=p+1;
        i:=i+1;
      end else
      begin
        tmp[p]:=b[j];
        p:=p+1;
        j:=j+1;
      end;
    while i<=mid do
    begin
      tmp[p]:=b[i];
      p:=p+1;
      i:=i+1;
    end;
    while j<=r do
    begin
      tmp[p]:=b[j];
      p:=p+1;
      j:=j+1;
    end;
    for i:=l to r do
      b[i]:=tmp[i-l+1];
  end;
end;
begin
  randomize;
  read(n);
  init;
  writeln('inited');

  move(a,b,n*2);
  getTime(t1);
  qsort(1,n);
  getTime(t2);
  diffTime(t1,t2,dur);
  writeln('quicksort:',dur.h,':',dur.m,':',dur.s,'.',dur.s100);

  move(a,b,n*2);
  getTime(t1);
  mergesort(1,n);
  getTime(t2);
  diffTime(t1,t2,dur);
  writeln('mergesort:',dur.h,':',dur.m,':',dur.s,'.',dur.s100);
end.
```

另一个用于比较排序算法的程序。

快排(平均)和归并排序理论时间复杂度均为$$O(n\log n)$$，但是同样规模归并排序运行时间为快排的2倍多。否则怎么大家都用快排呢？

这里没有用**overlay**，因为两个主要过程都是递归调用的。

## Turbo Pascal 3.0(1985)

### 来源

软件：3.01A with 8087-BCD，1张360KB软盘

手册：Reference Manual 1985/1986(建议阅读1986年的版本，更加详细)

### 新功能

> 几乎没有新功能，只有手册不同

- 将2.0参考手册中附录部分移动到正文中
- 提供`TURBO-BCD`版本，使用十进制表示浮点数，能提高精度，但降低效率

### 示例功能

#### Turbo-87

