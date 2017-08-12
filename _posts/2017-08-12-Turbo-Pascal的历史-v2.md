---
layout: post
tags: 历史
title: Turbo Pascal的历史 v2
---

# 前言

## 关于Pascal

Pascal已经离我们很远了，将要退出NOI的支持。实际上，现在OI已经很少用Pascal了，常规的都是C++。但是Pascal更容易学习和阅读，尽管标准提供的功能并不丰富。

## 关于Turbo Pascal

Turbo Pascal是我最早用的IDE，在Windows的时代，它已经相当古老了。现在虽然有Free Pascal，但是其稳定性和编译速度比不上TP。而GNU Pascal早已停止更新，虽然还能找到，但是相当难用。

TP的中文资料并不丰富，但是在`bitsavers.org`上有很多文档，在`winworldpc`可以获得各种版本的TP，这也成为了研究的基础。TP可以在CP/M-80,CP/M-86和DOS下使用，我只研究DOS的版本，毕竟CP/M更加古老。然而，在我用的win64平台上，根本不能运行16位的TP，所以我还需要PCem——一个PC模拟器。

本文以描述功能为主，并加上一些图片，避免涉及过多的代码。另外，本文第一版可以在`[OLD]2017-04-09-Turbo-Pascal的历史.md`中找到。

# Turbo Pascal与标准Pascal

## 不支持的标准特性

TP由于某些原因，不支持部分标准Pascal的特性：

- *New*过程不能用于变体记录，应该使用标准过程*GetMem*。
- 标准过程*Get*和*Put*没有实现，因为完全可以用*Read*和*Write*代替，而且更快。
- **goto**语句不能离开当前程序块(当前过程、函数或主程序)。
- 标准过程*Page*没有实现，因为CP/M没有对应功能。
- 关键字**packed**没有实际作用，因为只要有可能，压缩就会自动进行；标准过程*Pack*和*Unpack*没有实现。
- 过程或函数不能作为参数传递。

## 新增功能

### 绝对地址变量

关键字**absolute**可用于变量定义后，用于指定变量在内存中的地址。

```pascal
var
Abc: Integer absolute $0000:$00EE;
Def: Integer absolute $0000:$00F0;
Special: array[l .. CodeSize] absolute CSeg:$05F3;
```

地址的第一部分代表段地址，第二部分代表偏移地址。用CSeg表示代码段，DSeg表示数据段。

另外，**absolute**也可以指定一个变量在另一个的地址处。

```pascal
var
Str: string[32];
StrLen: Byte absolute Str;
```

### 位/字节封装

关键字**shl**,**shr**和**xor**分别表示左移、右移和异或，在标准Pascal中没有定义。

### 直接访问内存和数据端口

数组*Mem*和*MemW*用来直接访问内存，地址格式和“绝对地址变量”一样。前者类型为*Byte*，后者为*Integer*，且为小端表示。这两个数组可以被读取和写入，和普通数组一样。

数组*Port*和*PortW*用来访问8086/88的数据端口。对数组元素的赋值和引用只能在表达式中，不能将数组元素作为变量参数传递。

### 动态字符串

TP中可以用**string**定义长度不超过255的动态字符串，并且支持+连接字符串，直接赋值，以及大小比较等。并且定义了*Delete*,*Insert*,*Str*,*Val*等过程和*Copy*,*Concat*,*Length*,*Pos*等函数。使用比字符数组方便，但是有长度限制。

另外，字符串下标0处保存了其长度对应的*Char*类型值，可以用*Ord*读取长度，用*Chr*修改。

### 定义部分可以随意交换和重复

### 操作系统功能的完全支持

与具体版本有关，较新的版本可以直接调用有关过程和函数，而旧版本需要手动调用操作系统功能。

### 内联机器代码生成

用**inline**关键字可以插入一段机器代码，能自由访问寄存器，但是在退出前必须确保BP,SP,DS和SS与进入时相同。

```pascal
procedure UpperCase(var Strg: Str); {Str is type string[255] }
begin
inline
($C4/$BE/Strg/     {      LES DI,Strg[BP]           }
 $26/$8A/$0D/      {      MOV CL,FS:[DI]            }
 $FE/$Cl/          {      INC CL                    }
 $FE/$C9/          { L1:  DEC CL                    }
 $74/$13/          {      JZ L2                     }
 $47/              {      INC DI                    }
 $26/$80/$3D/$61/  {      CMP ES:BYTE PTR [DI], 'a' }
 $72/$F5/          {      JB L1                     }
 $26/$80/$3D/$7A/  {      CMP FS:BYTE PTR [DI], 'z' }
 $77/$EF/          {      JA L1                     }
 $26/$80/$2D/$20/  {      SUB FS:BYTE PTR [DI], 20H }
 $EB/$E9);         {      JMP SHORT L1              }
                   { L2:                            }
end;
```

### 包含文件

TP允许在一份代码中包含另一份代码，用`{$I filename}`来实现，如果没有扩展名，假设为.PAS。包含文件功能与C预处理指令`#include`大致相同，但是TP不允许嵌套包含文件。

### 将逻辑运算符运用到整数

逻辑运算符**not**,**and**,**or**,**xor**也可以运用到整数类型，即按位取反、按位与、按位或、按位异或。

### 支持共享变量的程序链

TP提供了*Chain*和*Execute*过程来调用其他程序。*Chain*只能调用用`Chn-file`选项编译的扩展名为.CHN的程序。这样的文件只包含程序代码，不包含Pascal库。*Execute*则能调用任何可执行文件，但是不能传递参数。

使用*Chain*支持共享绝对变量和全局变量，但是需要进行特殊处理：

- 共享的全局变量必须在一开始定义，且顺序必须相同；
- 程序编译时的数据段和代码段必须相同。

#### 程序`MAIN.COM`

```pascal
program Main;
var
  Txt:string[80];
  CntPrg:file;
begin
  Write('Enter any text: '); Readln(Txt);
  Assign(CntPrg, 'ChrCount.chn' );
  Chain(CntPrg);
end.
```

#### 程序`CHRCOUNT.CHN`

```pascal
program ChrCount;
var
  Txt: string[80];
  NoOfChar,
  NoOfUpc,
  I: Integer;
begin
  NoOfUpc := 0;
  NoOfChar := Length(Txt);
  for I := 1 to Iength(Txt) do
    if Txt[I] in ['A' .. 'Z'] then NoOfUpc := Succ(NoOfUpc);
  Write( 'No of characters in entry: ',NoOfChar);
  Writeln( '. No of upper case characters: ', NoOfUpc,'.');
end.
```

### 随机访问数据文件

使用*Seek(FilVar, n)*将文件指针移动到第n条记录，编号从0开始。

### 结构化常量

常量可以指定类型，包括非结构化类型和数组、多维数组、记录、集合等类型。

### 类型转换函数

可以用强制类型转换函数，但是浮点数不支持。

### 新的动态内存分配

TP1引入了*Mark*和*Release*来代替*Dispose*。*Mark(Var)*将堆指针保存到*Var*，而*Release(Var)*则将堆指针赋值为*Var*。这样*Release*就能释放从*Var*起的所有空间，比*New/Delete*更加高效。

![](/img/dispose_vs_markrelease.png)

# 版本比较

## 1.0

1.0没有DOS版本，并且找不到有关文档。

## 2.0

![](/img/tp2.png)

![](/img/tp2ui.png)

![](/img/tp2man.png)

### 文件

TP2非常简单，其实只要TURBO.COM就可以使用了，而TURBO.MSG提供了错误消息。两个文件还不到40KB。剩下的文件并不是必须的，尤其对于IBM PC而言。

### 使用

2.0的用户界面很奇怪，但还是可以用的。在主菜单，按下高亮的字母可以选择对应的功能。如果要打开或新建一个文件，需要：

1. L切换到需要的驱动器
2. A改变当前目录
3. W输入文件名
4. E进入编辑模式
5. ^KD退出编辑模式
6. S保存
7. C编译
8. R运行

#### ART.PAS

以下为运行ART.PAS的效果：

![](/img/tp2art.png)

注意，默认情况下，TP2把程序编译到内存中，因此可以直接运行。如果需要生成可执行文件，需要修改编译选项为Com-file。编译ART.PAS生成的ART.COM只有11,518字节。

#### 一个递归测试

TP2的编辑模式没有代码高亮功能。默认情况下，递归了8,127层，然后运行时错误FF，并给出了PC。如果编译到内存，会自动定位RE位置。

如果编译到文件，只要记录下PC，然后在选项中选择F即可。

当然，TP2还没有调试功能，只有简单的找到RE位置的功能。

```pascal
program recursion;
var
  maxdep:integer;

procedure recur(x:integer);
begin
  if x<=maxdep then
  begin
    writeln(x);
    recur(x+1);
  end;
end;

begin
  writeln('hello, world');
  read(maxdep);
  recur(1);
end.
```

### 外部子程序

TP中可以用**external**声明外部子程序，其必须是可重定位的。

```pascal
procedure Plot(X,Y: Integer); external 'PLOT';
procedure QuickSort(var List: PartNo); external 'QS';
```

### DOS功能调用

在TP2中，没有直接提供DOS接口，使用*MsDos*过程调用DOS功能。下面的代码提供了一种获取时间的方法，可以放到一个包含文件中。其他功能调用参见DOS参考手册，或者用较新的TP。

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

```

### 覆盖功能(**overlay**)

覆盖功能可以把一个程序的多个功能模块加载到内存中的同一个位置，以扩大程序的规模。只要在过程或函数声明前加入**overlay**关键字。

同一区域内的覆盖函数或过程不能相互调用，也不能使用**forward**定义，不能递归调用。总之在一个时刻，一个覆盖组内只能运行一个覆盖函数或过程。

注意，覆盖文件保存在磁盘上，因此频繁调用覆盖会降低效率，应该权衡使用这个功能。下面用文档中的插图来形象化理解。

![](/img/overlay1.png)

![](/img/overlay2.png)

![](/img/overlay3.png)

### IBM PC的附加功能

允许切换文本/图形模式，设置颜色，绘制窗口，以及使用声音等高级功能。这里不详细展开，可以参考示例程序。

### Turbo-87 Pascal

这个版本的TP2无法找到。根据文档，这个版本必须有8087浮点协处理器才能使用，并且用64位的实数代替默认的(其实就是`double`)，具有更高的精度和更快的速度。

## 3.0

![](/img/tp3disk.jpg)

![](/img/tp3.png)

![](/img/tp3man.png)

### 文件

TP3的文件比2.0稍微大一些。我找到的是带有BCD和8087的版本，分别为TURBOBCD.COM和TURBO-87.COM。

