# CSS：基础样式与元素外观

本材料用于多次实验作业。作业 4 需要掌握前两种 CSS 写法、基础选择器、层叠与继承、文字、尺寸、间距、图片和链接状态。其他部分根据后续作业的要求使用

页面布局、Flexbox、Grid 和定位在理论 05 中讲解；断点和布局重排在理论 06 中讲解

## 目录

- [CSS 的编写与引入](#css-的编写与引入)
- [语法](#语法)
- [选择器](#选择器)
- [伪类](#伪类)
- [伪元素](#伪元素)
- [层叠与选择器权重](#层叠与选择器权重)
- [继承与值的重置](#继承与值的重置)
- [单位与计算](#单位与计算)
- [CSS 变量](#css-变量)
- [颜色、背景与透明度](#颜色背景与透明度)
- [字体与文字](#字体与文字)
- [尺寸与盒模型](#尺寸与盒模型)
- [边框、轮廓线与阴影](#边框轮廓线与阴影)
- [图片](#图片)
- [列表与表格](#列表与表格)
- [链接、按钮与表单控件](#链接按钮与表单控件)
- [过渡、变换与动画](#过渡变换与动画)
- [样式的应用条件](#样式的应用条件)
- [CSS 的组织与检查](#css-的组织与检查)

## CSS 的编写与引入

CSS 决定 HTML 元素的外观。同样的属性可以通过三种方式编写

### style 属性

样式只属于某个具体元素：

```html
<p style="color: #1e40af; font-size: 24px;">专辑名称</p>
```

适合初次学习和单个示例，但重复使用时需要逐个修改元素。属性内不能编写选择器或 `:hover` 等状态

### style 元素

规则放在当前文档的 `head` 中：

```html
<style>
  .description {
    color: #374151;
    font-size: 16px;
  }
</style>
```

样式仅作用于该文档，即使其他页面也来自同一个网站

### 外部文件

```text
site/
|-- index.html
|-- pages/
|   `-- artist.html
|-- css/
|   `-- styles.css
`-- assets/
    |-- cover.jpg
    `-- fonts/
        `-- album-sans.woff2
```

在 `index.html` 的 `head` 中：

```html
<link rel="stylesheet" href="css/styles.css" />
```

在 `pages/artist.html` 的 `head` 中：

```html
<link rel="stylesheet" href="../css/styles.css" />
```

`styles.css` 文件本身只包含 CSS，不需要 `<style>` 标签：

```css
body {
  font-family: Arial, sans-serif;
  color: #1f2937;
}
```

同一个文件可以引入到所有页面中，修改公共规则会影响每个页面。文件使用 UTF-8 编码和 `.css` 扩展名

`href` 中的路径相对于 HTML 文档。外部 CSS 的 `url()` 中的路径相对于 CSS 文件：对于上述结构，应使用 `url("../assets/cover.jpg")`

可以引入多个样式表。当规则优先级相同时，起作用的是引入顺序，而不是加载速度。外部文件本身并不比 `<style>` 更强或更弱

`@import url("theme.css");` 也可以从 CSS 中引入样式表。本课程的普通文件使用 `<link>`：依赖关系直接显示在 HTML 中，也不会形成串联加载。`@import` 必须在普通规则之前；它前面可以出现 `@charset` 和用于声明层顺序的 `@layer`

## 语法

```css
/* 规则的简短说明 */
.track {
  color: #1f2937;
  padding: 16px;
}
```

`.track` 是选择器，大括号内部是声明块。每条声明由属性和值组成，中间用冒号分隔，以 `;` 结束

`style` 属性内不需要大括号。CSS 的注释使用 `/* ... */`，而不是 `//`

属性名用连字符连接，例如 `background-color`。值的类型取决于属性，例如 `16px`、`bold`、`#ffffff`、`url("image.jpg")`

浏览器会忽略未知属性和不合适的值。漏写分号可能影响相邻声明，未闭合的大括号可能破坏多个规则的结构

简写属性可以一次设置多个组成部分：

```css
.track {
  border: 2px solid #475569;
}
```

这里依次设置边框粗细、线型和颜色。简写可能重置未指定的组成部分，例如 `background: white` 会清除先前设置的背景图片。如果只想改变颜色，使用 `background-color`

## 选择器

### 基础选择器与组合

| 选择器 | 选择的对象 |
| --- | --- |
| `*` | 所有元素 |
| `h1` | 所有 `h1` 元素 |
| `.track` | 带有 `track` 类的元素 |
| `#recording` | 标识符为 `recording` 的元素 |
| `article.track` | 带有 `track` 类的 `article` 元素 |
| `.track.featured` | 同时拥有这两个类的元素 |
| `h1, h2, h3` | 列出的每种标题元素 |

```html
<article class="track featured">
  <h2>霓虹</h2>
</article>
```

类适合重复使用的样式。一个元素可以拥有多个类，`class` 中各个类名的顺序不影响优先级。教学项目适合采用表达用途的名称，例如 `track`、`description`、`duration`

### 元素之间的关系

| 选择器 | 选择的对象 |
| --- | --- |
| `.track h2` | `.track` 内任意深度的 `h2` |
| `.track > h2` | 仅选择 `.track` 的直接子元素 `h2` |
| `h2 + p` | 与 `h2` 同属一个父元素、紧跟在它后面的段落 |
| `h2 ~ p` | 与 `h2` 同属一个父元素、位于它之后的所有段落 |

`.track .featured` 查找后代元素，而 `.track.featured` 查找同时拥有两个类的同一个元素。空格会改变含义

### 属性选择器

| 选择器 | 条件 |
| --- | --- |
| `[required]` | 存在 `required` 属性 |
| `input[type="email"]` | 输入框具有指定类型 |
| `a[href^="https://"]` | 地址以指定字符串开头 |
| `a[href$=".pdf"]` | 地址以指定字符串结尾 |
| `[class~="featured"]` | 值中包含独立的单词 `featured` |
| `[data-kind*="live"]` | 值中包含子字符串 `live` |

`data-*` 可以在 HTML 中保存自定义标记，例如 `data-kind="live-recording"`。子字符串选择器不理解 URL 的含义：`file.pdf?download=1` 并不以 `.pdf` 结尾

语法形式列表：[MDN CSS 选择器](https://developer.mozilla.org/en-US/docs/Web/CSS/Reference/Selectors)

## 伪类

伪类以一个冒号开头，用于检查元素的状态或位置

| 选择器 | 应用时机 |
| --- | --- |
| `a:link` | 链接尚未访问 |
| `a:visited` | 链接已访问；浏览器出于隐私原因限制可用的样式 |
| `a:hover` | 指针悬停在链接上 |
| `a:active` | 链接正被激活，例如按住鼠标按钮时 |
| `a:focus` | 链接获得焦点 |
| `a:focus-visible` | 浏览器认为需要显示焦点，通常出现在键盘操作时 |
| `.track:focus-within` | 卡片本身或其后代元素获得焦点 |
| `input:checked` | 复选框或单选按钮被选中 |
| `input:disabled` | 输入框通过 `disabled` 属性被禁用 |
| `input:required` | 输入框为必填项 |
| `input:invalid` | 值不满足 HTML 约束 |
| `section:target` | 地址中 `#` 后的片段与该部分的标识符一致 |

`:active` 不表示“菜单中的当前页面”。当前页面可以使用类或 `[aria-current="page"]` 标记

当权重相同时，链接规则通常按 `:link`、`:visited`、`:hover`、`:active` 的顺序书写，使后面的状态覆盖前面的颜色。焦点轮廓线单独设置

### 在兄弟元素中的位置

```css
.track:first-child { border-color: #1e40af; }
.track:last-child { margin-bottom: 0; }
li:nth-child(odd) { color: #374151; }
li:nth-child(3n) { font-weight: bold; }
```

`:first-child` 要求元素是父元素的第一个子元素：如果第一张卡片前面已有标题，就不满足条件。`:nth-child()` 从 1 开始计算所有兄弟元素的位置；`odd` 表示奇数，`even` 表示偶数

`p:first-of-type` 选择兄弟元素中的第一个 `p`，即使它前面存在标题。`p:nth-of-type(2)` 选择第二个段落

### 选择条件

```css
.track:not(.featured) { border-color: #475569; }
:is(h1, h2, h3) { font-family: Arial, sans-serif; }
:where(.article) p { line-height: 1.6; }
.track:has(.badge) { background-color: #fef3c7; }
```

`:not()` 排除匹配项，`:is()` 合并多个选项，`:where()` 同样合并选项但不增加权重，`:has()` 检查是否存在满足关系的元素。示例中的 `:has()` 为包含 `.badge` 的卡片设置样式

对于教学虚拟机中的旧浏览器，要检查新选择器的支持情况。最初的作业使用类和普通的元素关系即可

## 伪元素

伪元素以两个冒号开头，可以为元素的某个部分设置样式

| 选择器 | 设置样式的区域 |
| --- | --- |
| `p::first-letter` | 第一个字母 |
| `p::first-line` | 显示出来的第一行 |
| `li::marker` | 列表标记 |
| `::selection` | 用户选中的文字 |
| `input::placeholder` | 空输入框中的提示文字 |
| `.badge::before`, `.badge::after` | 在元素开头或结尾生成的内容 |

```css
.badge::before {
  content: "★ ";
  color: #92400e;
}

li::marker {
  color: #1e40af;
}
```

`::before` 和 `::after` 通常需要 `content`，即使值为空字符串。价格、名称和警告等重要信息应保留在 HTML 中，CSS 装饰性内容不能成为它们的唯一来源

## 层叠与选择器权重

层叠为每个属性单独确定值。它考虑规则是否适用、来源和重要性、层叠层、选择器权重以及书写顺序。浏览器、用户和页面作者的规则具有不同优先级，单靠选择器权重无法覆盖优先级更高的来源

对于没有层叠层和 `!important` 的普通作者规则：

1. `style` 属性覆盖 `<style>` 和外部文件中的规则
2. 规则之间比较选择器权重
3. 权重相同时，后面的声明生效

选择器权重按三个数比较：先比较 `id` 的数量，再比较类、属性选择器和伪类的数量，最后比较标签和伪元素的数量

| 选择器 | 权重 |
| --- | --- |
| `h2` | 0-0-1 |
| `.track` | 0-1-0 |
| `.track h2` | 0-1-1 |
| `.track.featured` | 0-2-0 |
| `#recording` | 1-0-0 |

这不是十进制数字：再多的类也不能超过一个 `id`。`*` 和关系组合符不增加权重

`:where()` 连同其参数的权重都为零。`:is()`、`:not()` 和 `:has()` 使用参数中权重最高的选择器的权重，因此某个选项中的 `id` 可能意外增强整条规则

```css
.description {
  color: #166534;
  font-size: 18px;
}

.description {
  color: #374151;
}
```

文字会变为灰色，但仍保留 `18px` 字号。新规则不会整体取消旧规则

机制说明：[层叠](https://developer.mozilla.org/en-US/docs/Web/CSS/Guides/Cascade/Introduction)和[选择器权重](https://developer.mozilla.org/en-US/docs/Web/CSS/Guides/Cascade/Specificity)

### 重要性与层叠层

`color: red !important` 提高声明的优先级，并不是增加选择器权重。在作者样式中，这种声明可以覆盖普通 `style` 属性。重要声明之间也遵循层叠规则。不要把 `!important` 当成解决所有不明原因问题的办法

层叠层可以预先规定不同规则组的顺序：

```css
@layer base, components;

@layer base {
  h2 { color: #374151; }
}

@layer components {
  .track h2 { color: #1e40af; }
}
```

对于普通作者声明，后面的层比前面的层优先，层外规则比层内规则优先。对于 `!important`，层的顺序相反。层叠层适合较大的样式表，作业 4 不需要使用

动画和正在进行的过渡在层叠中有各自的位置：关键帧覆盖普通声明，重要声明覆盖关键帧，而正在进行的过渡甚至优先于重要声明

## 继承与值的重置

`color`、`font-family`、`font-size` 和 `line-height` 通常会继承。因此可以方便地在 `body` 上设置字体；子元素显式设置的值会替代继承值，不受父元素选择器权重的影响

`padding`、`margin` 和边框不会继承。透过透明元素看到父元素背景，并不是继承

| 值 | 作用 |
| --- | --- |
| `inherit` | 使用父元素的属性值 |
| `initial` | 使用规范定义的初始值 |
| `unset` | 可继承属性使用继承值，否则使用初始值 |
| `revert` | 回退当前样式来源的影响，例如回到用户或浏览器样式 |
| `revert-layer` | 回退当前层的影响 |

`initial` 不表示“浏览器通常显示的样子”：浏览器自己的样式表也可能覆盖初始值

```css
button,
input,
textarea,
select {
  font: inherit;
}
```

表单控件经常具有浏览器设置的独立字体。这条规则显式让它们使用父元素字体

## 单位与计算

| 单位 | 依据 |
| --- | --- |
| `px` | CSS 像素，不一定等于屏幕上的一个物理像素 |
| `%` | 取决于属性；普通 `width` 相对于包含块的宽度 |
| `rem` | 根元素 `html` 的字号 |
| `em` | 用于 `font-size` 时相对于父元素字号；用于间距时相对于元素自身字号 |
| `ch` | 当前字体中字符 `0` 的宽度 |
| `vw`, `vh` | 视口宽度或高度的 1% |
| `dvh` | 动态视口高度的 1%，会考虑浏览器工具栏变化 |
| `s`, `ms` | 秒和毫秒 |
| `deg` | 以度为单位的角度 |

默认设置下根字号通常为 `16px`，但用户可以修改。`2rem` 表示根字号的两倍，并不总是 `32px`

零长度可以写成 `0`。`line-height: 1.5` 表示字号的倍数；`line-height: 1.5px` 则是非常小的固定行高

```css
.description {
  max-width: 65ch;
  font-size: clamp(1rem, 0.9rem + 0.5vw, 1.25rem);
}

.cover {
  width: min(100%, 320px);
}
```

`min()` 选择较小值，`max()` 选择较大值，`clamp(最小值, 期望值, 最大值)` 设置上下限。`calc(100% - 2rem)` 用于计算表达式，`+` 和 `-` 两侧需要空格。参与计算的量必须适用于该属性

## CSS 变量

自定义属性以 `--` 开头，通过 `var()` 读取值：

```css
:root {
  --accent: #1e40af;
  --surface: #f3f4f6;
  --radius: 12px;
}

.track {
  color: var(--accent);
  background-color: var(--surface);
  border-radius: var(--radius);
}

.featured {
  --accent: #92400e;
}
```

`:root` 选择文档根元素。普通自定义属性参与层叠并会继承，因此在卡片内覆盖变量也会影响它的后代元素

`var(--accent, #1e40af)` 在变量不存在或具有保证无效的值时提供备用值。但备用值不能修复不匹配的类型：如果 `--accent: 20px`，声明 `color: var(--accent, red)` 不会让颜色变红

变量名区分大小写。变量适合保存配色、圆角和重复使用的间距。[`var()` 说明](https://developer.mozilla.org/en-US/docs/Web/CSS/Reference/Values/var)

## 颜色、背景与透明度

```css
.badge {
  color: #1e40af;
  background-color: rgb(219 234 254 / 80%);
  border: 1px solid currentColor;
}
```

`color` 设置文字颜色，`background-color` 设置背景。`currentColor` 使用计算后的文字颜色，`transparent` 表示完全透明的颜色

颜色可以写成名称（`white`）、HEX（`#ffffff`，简写为 `#fff`）、`rgb()` 或 `hsl()` 函数。例如 `hsl(220 70% 40%)` 指定色相、饱和度和明度。`/` 后的 alpha 通道控制颜色的透明度

`opacity: 0.5` 让整个元素连同后代一起半透明。如果需要背景半透明、文字不透明，应只在背景颜色中使用 alpha 通道。`opacity: 0` 不会删除元素，也不会自动将其排除在键盘导航之外

### 背景图片与渐变

```css
.album-note {
  background-color: #172554;
  background-image: linear-gradient(135deg, #172554, #3730a3);
  color: white;
}

.texture {
  background-image: url("../assets/texture.jpg");
  background-repeat: no-repeat;
  background-position: center;
  background-size: cover;
}
```

`cover` 填满区域，可能裁剪图片；`contain` 显示完整图片，可能留下空白。渐变属于背景图片；`radial-gradient()` 创建径向渐变

背景可以包含多个用逗号分隔的图片，第一张绘制在其他图片上方。有实际信息意义的插图应使用带有替代文本的 HTML 图片，CSS 背景没有 `alt`

应在实际背景上检查可读性，包括图片和透明效果。不要只用颜色传达含义，也不要通过 `opacity` 让所有文字变淡

## 字体与文字

| 属性 | 示例 | 用途 |
| --- | --- | --- |
| `font-family` | `Arial, sans-serif` | 按优先顺序排列的字体 |
| `font-size` | `1rem` | 字号 |
| `font-weight` | `400`, `700`, `normal`, `bold` | 字重 |
| `font-style` | `normal`, `italic` | 正体或斜体 |
| `line-height` | `1.6` | 行高 |
| `letter-spacing` | `0.03em` | 额外的字符间距 |
| `word-spacing` | `0.1em` | 额外的单词间距 |
| `text-align` | `left`, `center`, `right` | 块内文字的对齐方式 |
| `text-indent` | `1.5em` | 首行缩进 |
| `text-transform` | `uppercase`, `lowercase`, `capitalize` | 显示时的大小写转换 |
| `text-decoration` | `underline`, `line-through`, `none` | 下划线、删除线或不加装饰线 |

`text-align: center` 不会使块本身居中。`text-transform` 不改变 HTML 源文本。`capitalize` 改变单词的首字母，而不会纠正姓名的拼写

如果没有第一种字体，浏览器会尝试下一种。列表最后应使用通用字体族：`serif` 有衬线，`sans-serif` 无衬线，`monospace` 字符等宽。含空格的字体名称应加引号

`font-weight: 700` 通常表示粗体，但可用的字重取决于字体文件。并非每种字体都包含西里尔字母或汉字

```css
.description {
  font: 400 1rem/1.6 Arial, sans-serif;
}
```

`font` 简写至少需要字号和字体族，并会将未指定的组成部分重置为初始值

### 换行与长文本

- `white-space: normal` 允许普通换行，并合并连续空白
- `white-space: nowrap` 禁止自动换行
- `white-space: pre-wrap` 保留空格和原始换行，同时允许长行自动换行
- `overflow-wrap: anywhere` 允许在无法容纳时拆分长 URL 或单词
- `hyphens: auto` 在存在文档语言词典时启用自动断词

不要为长标题设置过小的固定高度。用户放大文字时，内容不应被隐藏

### 引入字体

```css
@font-face {
  font-family: "Album Sans";
  src: url("../assets/fonts/album-sans.woff2") format("woff2");
  font-weight: 400;
  font-style: normal;
  font-display: swap;
}

body {
  font-family: "Album Sans", Arial, sans-serif;
}
```

示例中的字体文件必须存在于指定路径。`font-display: swap` 允许先显示备用字体。单独的粗体文件需要另一条规则，并指定对应的 `font-weight`。请使用获得网页使用许可的字体。[`@font-face` 说明](https://developer.mozilla.org/en-US/docs/Web/CSS/Reference/At-rules/@font-face)

## 尺寸与盒模型

内容外面是内边距 `padding` 和边框 `border`。外边距 `margin` 将元素与相邻元素分开

```css
.track {
  width: 100%;
  max-width: 480px;
  box-sizing: border-box;
  padding: 16px;
  border: 2px solid #475569;
  margin-bottom: 24px;
}
```

在 `content-box` 下，`width: 480px` 指的是内容宽度。加上两侧各 `16px` 的内边距和各 `2px` 的边框，总宽度为 `516px`。在 `border-box` 下，这些都包含在 `480px` 内，内容宽度剩余 `444px`。两种情况下外边距 `margin` 都不包含在宽度中

通常为所有元素及伪元素统一设置该模式：

```css
*,
*::before,
*::after {
  box-sizing: border-box;
}
```

`width` 和 `height` 设置尺寸，`min-width` 和 `min-height` 设置最小值，`max-width` 和 `max-height` 设置最大值。`height: auto` 允许高度由内容决定。百分比高度需要包含块具有适当的确定高度，并不总是表示窗口高度的百分比

### 间距的简写

| 写法 | 各边 |
| --- | --- |
| `padding: 12px` | 所有边 |
| `padding: 12px 20px` | 上/下、左/右 |
| `padding: 12px 20px 8px` | 上、左/右、下 |
| `padding: 12px 20px 8px 4px` | 上、右、下、左 |

`margin` 的顺序相同。单边使用 `padding-left`、`margin-top` 等属性。`padding` 不能为负数；`margin` 可以为负数，但初期作业不需要使用

普通块级流中的垂直外边距可能折叠：相邻的两个正外边距不一定相加。文档流规则将在 CSS Layout 中详细讨论

逻辑属性考虑书写方向：`padding-inline` 沿行方向设置内边距，`margin-block` 沿垂直于行的方向设置外边距。对于普通横排俄语文本，它们分别对应左/右和上/下

### 溢出

`overflow: visible` 允许内容显示在边界外；`hidden` 裁剪可见区域；`auto` 在需要时提供滚动。可以通过 `overflow-x` 和 `overflow-y` 分别设置两个轴

不要通过裁剪来解决文本丢失问题。先检查宽度、高度、换行和间距。元素的显示类型也影响尺寸属性的应用；`display` 将在 CSS Layout 中讲解

## 边框、轮廓线与阴影

```css
.track {
  border: 2px solid #475569;
  border-radius: 12px;
  box-shadow: 0 4px 12px rgb(0 0 0 / 15%);
}
```

`border` 指定粗细、线型（`solid`、`dashed`、`dotted`）和颜色。`border-left` 改变一条边，`border-color` 改变颜色。如果没有可见线型，只指定颜色还不够

`border-radius` 设置圆角。在正方形图片上使用 `50%` 可以显示成圆形

`box-shadow` 中依次指定水平偏移、垂直偏移、模糊和颜色，还可以指定扩散距离以及用于内部阴影的 `inset`。阴影不占据文档流中的空间。`text-shadow` 为文字添加阴影，但过度模糊会降低可读性

`outline` 同样不占据文档流空间，尤其适合标记焦点。`outline-offset` 设置轮廓线与元素边缘之间的距离

## 图片

保持比例：

```css
.cover {
  width: 100%;
  max-width: 320px;
  height: auto;
  border-radius: 8px;
}
```

制作尺寸相同的正方形缩略图：

```css
.thumbnail {
  width: 160px;
  height: 160px;
  object-fit: cover;
  object-position: center;
}
```

`object-fit: cover` 裁剪多余部分，`contain` 在指定矩形内完整显示图片。`object-position` 指定图片的位置，例如 `50% 30%`

`aspect-ratio: 1 / 1` 设置首选宽高比，在其中一个尺寸自动确定时很有用。如果宽高都已固定，它不会覆盖这些值

`filter: grayscale(100%)` 将图片变为灰度，`brightness(0.8)` 降低亮度。滤镜改变显示效果，而不改变原始文件；用于重要信息时必须检查可读性

## 列表与表格

```css
.influences {
  list-style-type: square;
  padding-left: 24px;
}

.influences li {
  margin-bottom: 10px;
}
```

`list-style-position: outside` 将标记放在列表项主要文本块之外，`inside` 则放在内部。`list-style-type: none` 移除标记，但不会把列表变成普通文本

表格也使用相同的颜色、间距和边框属性设置外观：

```css
table {
  border-collapse: collapse;
}

th,
td {
  border: 1px solid #64748b;
  padding: 8px 12px;
  text-align: left;
  vertical-align: top;
}

thead {
  background-color: #e2e8f0;
}

tbody tr:nth-child(even) {
  background-color: #f8fafc;
}

caption {
  text-align: left;
  font-weight: bold;
  margin-bottom: 8px;
}
```

`border-collapse: collapse` 合并相邻边框。使用 `separate` 时，单元格间距通过 `border-spacing` 设置。行背景不能代替 `th` 表头、`scope` 和表格标题

## 链接、按钮与表单控件

链接用于跳转，按钮用于执行操作。外观不会改变 HTML 元素的用途

```css
a {
  color: #1e40af;
  text-decoration: underline;
  text-underline-offset: 0.15em;
}

a:hover {
  color: #7c2d12;
}

a:focus,
button:focus,
input:focus {
  outline: 2px solid #1e40af;
  outline-offset: 3px;
}
```

不要在没有可见替代样式的情况下移除焦点标识。不要让必要信息只在悬停状态下出现，因为触摸屏可能没有悬停状态

```css
button {
  font: inherit;
  color: white;
  background-color: #1e40af;
  border: 1px solid #1e40af;
  border-radius: 8px;
  padding: 10px 16px;
  cursor: pointer;
}

button:disabled {
  background-color: #475569;
  cursor: not-allowed;
}

input,
textarea,
select {
  font: inherit;
  border: 1px solid #64748b;
  padding: 8px;
}

input[type="checkbox"],
input[type="radio"] {
  accent-color: #1e40af;
}
```

`not-allowed` 光标不会禁用按钮，必须使用 HTML 属性 `disabled`。`placeholder` 提示不能代替 `label` 标签。输入错误应通过文字说明，而不只是使用红色边框

`appearance: none` 移除控件的部分原生外观；只有准备好重新提供清晰状态时才应使用。初期作业保留标准行为

## 过渡、变换与动画

### 状态之间的过渡

```css
button {
  transition: background-color 180ms ease, color 180ms ease;
}

button:hover {
  background-color: #172554;
}
```

`transition` 设置在普通状态上，使两个方向的变化都平滑。写法包含属性、时长和速度变化函数；额外的时间值表示延迟

应指定具体属性，而不是使用 `transition: all`。并非所有属性都能平滑插值，初期过渡练习不建议使用 `auto` 尺寸

### 变换

```css
.cover {
  transition: transform 180ms ease;
}

.cover:hover {
  transform: scale(1.03) rotate(-1deg);
}
```

`scale()` 缩放，`rotate()` 旋转，`translate()` 改变显示位置。变换不会重新分配相邻元素之间的空间，因此元素可能覆盖邻居。函数顺序会影响结果，变换原点通过 `transform-origin` 设置

### 关键帧

```css
@keyframes appear {
  from { opacity: 0.5; }
  to { opacity: 1; }
}

.cover {
  animation: appear 400ms ease-out;
}
```

`@keyframes` 描述各阶段，`animation` 将其应用于元素。除名称、时长和速度函数外，还可以设置延迟、重复次数、方向和状态保留方式。`infinite` 表示无限重复，`animation-fill-mode: forwards` 在结束后保留最终关键帧的值

不要用持续运动或闪烁来装饰教学页面

```css
@media (prefers-reduced-motion: reduce) {
  .cover {
    animation: none;
    transition: none;
  }

  .cover:hover {
    transform: none;
  }
}
```

这会考虑系统中减少动态效果的偏好。示例单独取消悬停时的变换：只关闭 `transition` 会取消平滑过程，但仍然放大图片

[prefers-reduced-motion 说明](https://developer.mozilla.org/en-US/docs/Web/CSS/Reference/At-rules/@media/prefers-reduced-motion)

## 样式的应用条件

`@media` 在条件满足时应用规则。除屏幕尺寸外，也可以考虑打印或配色偏好：

```css
@media print {
  body {
    color: black;
    background-color: white;
  }
}

@media (prefers-color-scheme: dark) {
  :root {
    --surface: #1f2937;
  }
}
```

深色主题需要协调所有相关元素的背景和文字颜色，仅替换一个变量可能不够。屏幕宽度条件和断点设计将在响应式布局理论中讲解

`@supports` 检查属性语法是否受支持：

```css
@supports (color: oklch(50% 0.1 250)) {
  .accent {
    color: oklch(50% 0.1 250);
  }
}
```

基础样式放在条件外，增强效果放在条件内。语法受支持并不保证完全没有浏览器差异

## CSS 的组织与检查

按用途组织规则：变量、通用样式、元素及其状态。优先使用短选择器和表达用途的类名。不要仅为了覆盖旧规则而增加新的 `id` 或 `!important`

在 `Elements` 中选中元素。`Styles` 面板显示匹配的规则、来源文件与行号，以及被划掉的覆盖声明。无效值通常会显示警告

`Computed` 显示最终值和尺寸示意图。标题、段落和列表具有浏览器默认样式，因此即使没有自己的 CSS，也可能出现间距

如果样式没有应用，请依次检查：

1. `Network` 中是否加载了正确的 `.css`，是否出现 `404`，服务器是否正确返回 CSS 而不是 HTML
2. 选择器是否匹配元素的标签、类、标识符或状态
3. 属性和值是否合法，大括号是否闭合
4. 声明是否被另一条规则、属性或简写属性覆盖
5. 该属性是否适用于当前元素，是否受到尺寸限制影响

DevTools 中的修改是临时的，要将需要的值写回源文件。怀疑缓存有问题时，可以在开发者工具中禁用缓存后重新加载页面

不要只检查普通状态，还要检查长文本、页面缩放、键盘导航和系统减少动态效果的设置。外观不应妨碍阅读和操作
