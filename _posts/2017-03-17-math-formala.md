---
layout: post
title: 如何解决数学公式显示问题
tags: 教程
---

> 这是我的第一篇教程*aka.how-to*

# 问题描述

## 使用`Github Pages`提供的`jekyll`

与标准的`Markdown`不同，不提供数学公式的支持。

 <!-- more -->

# 方法

1. 下载*主题仓库*中`/_layouts/default.html`放入`/_layouts`下

2. 加入以下代码到`<head>`中以支持`mathjax`*[来源][mathjax]*

   ```html
   <script type="text/javascript" async
   		src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-MML-AM_CHTML">
   	</script>
   ```

3. 在文章中使用`$$<math>$$`来使用数学公式

## 备选方法

将`Markdown`的`$<math>$`改为`![](https://latex.codecogs.com/gif.latex?<latex>)`，将以图片形式显示数学公式，**不推荐使用**。

> 来源于洛谷的Wiki系统

[mathjax]: http://docs.mathjax.org/en/latest/start.html

