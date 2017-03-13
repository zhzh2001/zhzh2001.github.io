# zhzh2001的网站

## 介绍

主要用于发表**原创**题解，以及其他有趣的总结；内容将不会很多。

## 文章

<ul>
  {% for post in site.posts %}
    <li>
      <a href="{{ post.url }}">{{ post.title }}</a>
    </li>
  {% endfor %}
</ul>

## 限制

这只是个个人网站，**且目前处于测试期** 。

以下简要总结了`Github Pages`有关的限制，详细请参见https://help.github.com/articles/what-is-github-pages/#usage-limits

- 每月带宽不超过100GB
- 每小时更新不超过10次

## 历史

以下按照时间顺序排列：

- 创建于2017/3/11
- 首次加入内容于2017/3/13

## 测试

> 这部分内容仅测试用

$C_m^n=\frac{\prod\limits_{m-n+1}^m}{n!}$ [^choose] *怎么不支持数学公式！*

- [x] 组合数公式
- [ ] 阶乘公式

这是一个`hello, world`

```cpp
#include<iostream>
using namespace std;
int main()
{
    cout<<"hello, world\n";
    return 0;
}
```

| 名称           | 版本    | 体系   | 版本   |
| ------------ | ----- | ---- | ---- |
| Windows 10   | 15055 | x64  | 专业版  |
| Ubuntu Linux | 14.04 | x86  | LTS  |
| Windows Bash | 16.04 | x64  | LTS  |

[^choose]: 组合数公式