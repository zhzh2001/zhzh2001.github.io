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