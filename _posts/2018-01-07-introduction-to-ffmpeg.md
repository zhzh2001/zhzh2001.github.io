---
layout: post
title: Introduction to FFmpeg
tags:
- intro
---

# 简介

一直以来就想介绍一下FFmpeg这个强大的多媒体工具。官方介绍为"A complete, cross-platform solution to record, convert and stream audio and video. "

FFmpeg包含三个部分：ffmpeg, ffplay和ffprobe，分别用于转换、播放和查看信息。当然查看信息用MediaInfo更加方便，文件右键即可。而播放，相信大家都有自己偏好的播放器，不会去用命令行工具播放。说句题外话，我目前用PotPlayer，另外K-Lite、VLC也不错。接下来主要就介绍ffmpeg。

很多播放器都用ffmpeg来软解码（与硬件解码相对），但除此之外，ffmpeg还可以用来转码，简单的剪辑，修改视频分辨率。使用各种库可以支持很多常见的编码操作。ffmpeg是命令行工具，方便批处理，还提供库以供再开发，完全开源。

<!-- more -->

ffmpeg的版本迭代很快，我一般用稳定版。由于GPL的限制，不包含非开源的内容，所以有一定的欠缺。可以自己编译非自由版本，但是难度很大，可能要花半天。我以前也试过，但现在已经放弃了，转而用自由版本。Windows可以直接获取二进制文件；一般的发行版都提供ffmpeg，可能版本比较旧，但其实影响不大。

# 多媒体基础

# 使用

最简单的转换只要`ffmpeg -i input.mp4 output.avi`，其中`-i`表示输入文件，后面表示输出文件。ffmpeg会自动选择合适的方法转换，但可能不是最佳的。

TBC
