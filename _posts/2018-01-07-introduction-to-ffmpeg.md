---
layout: post
title: Introduction to FFmpeg
tags:
- intro
updated: 2018-01-14
---

# 简介

一直以来就想介绍一下FFmpeg这个强大的多媒体工具。官方介绍为"A complete, cross-platform solution to record, convert and stream audio and video. "

FFmpeg包含三个部分：ffmpeg, ffplay和ffprobe，分别用于转换、播放和查看信息。当然查看信息用MediaInfo更加方便，文件右键即可。而播放，相信大家都有自己偏好的播放器，不会去用命令行工具播放。说句题外话，我目前用PotPlayer，另外K-Lite、VLC也不错。接下来主要就介绍ffmpeg。

很多播放器都用ffmpeg来软解码（与硬件解码相对），但除此之外，ffmpeg还可以用来转码，简单的剪辑，修改视频分辨率。使用各种库可以支持很多常见的编码操作。ffmpeg是命令行工具，方便批处理，还提供库以供再开发，完全开源。

<!-- more -->

ffmpeg的版本迭代很快，我一般用稳定版。由于GPL的限制，不包含非开源的内容，所以有一定的欠缺。可以自己编译非自由版本，但是难度很大，可能要花半天。我以前也试过，但现在已经放弃了，转而用自由版本。Windows可以直接获取二进制文件；一般的发行版都提供ffmpeg，可能版本比较旧，但其实影响不大。

# 多媒体基础

一个视频文件，一般包括视频(Video)、音频(Audio)，可能还包含字幕(Subtitle)、章节等文本信息，甚至包含附件。所以这个包装方式称为**容器**(Container)，或一般称为**格式**，与文件后缀有关。

一个容器中可以包含多个视频、音频、字幕**流**，而这些流的格式称为**编码格式**，相当于不同的压缩算法。编码方式可以分为有损和无损编码，一般视频不会使用无损编码。

## 格式

Matroska是较新的流行格式，开源，支持几乎所有编码，几乎什么都可以塞进去。但是比较旧的播放器可能不支持。推荐用MKVToolnix混流，而不是ffmpeg，前者输出文件的兼容性更好。我推荐自己制作或转码的视频用这个容器。

MPEG-4也比较常见，大多数播放器都支持。可以使用的编码相比Matroska少，但能满足常见的需要。

AVI(Audio Video Interleave)是一种相当古老的格式，由微软设计，所以在Windows下支持很好。从Windows 3.1开始的Video for Windows就用这种格式。较新的编码也能支持，但是播放器就不一定了。不应该再使用这种格式。

FLV(Flash Video)是网络视频常见的格式，支持的编码很有限。

WMV(Windows Media Video)是微软的另一种容器，现在也已经比较少见了。不过Windows 7自带的视频Wildlife.wmv很经典。其使用的编码是专有的，因此无法用ffmpeg实现编码。

# 使用

最简单的转换只要`ffmpeg -i input.mp4 output.avi`，其中`-i`表示输入文件，后面表示输出文件。ffmpeg会自动选择合适的方法转换，但可能不是最佳的。

TBC
