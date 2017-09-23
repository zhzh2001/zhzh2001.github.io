---
layout: page
title: 文章列表
---

<ul>
  {% for post in site.posts %}
    <li>
      <a href="{{ post.url }}">{{ post.title }}</a>
      {{ post.date | date:"%Y/%m/%d" }}
    </li>
  {% endfor %}
</ul>

