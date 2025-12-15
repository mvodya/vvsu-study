# Работа с React (Next.js)

## Создание проекта на Next.js

### Требования к окружению

Для работы с Next.js требуется:

* **Node.js** (установлен на предыдущей лабораторной)
* **npm** (устанавливается вместе с Node.js)
* **Visual Studio Code** (основная среда разработки, можно другую)

### Открытие терминала

Работа с Next.js всегда начинается из терминала

Варианты:

* Встроенный терминал VS Code
  *View -> Terminal*
  или
  ``Ctrl + ` ``
* Обычный терминал ОС

  * Windows - PowerShell / CMD (Shift + ПКМ в папке / Открыть в терминале)
  * macOS - Terminal
  * Linux - любой терминал

Все команды ниже выполняются в папке, где будет лежать проект

### Создание проекта

Команда для создания проекта:

```bash
npx create-next-app@latest my-app -e with-tailwindcss
```

Что происходит:

* `npx` - временно запускает пакет без установки
* `create-next-app` - официальный генератор Next.js
* `my-app` - имя папки проекта
* `with-tailwindcss` - готовая настройка Tailwind

После выполнения команды:

```txt
my-app/
├─ app/
├─ public/
├─ package.json
└─ ...
```

### Запуск проекта

Переход в папку проекта:

```bash
cd my-app
```

Запуск сервера разработки:

```bash
npm run dev
```

По умолчанию сайт доступен по адресу:

```
http://localhost:3000
```

## Что такое React в контексте Next.js

### Компонентный подход

React-компонент - это обычная JavaScript-функция, которая:

* принимает данные (props)
* возвращает JSX (разметку)

```ts
function Hello() {
  return <p>Привет</p>;
}
```

В Next.js страница - это тоже компонент

Файл:

```txt
app/page.tsx
```

Экспортирует компонент страницы:

```ts
export default function HomePage() {
  return <h1>Главная страница</h1>;
}
```

### JSX

JSX - это синтаксис, который выглядит как HTML, но является JavaScript-кодом

Пример:

```tsx
const title = "React";

<h1>{title}</h1>
```

Особенности JSX:

* используется `{}` для JS-выражений
* атрибут `class` заменен на `className`
* можно возвращать только один корневой элемент

## Компоненты в одном файле

В одном файле можно описывать несколько компонентов

```tsx
export default function HomePage() {
  return (
    <main>
      <HelloBlock />
      <InfoBox />
    </main>
  );
}

function HelloBlock() {
  return <p>Первый компонент</p>;
}

function InfoBox() {
  return <div>Еще один компонент</div>;
}
```

Используется для:

* простых компонентов
* локальной логики

## Вынесение компонентов в отдельные файлы

Для переиспользуемых элементов создается папка:

```txt
app/components/
```

Пример компонента:

```tsx
export default function MyButton() {
  return <button>Кнопка</button>;
}
```

Импорт:

```ts
import MyButton from "@/components/MyButton";
```

Использование:

```tsx
<MyButton />
```

## Клиентские компоненты и интерактивность

### Почему нужен `"use client"`

По умолчанию компоненты в Next.js - **серверные** (без доступа к событиям, состоянию, браузеру)

Для интерактивности нужен клиентский компонент:

```ts
"use client";
```

Добавляется **в начале файла**

### useState

`useState` - хук для хранения состояния.

```ts
const [count, setCount] = useState(0);
```

* `count` - текущее значение
* `setCount` - функция изменения

Пример кнопки со счетчиком:

```tsx
"use client";
import { useState } from "react";

export default function CounterButton() {
  const [count, setCount] = useState(0);

  return (
    <button onClick={() => setCount(count + 1)}>
      Кликнули: {count}
    </button>
  );
}
```

## Передача данных между компонентами (props)

### Передача значений

Компонент может принимать данные через параметры:

```tsx
function Display({ value }: { value: number }) {
  return <div>{value}</div>;
}
```

Использование:

```tsx
<Display value={count} />
```

### Передача функций

Функции передаются так же, как значения:

```tsx
function Button({ onClick }: { onClick: () => void }) {
  return <button onClick={onClick}>+</button>;
}
```

## Поднятие состояния

Состояние хранится выше, если:

* несколько компонентов используют одно значение
* один компонент меняет данные другого

```tsx
"use client";
import { useState } from "react";

export default function Page() {
  const [value, setValue] = useState(0);

  return (
    <>
      <Display value={value} />
      <Button onClick={() => setValue(value + 1)} />
    </>
  );
}
```

Принцип:

* состояние - в родителе
* данные -> вниз
* действия -> вверх (через функции)

## Отдельные страницы в Next.js

Каждая папка в `app/` - это маршрут.

```txt
app/
├─ page.tsx          -> /
├─ calculator/
│  └─ page.tsx       -> /calculator
```

Страница - обычный React-компонент:

```tsx
export default function CalculatorPage() {
  return <h1>Калькулятор</h1>;
}
```

## Работа с вводом данных

### Управляемый input

Значение input контролируется состоянием:

```tsx
<input value={value} onChange={handle} />
```

Обработчик:

```ts
const handle = (e: React.ChangeEvent<HTMLInputElement>) => {
  setValue(Number(e.target.value));
};
```

### Ограничение ввода (JS-логика)

При работе с полями ввода React не добавляет собственной логики обработки текста. Все ограничения и проверки делаются обычным JavaScript, точно так же, как и без React

React лишь дает событие `onChange`, а дальше мы работаем с тем, что ввел пользователь

#### Откуда берется вводимое значение

В обработчике события мы получаем объект события `e`

```ts
const handle = (e: React.ChangeEvent<HTMLInputElement>) => {
  console.log(e.target.value);
};
```

* `e.target` - сам `<input>`
* `e.target.value` - текущий текст в поле ввода (ВСЕГДА строка)

Даже если вводятся цифры - это строка, а не число

##### Пример: Разрешаем ввод только цифр (без regex)

Самый простой и понятный вариант - проверить каждый символ

```ts
const handle = (e: React.ChangeEvent<HTMLInputElement>) => {
  const value = e.target.value;

  if (value === "" || !isNaN(Number(value))) {
    setValue(Number(value));
  }
};
```

Что здесь происходит:

* `value === ""` - разрешаем очистку поля
* `Number(value)` - пробуем превратить строку в число
* `isNaN(...)` - проверяем, получилось ли число

Если пользователь вводит буквы - значение просто игнорируется

##### Пример: Ввод только одной цифры (0–9)

```ts
const handle = (e: React.ChangeEvent<HTMLInputElement>) => {
  const value = e.target.value;

  if (value.length <= 1 && value >= "0" && value <= "9") {
    setValue(Number(value));
  }
};
```

Здесь:

* `value.length <= 1` - не больше одного символа
* сравнение строк `"0"` → `"9"` работает для цифр корректно

##### Пример: Автоматическое обрезание лишнего

Если пользователь вставил текст целиком - можно взять только первый символ:

```ts
const handle = (e: React.ChangeEvent<HTMLInputElement>) => {
  const value = e.target.value;
  const firstChar = value[0];

  if (firstChar >= "0" && firstChar <= "9") {
    setValue(Number(firstChar));
  } else {
    setValue(0);
  }
};
```

Такой подход:

* не блокирует ввод
* аккуратно приводит значение к нужному формату

##### Пример: Ограничение через HTML-атрибуты

Часть ограничений можно сделать без JavaScript, прямо в input:

```tsx
<input
  type="number"
  min="0"
  max="9"
  step="1"
/>
```

Но важно понимать:

* браузер **не запрещает** ввод текста полностью
* проверка все равно нужна в JavaScript
* HTML-атрибуты - это подсказка, не защита
