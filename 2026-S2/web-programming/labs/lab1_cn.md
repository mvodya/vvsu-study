# 实验作业 1：本地 Web 服务器和第一个页面

## 作业内容

创建一个包含姓名、班级和图片的个人页面，并通过本地 Web 服务器打开它

完成作业需要 Python 3 和代码编辑器

## 项目结构

创建文件夹 `lab1-server`，其结构如下

```text
lab1-server/
|-- index.html
`-- assets/
    |-- avatar.jpg
    `-- my-image.jpg
```

文件夹和文件的名称必须与图示完全一致，包括字母大小写

在 Windows 中启用文件扩展名显示，或者检查文件名是否被保存为 `index.html.txt`

## 页面

在代码编辑器中打开 `lab1-server` 文件夹，创建 `index.html` 文件，并将以下代码复制到文件中

```html
<!doctype html>
<html lang="zh-CN">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>课程参与者</title>
  </head>
  <body>
    <h1>姓名</h1>
    <img src="assets/avatar.jpg" alt="课程参与者头像" width="240" />
    <p>班级：AAA-XX-XX</p>
    <p>我的第一个页面正在本地 Web 服务器上运行</p>
  </body>
</html>
```

将 `姓名` 和 `AAA-XX-XX` 替换为自己的信息

在 `title` 元素中，将文本替换为 `姓名 - 课程参与者`，并填写自己的姓名

选择一张至少为 240 x 240 像素的 JPEG 格式正方形图片，将其以 `avatar.jpg` 为文件名保存，并放入 `assets` 文件夹

## 自选图片

再选择一张任意主题的 JPEG 格式图片，该图片应与头像不同

将图片以 `my-image.jpg` 为文件名保存到 `assets` 文件夹中

在最后一个段落后添加图片元素

```html
<img src="assets/my-image.jpg" alt="图片描述" width="400" />
```

将 `图片描述` 替换为所选图片的简短描述

使用 UTF-8 编码保存 `index.html`

## 在 Windows 中启动

打开命令提示符 - 按 `Win + R`，输入 `cmd`，然后按 `Enter`

在文件资源管理器中打开 `lab1-server` 文件夹，并从地址栏复制它的路径

在命令提示符中输入 `cd /d`，添加一个空格，然后粘贴带引号的路径，例如

```bat
cd /d "C:\Users\Student\Desktop\lab1-server"
```

检查 Python 版本

```bat
python --version
```

如果命令显示 Python 3 的版本，请启动服务器

```bat
python -m http.server 8000 --bind 127.0.0.1
```

如果找不到 `python` 命令，或者该命令显示 Python 2 的版本，请使用 Python Launcher

```bat
py -3 -m http.server 8000 --bind 127.0.0.1
```

## 在 macOS 中启动

通过 Spotlight 打开 Terminal - 按 `Command + Space`，输入 `Terminal`，然后按 `Enter`

输入 `cd` 和一个空格，将 `lab1-server` 文件夹从 Finder 拖到 Terminal 窗口中，然后按 `Enter`

检查 Python 版本

```bash
python3 --version
```

启动服务器

```bash
python3 -m http.server 8000 --bind 127.0.0.1
```

## 检查

不要关闭运行服务器的窗口，在浏览器中打开以下地址

```text
http://127.0.0.1:8000/
```

页面上应显示你的姓名、班级和两张图片

页面地址必须以 `http://127.0.0.1:` 开头，而不是以 `file://` 开头

服务器窗口中应出现请求 `GET /`、`GET /assets/avatar.jpg` 和 `GET /assets/my-image.jpg`，并且没有 `404` 状态码

## 修改正在运行的页面

不要停止服务器，将最后一个段落的文本替换为以下内容

```text
无需重新启动服务器即可更新页面
```

保存文件并在浏览器中刷新页面

页面上应出现新文本，服务器窗口中应再出现一个请求 `GET /`

## 检查 404 状态码

在浏览器中打开一个不存在的地址

```text
http://127.0.0.1:8000/missing.html
```

如果服务器运行在备用端口 `8080` 上，也需要在地址中指定这个端口

浏览器应显示错误消息，服务器窗口中应出现请求 `GET /missing.html`，状态码为 `404`

返回主页并确认它仍然可以正常工作

要停止服务器，请返回运行服务器的窗口并按 `Ctrl + C`

## 如果服务器无法启动

如果端口 `8000` 已被占用，请在启动命令中将 `8000` 替换为 `8080`，并打开 `http://127.0.0.1:8080/`

如果显示文件列表，请检查启动服务器的文件夹中是否存在 `index.html`

如果图片未显示，请检查路径 `assets/avatar.jpg` 和 `assets/my-image.jpg`、文件名、扩展名以及图片的实际格式
