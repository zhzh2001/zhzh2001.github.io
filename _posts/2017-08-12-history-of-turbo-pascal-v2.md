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

 <!-- more -->

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

## 1.0(1983)

![](/img/tp1.png)

![](/img/tp1ui.png)

![](/img/tp1man.png)

### 官方介绍

![](http://edn.embarcadero.com/article/images/20693/tp1sm.gif)

官方下载：<http://altd.embarcadero.com/download/museum/tp1.zip>

发布日期：1983/11/20

#### 价格：$$\$49.95+\$5$$ 运费

#### 系统要求

- Intel 8086和Zilog Z-80微处理器
- 64KB内存
- 对于CP/M和DOS系统，5¼ "软盘
- 对于CP/M系统，8"软盘

#### 文件列表

TP1.0在单张软盘上发行，共10个文件，131,297字节。TURBO.COM(包含带有编译器的IDE，Wordstar风格的编辑器和在内存中运行的系统)大小为33,280字节。

- TURBO.COM - 编译器和编辑器
- TURBOMSG.OVR - 错误信息文件
- TINST.COM - IDE安装程序
- TINSTMSG.OVR - TINST程序的错误信息
- TLIST.COM - 打印程序
- ERROR.DOC - 手册附加内容
- CALC.PAS - MicroCalc表格示例程序
- CALCMAIN.PAS - 示例包含功能
- CALC.HLP - MicroCalc帮助手册
- CALCDEMO.MCS - 示例表格定义

### 使用

TP1还不支持目录操作，只能使用当前目录。另外还有一个没有实现的运行功能，输入X提示该功能在8086/88上暂未实现，只支持CP/M-80。

实际上，TP2的手册附录P前面的就是TP1的手册，据说更早时TP2直接提供TP1的手册加上附录P。

关于用户界面的使用参见TP2。

### 外部子程序

TP中可以用**external**声明外部子程序，其必须是可重定位的。

```pascal
procedure Plot(X,Y: Integer); external 'PLOT';
procedure QuickSort(var List: PartNo); external 'QS';
```

### DOS功能调用

在早期的TP中，没有直接提供DOS接口，使用*MsDos*过程调用DOS功能。下面的代码提供了一种获取时间的方法，可以放到一个包含文件中。其他功能调用参见DOS参考手册，或者用较新的TP。

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

## 2.0(1984)

![](/img/tp2.png)

![](/img/tp2ui.png)

![](/img/tp2man.png)

### 使用

虽然在手册中没有提到，用户界面已经可以切换目录了。

用户界面很奇怪，但还是可以用的。在主菜单，按下高亮的字母可以选择对应的功能。如果要打开或新建一个文件，需要：

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

编辑模式没有代码高亮功能。默认情况下，递归了8,127层，然后运行时错误FF，并给出了PC。如果编译到内存，会自动定位RE位置。

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

## 3.0(1985~1986)

![](/img/tp3disk.jpg)

![](/img/tp3.png)

![](/img/tp3man.png)

### 官方介绍(3.02)

官方下载：<http://altd.embarcadero.com/download/museum/tp302.zip>

发布日期：1986/9/17

价格：$$\$99.95$$(包含8087和BCD)；CP/M-80版本(不包含8087和BCD)：$$\$69.95$$

#### 系统要求

16位系统

- 128KB内存(包含8087和BCD)
- MS-DOS 2.0和CP/M-86 1.1

8位系统

- 48KB内存(不包含8087和BCD)
- CP/M-80 2.2

### 文件

TP3的文件比2.0稍微大一些。我找到的是带有BCD和8087的版本，分别为TURBOBCD.COM和TURBO-87.COM。

### 速度改进

2.0和3.0都提供了一个很大的示例CALC.PAS，有30+KB。下面是一个估测比较(IBM AT 286/6)：

|      | 加载时间 | 编译到内存 | 编译到文件 |
| ---- | ---- | ----- | ----- |
| 2.0  | 2s   | 8s    | 10s   |
| 3.0  | <1s  | 4s    | 4s    |

可以看出有不少改进。

### 海龟图形(Turtlegraphics)

海龟图形基于“海龟”的概念，“海龟”可以向特定角度移动特定距离，并绘制图形。海龟图形需要在窗口中操作。可以参考示例TURTLE.PAS。注意，编译时同一目录下应该有GRAPH.P和GRAPH.BIN文件。

![](/img/tp3turtle.png)

### 目录操作

提供了*ChDir*,*MkDir*,*RmDir*和*GetDir*来实现目录相关的操作，前三个过程和DOS一样，*GetDir(Drive,Str)*获取*Drive*的当前目录到*Str*。*Drive*是一个整数，0表示当前目录，1表示A，……可以参考DIRECT.PAS。

### 获取命令行参数

提供了*ParamCount*和*ParamStr*来获取命令行参数，其实与C的*argc*和*argv*一样。可以参考示例CMDLIN.PAS。

### Turbo-BCD

Turbo-BCD利用BCD来存储浮点数，以避免十进制的误差。例如，十进制0.1在二进制下无法精确表示，但是可以用十进制浮点数(BCD)精确表示。

Turbo-BCD不能使用*Sin*,*Cos*,*ArcTan*,*Ln*,*Exp*,*Sqrt*和*Pi*，不包含这些标识符的Pascal程序都能用Turbo-BCD编译。BCD浮点数的范围为$\vert R\vert\in10^{-63}\dots10^{63}$，有效位数18位，占用10字节空间。BCD浮点数可以用*Form*函数来格式化，具体参见手册。

可以用BCD.PAS来演示Turbo-BCD的优势。使用TURBO.COM编译的结果：

![](/img/tp3bcd1.png)

使用TURBOBCD.COM：

![](/img/tp3bcd.png)

![](/img/tp3bcd2.png)

### Turbo-87

与2.0的基本一样，也需要8087协处理器。我在没有x87的IBM AT(286)上运行，结果是一个无效的浮点数。因此，必须在有数学协处理器的PC上测试。由于PCem暂时没有独立的模拟x87的开关，所以我用486DX，也就是最早内置x87的CPU了。

演示Turbo-87的有HILB.PAS和TEST.PAS，下面使用TEST.PAS，可以比较速度和精度。我包含了我自己写的计时库TIMER.PAS。使用TURBO.COM：

![](/img/tp3x87a.png)

使用TURBO-87.COM：

![](/img/tp3x87.png)

![](/img/tp3x87b.png)

使用以下代码的结果为1.644903548804433，与Turbo-87基本一致：

```c++
#include <iostream>
#include <limits>
using namespace std;
int main()
{
	int n;
	cin >> n;
	double ans = .0;
	for (int i = 1; i <= n; i++)
		ans += 1.0 / (i * i);
	cout.precision(numeric_limits<double>::digits10);
	cout << fixed << ans << endl;
	return 0;
}
```

## 4.0(1987)

![](/img/tp4ui.png)

![](/img/tp4man.png)

### 文件

我找到的是3张360KB软盘的版本，最好把所有文件复制到同一个目录下。可以发现可执行文件已经从.COM变成.EXE了，下面列举部分重要的文件：

- TURBO.EXE：提供IDE，115KB
- TURBO.TPL：标准单元，37KB
- GRAPH.TPU：*Graph*单元，27KB
- TPC.EXE：命令行编译器，42KB
- MAKE.EXE：自动更新修改的程序，19KB

### 使用

TP4已经有菜单了，看起来比前几个版本好多了。但是那个Output窗口很讨厌，按F5可以关掉。切换到菜单可以用Alt+高亮字母，也可以用F10，在PCem中还是用F10比较好，因为前者也会触发PCem的菜单。按ESC就可以退到编辑窗口。

#### ARTY4.PAS

ART.PAS也升级了！

![](/img/tp4com.png)

![](/img/tp4art.png)

编译到文件生成的ARTY4.EXE有24,384字节，比2.0大了很多。

### 升级早期程序

UPGRADE.EXE用于完成这一功能，我用3.0的CALC.PAS试了，原始文件被重命名为CALC.3TP。不过编译升级后的文件也用了4s，没有手册所说的速度提升。不过，MCALC.PAS提供的MicroCalc比以前的高级多了。不但增加了颜色，而且再次编译很快，因为采用了MAKE.EXE。

![](/img/tp3calc.png)

![](/img/tp4calc.png)

### 代码生成优化

我同时也升级了TEST8087.PAS及TIMER.PAS，用*Dos*单元的*GetTime*重写，并把我的*GetTime*重命名为*myGetTime*，这样就可以了。当然，我准备把TIMER.PAS写成单元的格式。

用32767运行，不使用8087，3.0用了42s，4.0用了32.5s，改进还是很明显的。

### 单元和Make

单元是4.0最大的改进，单元中可以提供各种接口。这样就可以把一个很大的程序分成一些单元，可以分别编写和调试，最后连接成最终的程序。当只修改部分文件时，利用Make功能，只需要重新编译修改部分再连接即可。而Build功能则重新编译所有的文件。

单元的结构如下：

```pascal
unit unitname;
interface
{ uses <list of units>; Optional}
{ public declarations }
implementation
{ private declarations }
{ procedures and functions}
begin
{ initialization code}
end.
```

实际上，根据上述结构，单元与C中的头文件、实现文件类似。其中**interface**到**implementation**之间的部分类似于头文件，而**implementation**后的部分实现接口。

TP提供的各种接口也通过单元来提供，使用**uses**来包含其他单元。其中TURBO.TPL中包含了*System*,*Crt*,*Dos*,*Printer*,*Turbo3*和*Graph3*，而GRAPH.TPU中包含了*Graph*单元。

- *System*单元包含了基本的Pascal过程和函数，每个程序默认包含了*System*单元。
- *Dos*单元提供了大部分DOS调用的API，例如*GetTime*,*SetTime*,*DiskSize*等，也提供了旧的*MsDos*和*Intr*。
- *Crt*单元提供了文本界面控制和声音控制等。
- *Printer*单元提供了*Lst*文件来支持打印机。
- *Graph3*单元提供了3.0中的所有图形功能。
- *Graph*单元提供了更多更快更强大的图形功能，包括我用的VGA和更早的CGA,EGA和Hercules等。

由于有了单元，程序链和覆盖功能都不再支持，应该转换到单元。

### 包含文件的嵌套

以前的版本包含文件不能嵌套，TP4可以嵌套不超过8层。

### 新的数据类型

TP4提供了新的整数类型*longint*,*shortint*,*word*，如果有x87协处理器，还可以使用*single*,*double*,*extended*和*comp*。在此之前，TP只提供了16位整数，现在最多可以用64位整数(即*comp*)。

> TP中的64位整数*comp*：很多用过Pascal的OIer都不知道TP也是有64位整数的，*comp*的范围为$-2^{63}+1\dots2^{63}-1$。虽然*comp*是浮点数类型，但是只能保存整数，而且与标准的64位整数相差1。*comp*需要x87协处理器才能使用。

要使用x87，必须打开编译开关`{$N+}`，默认是`${N-}`。开启这个模式后，所有浮点运算都以*extended*精度运算，默认输出格式也改为*extended*的格式。如果没有x87，TP不允许编译`${N+}`的程序。

另外8087使用自己的运算栈，因此写x87的过程和函数应该特别注意。例如以下函数将引起8087栈溢出：

```pascal
function Fib(N: integer): extended;
begin
  if N = 0 then Fib := 0.0 else
  if N = 1 then Fib := 1.0 else
  Fib := Fib(N-1) + Fib(N-2);
end;
```

正确的写法是用临时变量保存`Fib(N-1)`和`Fib(N-2)`。参考FIB8087.PAS。

### 短路的布尔表达式求值

布尔表达式默认是短路的，但是可以通过编译开关来改变。

### 条件编译

提供了编译指令`{$DEFINE name}`,`{$UNDEF name}`,`{$IFDEF name}`,`{$IFNDEF name}`,`{$IFOPT switch}`(判断编译开关),`{$ELSE}`,`{$ENDIF}`来实现条件编译，与C预处理指令类似。另外，如果有x87，就会定义*CPU87*。

### 命令行编译器

TPC.EXE是命令行编译器，使用也很简单，例如`TPC QSORT.PAS`即可编译QSORT.PAS到QSORT.EXE。常用的选项包括/M(Make),/B(Build),/Q(Quiet),/F(Find),/R(内存中运行),/X(运行)，其中/F就等价于以前的“找运行时错误”功能。

## 5.0(1988)

![](/img/tp5ui.png)

![](/img/tp5man.png)

### 安装

这是第一个需要安装的版本，我用的是2张720KB软盘的版本。INSTALL.EXE用于安装，安装过程不仅包括复制文件，还需要解压缩.ARC，因此最好用安装程序。

![](/img/tp5inst.png)

### 使用

用户界面与4.0相差较大，蓝色占了大部分面积，菜单也更加丰富了。不过Load和Save不在快捷键列表中显示比较遗憾，但还是可以用的。现在Output用经典快捷键Alt+F5，全屏显示。相比4.0主要增加了调试功能。另外，运行MicroCalc时显示的可用内存增加了很多(都在内存中运行)。

![](/img/tp5calc.png)

### 源代码级别的调试

TP5终于在IDE中支持基本的调试功能了，使用也很简单。用户手册中提供了以下例程：

```pascal
{$D+,L+} { Just to be sure complete debug information is being generated }
{$R-}                            { Just to be sure range-checking is off }
program RangeTest;
var
  List : array[1 .. 10] of integer;
  Indx : integer;
begin
  for Indx := 1 to 10 do
    List[Indx] := Indx;
  Indx := 0;
  while (Indx < 11) do
  begin
    Indx := Indx + 1;
    if List[Indx] > 0 then
      List[Indx] := -List [Indx]
  end;
  for Indx := 1 to 10 do
    Writeln(List[Indx])
end.
```

这段程序是死循环的，原因并不清晰：因为List[11]的位置就是Indx，因此接下来把Indx设置为-11，造成了死循环。然而，如果不知道TP的内存分配机制，可能很难想到这个错误。

而使用调试功能，很容易发现这个问题。这要把List和Indx加入Watch，然后F8单步执行，当Indx=11的时候，就显示了上述行为的发生。如果打开范围检查呢(`{$R+}`)？在修改前先停止调试，即Run-Program reset。这时就报告错误201了，这相信大家都很熟悉。不过需要注意的是，TP5默认不开启范围检查，需要手动改选项或加上编译开关`{$R+}`。

### 完整的8087浮点数模拟

默认开启了编译开关`{$E+}`来允许使用软件模拟8087浮点数运算，这样程序可以在具有x87的PC上使用x87指令，而在没有协处理器的PC上使用软件模拟。

### 基于单元的覆盖功能

在传统的覆盖功能被4.0抛弃后，5.0引入了基于单元的覆盖功能，可以直接把单元作为覆盖的内容。需要使用编译开关`${O+}`来允许一个单元使用覆盖功能，用`${F+}`强制所有调用为FAR。只要在需要覆盖的单元中加入上述两个编译开关，在主程序中加入`${F+}`即可。

在主程序中，必须先包含*Overlay*单元，之后用`{$O unitname}`来指定覆盖的单元。主程序还应该使用*OvrInit*初始化覆盖文件，如果有可能，可以使用*OvrInitEMS*将覆盖文件载入EMS。另外，还有一个类似*IOResult*的*OvrResult*来检测错误。包含覆盖功能的程序只能编译到文件。参考OVRDEMO.PAS。

### EMS扩展内存

先让我来解释一下EMS是什么。在8086/8088中，寻址空间为$2^{20}$=1MB，其中低640KB可以被程序直接访问(常规内存)，而剩下的384KB则用于硬件。而到了IBM AT/286时代，尽管寻址空间达到了16MB，但是只支持在保护模式下访问，没有任何DOS程序可以在那里运行。

当时的解决方案之一是内存盘，这个相信大家应该不陌生，但是内存盘不能用来运行程序。另一种方法就是扩展内存，利用384KB的一部分来作为“窗口”来使用高内存区。其中一种就是Lotus/Intel/Microsoft
Expanded Memory Specification (EMS)，TP5支持3.2+的版本，在扩展内存中可以存放覆盖文件，并将扩展内存用于内置代码编辑器。可以用EMS.PAS来检测EMS是否可用。

### 过程类型

过程类型可以将过程或函数作为变量定义，并作为参数传递。

```pascal
type
  Proc       = procedure;
  SwapProc   = procedure(var X,Y: integer);
  StrProc    = procedure(S: string);
  MathFunc   = function(X: real): real;
  DeviceFunc = function(var F: text): integer;
  MaxFunc    = function(A,B: real; F: MathFunc): real;
```

当然，TP不允许返回过程类型，也不允许标准过程或函数、嵌套过程或函数、**inline**过程或函数、**interrupt**过程或函数赋值给过程类型。另外，需要开启编译开关`{$F+}`。参考PROCVAR.PAS。

### 常数表达式

TP5允许用表达式初始化常数。

```pascal
const
  Min      = 0;
  Max      = 100;
  Center   = (Max - Min) div 2;
  Beta     = Chr(225);
  NumChars = Ord('Z') - Ord('A') + 1;
  Message  = 'Out of memory';
  ErrStr   =' Error: ' + Message + '. ';
  ErrPos   = 80 - Length(ErrorStr) div 2;
  Ln10     = 2.302585092994045684;
  Ln10R    = 1 / Ln10j
  Numeric  = ['0' .. '9'];
  Alpha    = ['A' .. 'Z' ,'a' .. 'z'];
  AlphaNum = Alpha + Numeric;
```

## 5.5(1989)

![](/img/tp55ui.png)

![](/img/tp55man.png)

![](/img/tp55inst.png)

### 官方介绍

![](http://edn.embarcadero.com/article/images/20803/tp55ad.jpg)

官方下载：<http://altd.embarcadero.com/download/museum/tp55.zip>

发布日期：1989/5/2

### TOUR

5.5包含了一个TOUR.EXE——一个在线的介绍工具，可以让新手了解IDE的使用，大约需要15分钟完成。

![](/img/tp55tour.png)

### 面向对象

5.5最主要的改进就是全面支持面向对象程序设计(OOP)，当然TP主要借鉴了C++。其实从某种角度而言，对象就是带有过程和函数的记录，在TP中用关键字**object**代替**record**。

#### 类定义

```pascal
type
  Location = object
    X, Y : Integer;
  end;
  Point = object(Location)
    Visible : Boolean;
  end;
```

其中括号表示了继承关系。在*Point*类中，可以正常使用*X*和*Y*。

#### 方法定义

```pascal
type
  Location = object
    X, Y : Integer;
    procedure Init(NewX, NewY : Integer);
  end;
procedure Location. Init (NewX, NewY : Integer);
begin
  X := NewX; { The X field of a Location object }
  Y := NewY; { The Y field of a Location object }
end;
```

类拥有的过程和函数被称为*方法*，与C++使用::不同，TP直接用.表示类的方法。另外，TP没有访问控制，不能阻止直接访问类的字段。字段必须在所有方法前定义。

#### *Self*的使用

```pascal
type
  MouseStat = record
    Active: Boolean;
    X, Y : Integer;
    LButton, RButton : Boolean;
    Visible : Boolean;
  end;
procedure Location.GoToMouse(MousePos : MouseStat);
begin
  Hide;
  with MousePos do
  begin
    Self.X := X;
    Self.Y := Y;
  end;
  Show;
end;
```

以上代码展示了*Self*的用法，其实和*this*类似，只不过后者是指针。

#### 方法重载

```pascal
type
  Circle = Object(Point)
    Radius : Integer;
    procedure Init(InitX, InitY : Integer; InitRadius : Integer);
    procedure Show;
    procedure Hide;
    procedure Expand(ExpandBy : Integer);
    procedure MoveTo(NewX, NewY : Integer);
    procedure Contract (ContractBy : Integer);
  and;
procedure Circle.Init(InitX, InitY : Integer; InitRadius : Integer);
begin
  Point.Init(InitX, InitY);
  Radius := InitRadius;
and;
procedure Circle. Show;
begin
  Visible := True;
  Graph.Circle(X, Y, Radius);
and;
procedure Circle. Hide;
var
  TempColor : Word;
begin
  TempColor := Graph.GetColor;
  Graph.SetColor(GetBkColor);
  Visible := False;
  Graph.Circle(X, Y, Radius);
  Graph.SetColor(TempColor);
end;
procedure Circle.Expand(ExpandBy : Integer);
begin
  Hide;
  Radius := Radius + ExpandBy;
  if Radius < 0 then Radius := 0;
Show;
end;
procedure Circle. Contract (ContractBy : Integer);
begin
  Expand(-ContractBy);
end;
procedure Circle.MoveTo(NewX, NewY : Integer);
begin
  Hide;
  X := NewX;
  Y := NewY;
  Show;
end;
```

重载方法只要定义完全相同的方法，并改变内容。在上例中，用*Point.Init*来调用*Point*的方法。

#### 构造和析构

TP使用关键字**constructor**修饰构造过程，用**destructor**修饰析构过程，建议命名为*Init*和*Done*。

```pascal
Point = object(Location)
  Visible : Boolean;
  Next : PointPtr;
  constructor Init(InitX, InitY : Integer);
  destructor Done; virtual;
  procedure Show; virtual;
  procedure Hide; virtual;
  function IsVisible : Boolean;
  procedure MoveTo(NewX, NewY : Integer);
  procedure Drag (DragBy : Integer); virtual;
end;
```

#### 虚方法

TP也支持虚方法，使用**virtual**关键字在方法定义后，这里不再详细介绍，可以参考C++。

#### 面向对象的Calc

历史悠久的MicroCalc被重命名为TurboCalc，并且用面向对象重写。

![](/img/tp55calc.png)

### 对象调试

5.5同时支持对对象进行调试。

### 覆盖功能的改进

#### 覆盖缓冲区管理

在5.0中，覆盖缓冲区用一个类似循环队列的数据结构维护，当空间足够时直接载入，不足时删除最早的模块。而5.5提供了可选的其他算法：当一个模块到队尾的空间小于阈值时，将这个模块标记，如果随后调用了被标记的模块，说明这个模块使用频繁，将其移至队首。通过测试，可以得出最佳的阈值，并用5.5提供的接口实现。

#### 将覆盖文件放入.EXE文件

TP5.5支持将覆盖文件放入.EXE中，以减少文件数量。首先必须确保关闭调试符号，然后用带/B选项的COPY命令，例如`COPY /B MYPROG.EXE + MYPROG.OVR`。加载覆盖文件用.EXE文件，即`Ovrlnit(ParamStr(0));`

## 6.0(1990)

![](/img/tp6ui.png)

![](/img/tp6man.png)

![](/img/tp6inst.png)

### IDE

![](/img/tp6ide.png)

新的IDE支持多窗口编辑，并支持窗口重叠、并列等排列(Windows 2.0)。支持鼠标，但是我自己安装了CuteMouse。

### Turbo Vision

![](/img/tp6tvdemo.png)

Turbo Vision是一个面向对象的应用程序框架，用于开发图形界面程序，风格与IDE相同。当然，其实还是基于文本界面的，只是提供了窗口、菜单、状态栏、对话框等元素的实现。上图是一个示例，缺点是很费内存，右下角显示的数字就是可用内存。应该在IDE外运行时可用空间将增加。

![](/img/tp6mle.png)

退出IDE后，可以自由打开文件，但是只能查看，不能修改。另外提供了4个工具：15数码、日历、ASCII表和计算器。

![](/img/tp6file.png)

![](/img/tp6tools.png)

另外还有智能帮助功能，只要按下F1即可。

![](/img/tp6puzzlehelp.png)

### 内联汇编器

这个功能允许你直接在Pascal代码中插入80(2)86/7汇编代码，当然旧的方法仍然可以使用。它支持Turbo Assembler的相当大部分语法子集，但不支持绝大部分编译指令，应该使用Pascal语法来代替。

通过**asm**语句来使用内联汇编器。

```pascal
asm
  mov    ah,0        { Read keyboard function code }
  int    16H            { Call PC BIOS to read key }
  mov    CharCode,al             { Save ASCII code }
  mov    ScanCode,ah              { Save scan code }
end;

asm
  push   ds                              { Save DS }
  lds    si,Source           { Load source pointer }
  les    di,Dest        { Load destination pointer }
  mov    cx,Count                { Load block size }
  cld                              { Move forwards }
  rep    movsb                        { Copy block }
  pop    ds                           { Restore DS }
end;
```

#### 内联汇编的关键字

```
AH      CL      FAR     SEG
AL      CS      HIGH    SHL
AND     CX      LOW     SHR
AX      DH      MOD     SI
BH      DI      NEAR    SP
BL      DL      NOT     SS
BP      DS      OFFSET  ST
BX      DWORD   OR      TBYTE
BYTE    DX      PTR     TYPE
CH      ES      QWORD   WORD
                        XOR
```

### 面向对象的改进

**private**带来了私有的字段和方法，在上一个版本中没有提供支持。但是，与C++中的*class*不同的是，默认的仍然为**public**(当然没有这个关键字)，类似于*struct*(POD)。

```pascal
type
  NewObject = object(ancestor)
    fields; { these are public }
    methods; { these are public }
  private
    fields; { these are private }
    methods; { these are private }
  end;
```

### 扩展语法

扩展语法开关(`$X`)可以像过程一样使用函数，即忽略其返回值。

### 286代码生成

使用`$G`开关可以允许生成286代码，其无法在8086上运行。

### TPTOUR

这个版本的TOUR比5.5要高级多了，在鼠标使用方面有点像Windows 3.1的教程。不过这个教程还是偏向于使用键盘而不是鼠标，有时经常会出现警告：请使用键盘而不是鼠标。

![](/img/tp6tour.png)

![](/img/tp6tour2.png)

![](/img/tp6tour3.png)

![](/img/tp6tourwarn.png)

## 7.0(1992)

![](/img/tp7ui.png)

![](/img/tp7man.png)

### IDE

除了提供传统的实模式(8086)下的TURBO.EXE之外，还提供了保护模式下的TPX.EXE。要运行保护模式下的IDE，必须具有286和至少2MB内存。

两个IDE有一些区别，内存占用不同，并且TPX不再提供编译到内存功能，只能编译到文件。

![](/img/tp7protect.png)

![](/img/tp7real.png)

#### 语法高亮

7.0支持默认关键字高亮为白色，注释为灰色，汇编为绿色，其余符号为黄色。当然这个设置可以在菜单中手动修改，可以修改到接近Free Pascal的高亮，但是不识别编译开关。

![](/img/tp7highlight.png)

#### 撤销功能

撤销和恢复功能，与现代编辑器相同，可持久化……

#### 右键菜单

这个在现在看来也很基本……

### 语言改进

#### 变长参数

支持变长字符串*OpenString*和变长数组参数(注意只能是参数)，用*Low*和*High*返回起始下标和结束下标，相当于C++容器的*begin*和*end*方法，只不过不超尾。

#### 面向对象

提供了**public**关键字，可以与**private**混合使用。另外**inherited**关键字可以调用祖先的方法。

#### **const**参数

**const**的参数不能被修改，相当于常量。

### 运行时库(RTL)

#### 新增系统函数

- *Assigned*:测试指针是否为**nil**
- *Break*
- *Continue*
- *High*
- *Low*

