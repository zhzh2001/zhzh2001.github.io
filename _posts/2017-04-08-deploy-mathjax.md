---
layout: post
tags: 教程
title: 部署Mathjax
---

# 关于Mathjax

## 介绍

`Mathjax`是一种方便的数学公式渲染工具，能接受多种输入和输出要求：

- 输入
  - ASCIIMath
  - MathML
  - TeX(在Markdown中所用)
- 输出
  - CommonHTML(默认)
  - HTML-CSS
  - NativeMML
  - PlainSource
  - PreviewHTML(快速预览)
  - SVG

## CDN服务器将要关闭

`Mathjax`推荐通过cdn.mathjax.org使用，但由于经费问题，**CDN服务器将于4月30日关闭**。当我得到这条消息时，又想起了曾经被否定的计划。

 <!-- more -->

# 部署Mathjax

## 转移

官方推荐2种方法来转移：

- 使用cloudflare的CDN
  - 方法：将<https://cdn.mathjax.org/mathjax/latest/>改为<https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.0/>
  - 缺点：不能使用`latest`，必须手动指定版本号
- 在本地部署`Mathjax`


## 本地部署步骤

1. 下载`Mathjax`，可以为`release`或`git`版本，我使用[Mathjax-2.7](https://github.com/mathjax/MathJax/archive/2.7.0.zip)，也可以使用`git`版本。
2. 解压文件到网站的**一个易于访问的位置**，可以直接通过相对路径表示。
3. 删除不必要的文件，整个版本有~60MiB，上万个文件。

## 精简过程

> 可以直接下载我的版本：<https://github.com/zhzh2001/zhzh2001.github.io/tree/master/mathjax>

1. 删除`/config`下多余的配置文件。

   我保留了`Tex-MML-AM_CHTML.js`，表示支持`Tex`，`MathML`和`ASCIIMath`输入，`CommonHTML`输出，也是官方推荐的选项。

2. 删除`/docs`，`/test`和`/unpacked`，包含`Mathjax`的文档及用于测试的文件。

3. 保留`/fonts/HTML-CSS`下的一种字体，并只保留这种字体的`woff`版本。

   我保留了`Tex`，是最小的字体。

4. 保留`/localization`下的一种语言或全部删除(使用默认的英语)。

5. 删除`/jax/output`下不必要的输出格式，如果保留`SVG`则可以删除整个`/font`目录。

   我保留了`CommonHTML`。

## 自定义弹出菜单

> 删除部分文件后，如果不删除有关的菜单，可能使用户不小心选错而无法使用`Mathjax`。

1. 打开保留的配置文件，如`/config/Tex-MML-AM_CHTML.js`。

2. 删除`Mathjax.Ajax.Preloading`下不存在的脚本(可选)。

3. 推荐：删除`Mathjax.Hub.Config`下的`{extensions: ['[Contrib]/a11y/accessibility-menu.js']}`。

   > 因为该脚本需要从`[Contrib]`加载，当前指向cdn.mathjax.org

4. 在`Mathjax.Hub.Config`下加入
```javascript
MathMenu: {
   delay: 0,	//可以随意设置
   showRenderer: false,	//不显示渲染器
   showLocale: false	//不显示语言
}
```

5. 推荐：关闭快速预览，在`Mathjax.Hub.Config`下加入
```javascript
"fast-preview": {
  disabled: true	//禁用快速预览
}
```

6. 推荐：加入标准Markdown的`$`公式，而不是Latex的`$$`
```javascript
tex2jax: {inlineMath: [['$','$'], ['\\(','\\)']]}
```

> 目前Mathjax已更新到2.7.1，可以选择升级。
