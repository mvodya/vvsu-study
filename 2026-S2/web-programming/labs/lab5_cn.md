# 实验作业 5：CSS 图形、宾果与课程表

## 作业内容

本作业安排两次课，分为三个部分：用 HTML 元素绘图、制作主题宾果，以及为课程表添加样式

第一次课完成绘图并开始制作宾果，第二次课完成宾果并为课程表添加样式

## 项目

创建 `lab5-css` 文件夹，结构如下

```text
lab5-css/
|-- index.html
|-- bingo.html
|-- css/
|   `-- styles.css
|-- assets/
|   `-- 宾果图片
`-- schedule/
    |-- index.html
    |-- week2.html
    |-- week3.html
    `-- subjects/
        |-- subject1.html
        |-- subject2.html
        `-- subject3.html
```

在绘图页和宾果页添加 `绘图`、`宾果`、`课程表` 链接；在课程表各页添加返回绘图页的链接，保留各周与课程介绍之间的导航

将项目的全部样式写入同一个 `css/styles.css`，通过 `link` 引入每个 HTML 文件；编写路径时注意文件夹层级

本作业不使用 JavaScript、CSS 库、Flexbox、Grid 或媒体查询

## 第一部分：用 div 绘图

在 `index.html` 中选择一种图案；下表列出必需部件的最少数量，不包含整个场景容器

| 图案   | 必需部件                                                                     | 嵌套关系                                                                     |
| ------ | ---------------------------------------------------------------------------- | ---------------------------------------------------------------------------- |
| 雪人   | 三个大小不同的雪球圆形、两只眼睛、一个鼻子、两只手臂、三颗纽扣，共 11 个部件 | 眼睛和鼻子放在最上方的圆形内，手臂和纽扣放在中间圆形内，三个圆形都放在场景内 |
| 笑脸   | 一个圆形脸部、两只眼睛、两个瞳孔、一张嘴、两条眉毛，共 8 个部件              | 眼睛、嘴和眉毛放在脸部内，每个瞳孔放在对应眼睛内                             |
| 小房子 | 墙体、屋顶、门、门把手、两扇窗、两根窗框横档，共 8 个部件                    | 墙体和屋顶放在场景内；门和窗放在墙体内，把手放在门内，每扇窗内放一根横档     |
| 小猫   | 身体、头、两只耳朵、两只眼睛、鼻子、两只爪子、尾巴，共 10 个部件             | 头、爪子和尾巴放在身体内；耳朵、眼睛和鼻子放在头内                           |

1. 每个部件使用独立的 `div`，并设置有意义的类名；部件不使用图片、字符、SVG 或伪元素代替
2. 场景尺寸为 `400px` × `400px`，设置 `position: relative`；内部部件使用 `position: absolute`，通过 `top`、`left`、`right` 或 `bottom` 定位
3. 使用 `width` 和 `height` 设置部件尺寸，至少使用三种 `background-color` 值，至少两个部件使用 `border-radius`；三角形可以使用零尺寸和有颜色的边框
4. 重复部件使用公共类，不同坐标使用附加类，例如 `eye eye-left`
5. 眼睛的坐标应相对于头部，把手的坐标相对于门，其他嵌套部件依此类推；所有部件都应位于场景范围内
6. 为场景添加 `role="img"`，并通过 `aria-label` 简短描述图案

### 示例：小人

以下完整页面演示嵌套和坐标；自己的作业应完成上面四种图案中的一种

眼睛放在头部内，手臂和腿放在躯干内；头部和躯干的 `absolute` 定位也使其成为内部部件的定位参照

```html
<!doctype html>
<html lang="zh-CN">
  <head>
    <meta charset="utf-8" />
    <title>用 div 绘制的小人</title>
    <style>
      .person {
        position: relative;
        width: 240px;
        height: 320px;
        background-color: #eef2ff;
      }
      .head {
        position: absolute;
        top: 20px;
        left: 80px;
        width: 80px;
        height: 80px;
        border-radius: 50%;
        background-color: #fbbf24;
      }
      .eye {
        position: absolute;
        top: 28px;
        width: 10px;
        height: 10px;
        border-radius: 50%;
        background-color: #1f2937;
      }
      .eye-left {
        left: 20px;
      }
      .eye-right {
        right: 20px;
      }
      .torso {
        position: absolute;
        top: 100px;
        left: 115px;
        width: 10px;
        height: 110px;
        background-color: #1f2937;
      }
      .arm,
      .leg {
        position: absolute;
        width: 10px;
        border-radius: 5px;
        background-color: #1f2937;
        transform-origin: top center;
      }
      .arm {
        top: 12px;
        left: 0;
        height: 75px;
      }
      .arm-left {
        transform: rotate(50deg);
      }
      .arm-right {
        transform: rotate(-50deg);
      }
      .leg {
        top: 100px;
        left: 0;
        height: 90px;
      }
      .leg-left {
        transform: rotate(25deg);
      }
      .leg-right {
        transform: rotate(-25deg);
      }
    </style>
  </head>
  <body>
    <h1>用 div 绘制的小人</h1>
    <div
      class="person"
      role="img"
      aria-label="小人：圆形头部、两只眼睛、手臂和腿"
    >
      <div class="head">
        <div class="eye eye-left"></div>
        <div class="eye eye-right"></div>
      </div>
      <div class="torso">
        <div class="arm arm-left"></div>
        <div class="arm arm-right"></div>
        <div class="leg leg-left"></div>
        <div class="leg leg-right"></div>
      </div>
    </div>
  </body>
</html>
```

[打开小人的 HTML 示例](../assets/css-layout/person_cn.html)

## 第二部分：设计专业学生宾果

在 `bingo.html` 中制作 25 个格子的棋盘，共五列、五行

1. 使用一个 `div` 容器和其中的 25 个 `div` 格子，格子使用同一个公共类，顺序为从左到右、从上到下；不使用 `table`、单独的行容器或 `br`
2. 格子使用 `display: inline-block`，填满容器宽度后自动换行，设置 `vertical-align: top`
3. 棋盘宽度为 `700px`，每个格子连同边框和内边距的尺寸为 `140px` × `140px`；使用 `box-sizing: border-box`
4. 消除 `inline-block` 之间空白字符的影响：容器设置 `font-size: 0`，格子恢复为 `font-size: 14px`；不要添加会影响五列宽度计算的外边距
5. 每个格子包含文字和一张通过 `img` 引入的独立图片：使用含义相关的本地照片或图标，尺寸为 `40px` × `40px`；至少使用五个不同文件，不使用表情符号代替图片
6. 可以使用下表中的文字，也可以在同一主题下自行替换；图片应在含义上补充文字
7. 为格子设置边框、内边距、内容居中和易读的文字颜色；使用 `:hover` 改变任意格子的 `background-color`
8. 如果图片重复表达文字的含义，设置 `alt=""`；如果图片提供额外信息，在 `alt` 中描述

| 1              | 2              | 3                | 4            | 5                |
| -------------- | -------------- | ---------------- | ------------ | ---------------- |
| 打开了编辑器   | 忘记保存       | 找到了合适的字体 | 画了草图     | 喝了茶           |
| 选好了配色     | 重命名了图层   | 收到了修改意见   | 导出了 PNG   | 休息了一下       |
| 调整了间距     | 找到了参考素材 | 完成了设计稿     | 检查了对比度 | 保存了副本       |
| 去掉了多余颜色 | 展示了作品     | 画了图标         | 修正了错别字 | 整理了文件夹     |
| 重读了作业要求 | 引入了 CSS     | 检查了链接       | 提交了作业   | 合上了笔记本电脑 |

这是一个视觉宾果页面：无需实现点击标记或保存状态；本部分不提供现成代码

## 第三部分：课程表样式

将实验作业 03b 的成果复制到 `schedule`，或将[现成的 HTML 模板](../assets/lab5-schedule/schedule-starter.zip)解压到该文件夹；保留原来的 03b 作业

使用全部三个周页面和三个课程介绍页面；保留表格内容、`caption`、列标题、无课日的合并单元格以及原有链接

在公共样式文件中设置以下内容

1. 统一的字体、文字颜色和页面背景，以及 `header`、`main`、`footer` 的内边距
2. 主要内容宽度为 `960px`，水平外边距为 `auto`
3. 表格宽度为 `100%`，设置 `border-collapse: collapse` 和 `24px` 下外边距
4. `th` 和 `td` 设置 `1px solid` 边框、`12px` 内边距和 `top` 垂直对齐
5. 列标题使用单独的背景和对比清晰的文字颜色；`caption` 左对齐、加粗，设置 `8px` 下外边距
6. 通过 `:nth-child(even)` 为 `tbody` 的偶数行设置另一种背景；鼠标悬停于行时改变该行所有单元格的背景，使偶数行的悬停效果也能清楚显示
7. 为 `Пар нет`（无课）单元格添加独立类：文字居中、斜体，并使用专门的背景
8. 时间列通过 `white-space: nowrap` 禁止换行，教室编号居中
9. 导航链接设置内边距、边框和 `display: inline-block`；为 `:hover` 和 `:focus-visible` 设置明显的样式，保留可见的焦点标记
10. 使用同一个 CSS 文件统一课程介绍页的标题、列表以及 `dt` / `dd` 样式

限定规则的作用范围，例如在 `body` 上设置类，使课程表样式只作用于相应页面

## 检查与答辩

通过本地服务器打开项目，检查全部导航、图片和 CSS 的加载、宾果每行五个格子的排列，以及悬停状态

在开发者工具中展示绘图部件的嵌套关系，以及样式规则来自哪个文件

答辩时，教师可能要求修改公共背景、格子悬停颜色、绘图部件的尺寸或位置；请修改源 CSS 并刷新页面，对于公共规则，在两个页面上检查效果
