# Лабораторная работа №3 - Основы CSS

**Цель:** научиться применять CSS-стили для оформления HTML-страницы, управлять цветом, шрифтами, размерами и расположением элементов.

---

## Первая часть задания - "Рисуем с помощью CSS"

### Задание

Создайте **одностраничный сайт** (`art.html`), на котором с помощью HTML и CSS вы изобразите простую фигуру или персонажа.

Например:

* Снеговика
* Колобка
* Смайлик
* Домик
* Светофор
* Котика (по желанию)

Для построения используйте только **`<div>`** и CSS-свойства.

### Требования

1. Каждый элемент (глаза, тело, рот и т.д.) должен быть отдельным `<div>` с собственным классом
2. Размер, цвет, форма задаются с помощью CSS
3. Обязательно использовать
   * `border-radius` (скругления)
   * `background-color`
   * `position: relative` / `absolute`
   * `width`, `height`
4. Страница должна быть аккуратной и иметь фон

### Пример (снеговик)

```html
<!DOCTYPE html>
<html lang="ru">

<head>
  <meta charset="UTF-8">
  <title>Снеговик</title>
  <style>
    body {
      background-color: #dff3ff;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
    }

    .snowman {
      position: relative;
      display: flex;
      flex-direction: column;
      align-items: center;
    }

    .ball {
      border-radius: 50%;
      background-color: white;
      border: 3px solid #aaa;
      position: relative;
    }

    .top {
      width: 80px;
      height: 80px;
      margin-bottom: -10px;
    }

    .middle {
      width: 120px;
      height: 120px;
      margin-bottom: -20px;
    }

    /* Глаза */
    .eye {
      position: absolute;
      width: 10px;
      height: 10px;
      background-color: black;
      border-radius: 50%;
      top: 25px;
    }

    .eye.left {
      left: 20px;
    }

    .eye.right {
      right: 20px;
    }

    /* Морковный нос */
    .nose {
      position: absolute;
      top: 40px;
      left: 35px;
      width: 30px;
      height: 15px;
      background: orange;
      clip-path: polygon(0 0, 100% 50%, 0 100%);
      transform: rotate(10deg);
    }

    /* Пуговицы */
    .button {
      position: absolute;
      width: 12px;
      height: 12px;
      background-color: black;
      border-radius: 50%;
      left: 50%;
      transform: translateX(-50%);
    }

    .b1 {
      top: 30px;
    }

    .b2 {
      top: 60px;
    }

    .b3 {
      top: 90px;
    }
  </style>
</head>

<body>
  <!-- Снеговик -->
  <div class="snowman">

    <!-- Голова -->
    <div class="ball top">
      <div class="eye left"></div>
      <div class="eye right"></div>
      <div class="nose"></div>
    </div>

    <!-- Туловище -->
    <div class="ball middle">
      <div class="button b1"></div>
      <div class="button b2"></div>
      <div class="button b3"></div>
    </div>

  </div>
</body>

</html>
```

---

## Вторая часть задания - "CSS-бинго 6 × 6"

### Задание

Создайте страницу `bingo.html`, на которой будет сетка 6 × 6 с текстом и изображениями

Используйте:

* контейнер с `display: flex; flex-wrap: wrap;`
* или CSS Grid (`display: grid;`)

### Требования

1. Размер карточки - 600 × 600 px
2. Каждая ячейка должна быть одинакового размера
3. Внутри ячейки может быть **текст, картинка или и то, и другое**
4. Добавьте рамку и цвет фона каждой ячейки
5. При наведении (`:hover`) фон ячейки должен меняться

### Пример (структура)

```html
<!DOCTYPE html>
<html>

<head>
  <meta charset="UTF-8">
  <title>CSS Bingo</title>
  <style>
    .bingo {
      display: grid;
      grid-template-columns: repeat(6, 1fr);
      grid-template-rows: repeat(6, 1fr);
      width: 600px;
      height: 600px;
      border: 3px solid #444;
      margin: 50px auto;
    }

    .cell {
      border: 1px solid #999;
      display: flex;
      justify-content: center;
      align-items: center;
      font-family: sans-serif;
      background-color: #f0f0f0;
      transition: background 0.3s;
    }

    .cell:hover {
      background-color: #cfe8ff;
    }
  </style>
</head>

<body>
  <div class="bingo">
    <div class="cell">🎮</div>
    <div class="cell">🐱</div>
    <div class="cell">🍀</div>
    <!-- ... -->
  </div>
</body>

</html>
```

## Третья часть задания - "Оформляем лабораторную 2"

Теперь вернитесь к вашей **второй лабораторной (HTML)** и добавьте к ней **оформление через CSS**

Создайте файл `style.css` и подключите его ко всем страницам через:

```html
<link rel="stylesheet" href="style.css">
```

### Требования по оформлению

#### На странице `index.html`

* Добавить **фон страницы** (`background-color` или `background-image`)
* Задать **цвет текста** (`color`)
* Настроить **шрифт и размер** для заголовков `h1`, `h2`, `h3`
* Сделать **рамку для iframe** (через CSS, а не атрибут)

#### На страницах с неделями (`w1.html`, `w2.html`, `w3.html`)

* Использовать **один общий `style.css`**
* Оформить таблицы:
  * Границы (`border`, `border-collapse`)
  * Цвет заголовков (`background-color` для `<th>`)
  * Цвет фона строк через `nth-child`
  * Шрифт и выравнивание текста
  * Стили ссылок на недели

#### На странице `me.html`

* Добавить **фон с картинкой** (через `background-image`)
* Использовать **не менее 5 разных цветов** для разных участков текста (через классы)
* Оформить **изображение** (скругление, тень, рамка)
* Сделать **две колонки на компьютере** и **одну колонку на телефоне** с помощью Flex или Grid

---

## Четвертая часть задания (дополнение к третьей)

* Добавьте **плавные переходы (`transition`)** при наведении на разные элементы (например, на ячейки таблицы)
* Используйте **псевдоклассы** (`:hover`, `:first-child`, `:nth-child`)
* Сделайте стили разных страниц чуть отличающимися (например, фон недели 1 - синий, недели 2 - зеленый и т.д.)
