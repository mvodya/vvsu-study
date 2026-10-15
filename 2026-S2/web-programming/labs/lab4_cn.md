# 实验作业 4：初识 CSS 样式

## 作业内容

创建一个音乐发行网站，包含三个页面：专辑介绍、曲目和音乐人介绍

选择真实或虚构音乐人的专辑；可以使用以下示例中的名称、文字和曲目，也可以在保留要求的前提下替换或补充

示例：虚构音乐人 `静谧之城`，专辑 `午夜之后`，风格 `电子音乐`，发行日期 `2026 年 10 月 15 日`

## 项目

创建新项目

```text
lab4-css/
|-- index.html
|-- tracks.html
|-- artist.html
`-- assets/
    `-- cover.jpg
```

每个页面都要显示音乐人名称、当前页面的标题，以及指向相应文件的 `专辑介绍`、`曲目`、`音乐人介绍` 链接

为 `cover.jpg` 选择专辑封面，或者为虚构发行作品制作自己的图片

只使用 `style` 属性以及位于 `head` 内的 `<style>` 元素；不使用独立 CSS 文件、JavaScript 或 CSS 库

内容按普通文档流从上到下排列，不使用 Flexbox、Grid、`float`、`position` 或分栏

## 专辑介绍：style 属性

在 `index.html` 中用 `h1` 显示专辑名称，然后添加三个独立段落，分别介绍音乐人、风格和发行日期

```text
音乐人：静谧之城
风格：电子音乐
发行日期：2026 年 10 月 15 日
```

先通过每个段落自身的 `style` 属性设置样式

1. 第一段：文字颜色 `#1e40af`，字号 `24px`，粗体
2. 第二段：颜色 `#166534`，字号 `18px`，斜体
3. 第三段：颜色 `#991b1b`，背景 `#fee2e2`，内边距 `12px`

属性中列出 `属性: 值`，各项用分号分隔，例如

```html
<p style="color: #1e40af; font-size: 24px;">音乐人：静谧之城</p>
```

粗体使用 `font-weight: bold`，斜体使用 `font-style: italic`

## 专辑介绍：style 元素

在 `index.html` 页面的 `head` 内添加

```html
<style>
  body {
    font-family: Arial, sans-serif;
    color: #1f2937;
    background-color: #ffffff;
  }

  .release-info {
    font-size: 18px;
  }
</style>
```

这里的 `body` 按标签名选择元素，`.release-info` 则选择所有具有 `class="release-info"` 的元素

1. 为三个段落都添加 `release-info` 类
2. 将第二段的字号、颜色和字体样式从属性移入 `.release-info` 规则，并删除该段落空的 `style` 属性
3. 在此规则后再添加一条 `.release-info` 规则，只设置 `color: #374151`；其余属性保留在第一条规则中
4. 第一段和第三段应保留各自属性中的颜色，第二段应使用最后一条规则中的颜色
5. 添加第四个段落，简要介绍专辑，设置 `release-info` 类，不添加 `style` 属性，例如 `适合夜晚漫步城市时聆听的音乐`
6. 使用标签选择器为 `h1` 设置字号 `32px`，通过 `text-align` 居中，通过 `letter-spacing` 设置 `1px` 的字符间距

在介绍后添加 `assets` 中的封面，设置 `cover` 类；在 `<style>` 中为它设置 `width: 100%`、`max-width: 320px`、`height: auto`，并通过 `border-radius` 设置 `8px` 圆角

`body` 的字体通过继承作用于内部文字；类规则可以用于多个元素，而 `style` 属性只属于一个元素

## 曲目：类、尺寸与间距

在 `tracks.html` 中创建至少三张 `article` 卡片，每张包含用 `h2` 表示的曲名、时长、介绍以及一个独立的情绪段落

| 曲目 | 时长 | 介绍 | 情绪 |
| --- | --- | --- | --- |
| 末班电车 | 03:42 | 柔和的节奏与夜间城市的声音 | 平静 |
| 霓虹 | 04:08 | 快速节拍与明亮的合成器旋律 | 活力 |
| 黎明之前 | 05:16 | 为夜间漫步缓缓收尾 | 沉思 |

将本页所有规则写入它自己的 `<style>`；`index.html` 中的样式不会自动应用到这里

1. 为 `body` 设置 `font-family: Arial, sans-serif` 和 `color: #1f2937`
2. 为所有卡片添加 `track` 类，并使用一条规则设置：背景 `#f3f4f6`，边框 `2px solid #475569`，圆角 `12px`，内边距 `16px`，下外边距 `24px`
3. 为同一个类设置 `width: 100%`、`max-width: 480px` 和 `box-sizing: border-box`，使边框和内边距包含在指定宽度内
4. 通过 `.track h2` 选择器设置标题字号 `22px`，上外边距 `0`
5. 为介绍段落添加 `description` 类，设置 `font-size: 16px` 和 `line-height: 1.6`
6. 为一张卡片添加第二个类 `featured`，保留 `track`；将 `.featured` 规则放在 `.track` 后，只将背景改为 `#fef3c7`，边框颜色改为 `#92400e`
7. 为情绪段落添加 `mood` 类并设置粗体，再为其中一个添加 `accent` 类，设置颜色 `#991b1b`
8. 将每首曲目的时长放在带有 `duration` 类的段落中，设置 `font-family: monospace`、`font-size: 16px` 和 `color: #475569`

多个类名之间用空格分隔：`class="track featured"`

`padding` 设置内边距，`margin-bottom` 和 `margin-top` 设置外边距，`border` 设置边框，`border-radius` 设置圆角；即使没有颜色，情绪文字也应表达清楚

## 音乐人介绍：文字、列表与链接

在 `artist.html` 中放置音乐人的简短简介、音乐影响因素列表，以及一个独立的专辑录制说明段落

简介

```text
静谧之城是来自符拉迪沃斯托克的电子音乐项目。它的音乐将城市声音录音与合成器旋律结合在一起
```

音乐影响因素

- 氛围音乐
- 合成器流行音乐
- 夜间城市的声音

录制说明

```text
专辑中的街道声音录制于夜间散步期间
```

将本页所有样式放在它的 `<style>` 中

1. 为 `body` 设置 `font-family: Georgia, serif` 和 `color: #1f2937`
2. 为简介添加 `intro` 类：字号 `20px`，行高 `1.5`，左对齐
3. 为列表添加 `influences` 类：通过 `list-style-type: square` 使用方形标记，左内边距为 `24px`
4. 通过 `.influences li` 选择器设置列表项的下外边距 `10px`
5. 为录制说明段落设置 `id="recording"`；在 `#recording` 规则中设置背景 `#fff7ed`，左边框 `4px solid #9a3412`，内边距 `12px`
6. 为页面中的所有链接设置颜色 `#1e40af`，并通过 `text-decoration: underline` 添加下划线
7. 在 `a:hover` 规则中将颜色改为 `#7c2d12`，背景改为 `#fff7ed`
8. 在 `a:focus` 规则中设置 `outline: 2px solid #1e40af` 和 `outline-offset: 3px`；不要移除焦点轮廓线

`:hover` 在鼠标悬停时应用，`:focus` 在链接获得焦点时应用，例如按下 `Tab` 之后

## 在浏览器中检查

通过本地 Web 服务器打开项目

在开发者工具中选中专辑信息的第二个段落，在 `Styles` 面板中找到第一条 `.release-info` 规则里被划掉的颜色值；再查看第一段，找到来自属性的生效颜色

选中一张曲目卡片，找到元素尺寸示意图，对比 `padding`、`border` 和 `margin` 的位置

在音乐人页面将鼠标移到链接上，再使用 `Tab` 选中链接，并按 `Enter` 跳转
