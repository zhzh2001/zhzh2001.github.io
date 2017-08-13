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

## 1.0(1983)

1.0没有DOS版本，并且找不到有关文档。

## 2.0(1984)

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

## 3.0(1985)

![](/img/tp3disk.jpg)

![](/img/tp3.png)

![](/img/tp3man.png)

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

