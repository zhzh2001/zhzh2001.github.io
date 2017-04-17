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