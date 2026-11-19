# CSS 响应式布局

## 视口

响应式布局会随着可用宽度的变化，调整各区块的尺寸和排列方式

Viewport（视口）是页面的可视区域，以 CSS 像素为单位衡量

在页面的 `head` 中设置

```html
<meta name="viewport" content="width=device-width, initial-scale=1" />
```

`width=device-width` 使布局视口的宽度与设备相匹配，`initial-scale=1` 设置初始缩放比例。应保留用户通过手势放大页面的能力。[MDN 视口说明](https://developer.mozilla.org/en-US/docs/Web/HTML/Reference/Elements/meta/name/viewport)

## 灵活的尺寸

容器应适应窗口宽度，同时设置最大宽度以便阅读

```css
* {
  box-sizing: border-box;
}

body {
  margin: 0;
}

.container {
  width: calc(100% - 32px);
  max-width: 1120px;
  margin-inline: auto;
}

img {
  display: block;
  max-width: 100%;
  height: auto;
}
```

窗口宽度为 `360px` 时，容器宽度为 `328px`；窗口宽度为 `1280px` 时，容器达到最大宽度 `1120px`

图片缩小时保持原有比例。灵活的尺寸在布局断点之间也能发挥作用。[MDN 响应式设计说明](https://developer.mozilla.org/en-US/docs/Learn_web_development/Core/CSS_layout/Responsive_Design)

## 媒体查询与 mobile-first

媒体查询 `@media` 在满足条件时应用 CSS。断点（breakpoint）是布局发生变化的宽度边界

Mobile-first（移动优先）指先编写适合窄屏的基础样式，再为更宽的屏幕添加调整。以下示例适用于包含三张卡片的 `cards` 容器

```css
.cards {
  display: grid;
  grid-template-columns: minmax(0, 1fr);
  gap: 16px;
}

@media (min-width: 640px) {
  .cards {
    grid-template-columns: repeat(2, minmax(0, 1fr));
  }
}

@media (min-width: 1024px) {
  .cards {
    grid-template-columns: repeat(3, minmax(0, 1fr));
    gap: 24px;
  }
}
```

宽度为 `768px` 时，基础规则和第一个媒体查询生效。宽度为 `1280px` 时，两个媒体查询都生效，使用相同选择器的后一个声明将布局设置为三列

`min-width` 包含指定的边界值。根据内容选择断点，当卡片有足够空间增加一列时调整布局。[MDN 媒体查询说明](https://developer.mozilla.org/en-US/docs/Learn_web_development/Core/CSS_layout/Media_queries)

## 区块布局的调整

图片和介绍可以从单列布局调整为双列布局

```css
.feature {
  display: grid;
  grid-template-columns: minmax(0, 1fr);
  gap: 24px;
}

@media (min-width: 640px) {
  .feature {
    grid-template-columns: repeat(2, minmax(0, 1fr));
    align-items: center;
  }
}
```

为导航设置 `display: flex` 和 `flex-wrap: wrap`，使链接可以换行。HTML 元素的顺序决定阅读顺序和按 Tab 键时的焦点移动顺序

## 文字与空间不足

文字换行时，卡片高度应随之增加。包含名称和价格的一行可以这样设置

```css
.menu-row {
  display: flex;
  align-items: baseline;
  gap: 16px;
}

.menu-name {
  flex: 1;
  min-width: 0;
  overflow-wrap: anywhere;
}

.price {
  flex-shrink: 0;
  white-space: nowrap;
}
```

名称占据剩余空间，`min-width: 0` 允许它收缩。长单词可以换行，价格保持完整

文字大小和间距也可以在媒体查询中调整

## 在浏览器中检查

在 Chrome 开发者工具中点击手机和平板图标，选择 `Responsive` 模式

1. 设置窗口宽度，检查断点两侧及边界处的布局，例如 `639px`、`640px` 和 `641px`
2. 连续调整宽度，观察换行和图片的变化
3. 在 `Elements` 中选择出现问题的元素，在 `Styles` 中找到对应的媒体查询，在 `Computed` 中查看最终宽度

如果出现横向滚动，检查固定宽度、`min-width`、长单词和图片尺寸。使用 `overflow-x: hidden` 裁切可能会隐藏超出屏幕边界的内容

退出设备模式，通过浏览器菜单将页面缩放比例提高到 `200%`。以 CSS 像素计算的可用宽度会减小，布局可能随之调整。检查文字、价格和链接，然后将缩放比例恢复为 `100%`
