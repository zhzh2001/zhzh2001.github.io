## 介绍

主要用于发表**原创**题解，以及其他有趣的总结；内容将不会很多。

采用`Github`默认的*将纯文本转换为静态博客网站*的[`jekyll`][jekyll]技术，~~完全用`Markdown`完成~~(除了*文章列表*为`HTML`)

使用的主题为[`Cayman`](https://github.com/pages-themes/cayman)

---

## 文章列表

<ul>
  {% for post in site.posts %}
    <li>
      <a href="{{ post.url }}">{{ post.title }}</a>
    </li>
  {% endfor %}
</ul>

---

## 代码仓库

欢迎访问我的`代码仓库`@[github](https://github.com/zhzh2001/Learning) ，包含2017年起的**所有代码**。

### 其他仓库

[CCR-Plus](https://github.com/sxyzccr/CCR-Plus)

## 其他网站

> 以下排列顺序为发布时间，直接复制从聊天记录

[萌呆农大](http://blog.csdn.net/mdnd1234)

[jzqjzq](http://blog.csdn.net/jzq233jzq)

[GTA5](https://swwind.github.io)

[szb](https://shenzhebei.github.io)

我用于创建文章的编辑器[Typora](https://typora.io/)

[jekyll中文][jekyll]



---

## 限制

这只是个个人网站，**且目前处于测试期** 。

以下简要总结了`Github Pages`有关的限制，详细请参见[这里](https://help.github.com/articles/what-is-github-pages/#usage-limits)

- 每月带宽不超过100GB
- 每小时更新不超过10次

## 历史

以下按照时间顺序排列：

- α创建于2017/3/11
- β首次加入内容于2017/3/13
- β修复数学公式显示问题2017/3/14
- β尝试修改主题2017/3/17

---

## 测试

> 这部分内容仅测试用

$$f_n=\sum\limits_{i=0}^{n-1}f_i*f_{n-i-1}$$ [^catalan] ~~怎么不支持数学公式！~~

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

[jekyll]:http://jekyllcn.com/

[^catalan]: 卡特兰数公式

