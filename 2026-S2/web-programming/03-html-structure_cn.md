# HTML：文档结构与表格

实验作业 03a 需要学习表格之前的各部分；`表格` 部分将在实验作业 03b 中使用

## 语义区域

语义元素说明内容的用途。屏幕阅读器使用页面区域进行导航

| 元素      | 用途                           |
| --------- | ------------------------------ |
| `header`  | 页面或独立内容的引导部分       |
| `nav`     | 主要导航区域                   |
| `main`    | 页面的主要内容                 |
| `footer`  | 结尾信息，例如作者信息         |
| `section` | 带有标题的主题部分             |
| `article` | 独立内容：课程介绍、新闻或卡片 |
| `div`     | 没有特定语义的元素分组         |

简单文档使用一个 `main`；公共页头和页脚放在它的外部。`article` 可以包含多个 `section`。标题级别根据内容的层级关系选择

如果 `main` 设置了 `id="content"`，在页头之前添加 `<a href="#content">跳转到正文</a>` 链接就能跳过菜单。`Tab` 选中下一个链接，`Shift + Tab` 选中上一个，`Enter` 激活链接

## 多个页面与路径

每个页面都是独立的 HTML 文档，拥有自己的 `head`、`body` 和 `title` 名称。普通 HTML 文件中的公共部分不会自动同步

相对路径以当前文档所在的文件夹为起点：

```text
site/
|-- index.html
|-- week2.html
`-- subjects/
    `-- drawing.html
```

| 起点                    | 目标   | `href` 的值             |
| ----------------------- | ------ | ----------------------- |
| `index.html`            | 第二周 | `week2.html`            |
| `week2.html`            | 素描   | `subjects/drawing.html` |
| `subjects/drawing.html` | 第一周 | `../index.html`         |

`../` 表示向上一级文件夹。以 `/` 开头的路径从网站根目录开始计算

链接 `../week2.html#monday` 打开另一个页面，并跳转到具有 `id="monday"` 的元素

`上一页` 和 `下一页` 是明确指定目标地址的普通链接，不依赖浏览器历史记录

## 属性列表、图注与联系方式

`dl` 将属性名称 `dt` 和对应的值 `dd` 组织在一起：

```html
<dl>
  <dt>授课形式</dt>
  <dd>实践课</dd>
  <dt>需要携带的物品</dt>
  <dd>画册和铅笔</dd>
</dl>
```

`figure` 将插图与图注 `figcaption` 组合在一起，`figcaption` 位于第一个或最后一个子元素的位置。图注对访客可见，但不能代替 `alt`

`address` 包含与当前内容有关的作者或组织的联系方式。单纯的活动地点地址不一定需要使用此元素

链接 `<a href="mailto:hello@example.com">写邮件</a>` 将地址传递给已配置的邮件应用，但不会自动发送邮件

## 表格

表格用于展示相互关联的数据，例如课程表，而不是用于安排网站各个区块的位置

| 元素      | 用途                                  |
| --------- | ------------------------------------- |
| `table`   | 表格                                  |
| `caption` | 表格标题，紧接在 `table` 开始标签之后 |
| `thead`   | 表头行分组                            |
| `tbody`   | 数据行分组                            |
| `tr`      | 行                                    |
| `th`      | 表头单元格                            |
| `td`      | 数据单元格                            |
| `tfoot`   | 可选的汇总行，例如总金额              |

```html
<table>
  <caption>
    10 月 5 日，星期一
  </caption>
  <thead>
    <tr>
      <th scope="col">时间</th>
      <th scope="col">课程</th>
      <th scope="col">教室</th>
      <th scope="col">教师</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>08:30 - 10:00</td>
      <td><a href="subjects/drawing.html">素描</a></td>
      <td>1410</td>
      <td>伊万诺娃·安娜·谢尔盖耶芙娜</td>
    </tr>
  </tbody>
</table>
```

单元格放在行内。链接、段落和列表放在单元格中，而不是放在行与行之间

`scope="col"` 将表头与列关联，`scope="row"` 将表头与行关联。这能帮助屏幕阅读器解释数据。`td` 中的粗体文字不能代替 `th`

### 合并单元格

`colspan` 指定占用的列数，`rowspan` 指定占用的行数

对于没有课的日子，保留 `caption` 和 `thead`，在 `tbody` 中添加：

```html
<tr>
  <td colspan="4">当天无课</td>
</tr>
```

该单元格占据四列，不需要额外的空单元格。设置 `rowspan="2"` 时，单元格还会占据下一行的对应位置，因此下一行应省略该位置的单元格

### 不使用 CSS 时的外观

浏览器会排列行和列，通常用粗体显示 `th`，并在表格上方显示 `caption`。宽度取决于内容

默认不显示网格线。边框、内边距和背景通过 CSS 设置。不要使用已弃用的 `border`、`cellpadding`、`cellspacing` 和 `bgcolor` 属性

更多说明：[MDN 表格文档](https://developer.mozilla.org/en-US/docs/Web/HTML/Reference/Elements/table)
