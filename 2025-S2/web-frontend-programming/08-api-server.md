# API сервер на Node.js (Express.js)

## Что такое API

**API (Application Programming Interface)** - это способ, с помощью которого одна программа может взаимодействовать с другой программой

В контексте веб-разработки API обычно означает:

* Сервер, который принимает HTTP-запросы
* Сервер, который возвращает данные (чаще всего в формате JSON)
* Клиент (браузер, сайт, мобильное приложение) не получает HTML, а только данные

Пример:

* Браузер отправляет запрос:
  `/api/calc?a=2&b=3`
* Сервер отвечает:
  `{ "result": 10 }`

API часто используется для:

* Обмена данными между фронтендом и бекендом
* Мобильных приложений
* Интеграций между сервисами
* Микросервисной архитектуры

## На чем можно писать API

API можно реализовать на самых разных технологиях:

* Node.js (Express, Fastify, NestJS)
* Python (Flask, FastAPI, Django)
* PHP (Laravel, Slim)
* Java (Spring)
* C# (.NET Web API)
* Go, Rust (Rocket, Axum), C++ (asio, Drogon) и др.

В рамках этого курса используется:

* **Node.js** - платформа для запуска JavaScript на сервере
* **Express.js** - простой и популярный фреймворк для создания API (как правило является компонентов для сложных фреймворков, таких, как NestJS)

## Создание проекта с Express.js

Express.js - это минималистичный фреймворк для Node.js, который упрощает:

* Обработку HTTP-запросов
* Маршрутизацию (GET, POST и др.)
* Работу с JSON
* Создание REST API

Express не навязывает структуру/архитектуру проекта

### Инициализация проекта

Создаем папку проекта:

```bash
mkdir api-server
cd api-server
```

Инициализируем npm-проект:

```bash
npm init -y
```

Устанавливаем Express:

```bash
npm install express
```

В папке появится файл `package.json` и папка `node_modules`

### Базовый сервер

Создаем файл `index.js`:

```js
const express = require("express");

const app = express();
const PORT = 3001;

app.listen(PORT, () => {
  console.log("Server started on port", PORT);
});
```

Запуск сервера:

```bash
node index.js
```

Если ошибок нет - сервер успешно запущен. Пока сервер ничего не умеет делать (никак не отвечает на запросы к нему)

## HTTP и методы запросов

HTTP - это протокол, по которому клиент и сервер обмениваются данными

Каждый запрос состоит из:

* Метода (GET, POST, PUT, DELETE и др.)
* Адреса (URL)
* Параметров
* Тела запроса (не обязательно)

### HTTP-методы

* **GET** - получить данные
* **POST** - отправить данные
* **PUT** - обновить данные

Полный список методов [доступен на MDN](https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference/Methods)

Примеры:

* GET `/users` - получить список пользователей
* POST `/users` - создать пользователя
* PUT `/users/5` - обновить пользователя

### HTTP-коды ответа

Сервер всегда возвращает код состояния

Основные коды:

* `200` - OK, запрос выполнен успешно
* `400` - Bad Request, ошибка в запросе
* `404` - Not Found, ресурс не найден
* `500` - Internal Server Error, ошибка сервера

Полный список кодов [доступен на MDN](https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference/Status)

В Express код можно задать явно:

```js
res.status(200).json({ ok: true });
```

## Формат JSON

**JSON (JavaScript Object Notation)** - текстовый формат для передачи данных

Пример JSON:

```json
{
  "result": 10,
  "success": true
}
```

Особенности JSON:

* Данные передаются в виде объекта
* Ключи и строки всегда в двойных кавычках
* Легко читается человеком
* Стандарт для API

Express умеет автоматически отправлять JSON:

```js
res.json({ result: 10 });
```

## Простейший API на Express.js

Сделаем API, который:

* принимает два числа `a` и `b`
* складывает их
* умножает сумму на 2
* возвращает результат в JSON

Файл `index.js`:

```js
const express = require("express");

const app = express();
const PORT = 3001;

app.get("/api/calc", (req, res) => {
  const a = Number(req.query.a);
  const b = Number(req.query.b);

  if (isNaN(a) || isNaN(b)) {
    return res.status(400).json({
      error: "Parameters a and b must be numbers"
    });
  }

  const result = (a + b) * 2;

  res.json({
    a,
    b,
    result
  });
});

app.listen(PORT, () => {
  console.log("Server started on port", PORT);
});
```

### Что здесь происходит

* `app.get` - обработчик GET-запроса
* `/api/calc` - адрес API
* `req.query` - параметры из URL
* `Number(...)` - преобразование строки в число
* `res.json(...)` - ответ в формате JSON

### Тестирование API

Можно просто открыть в браузере:

```
http://localhost:3001/api/calc?a=2&b=3
```

Ответ:

```json
{
  "a": 2,
  "b": 3,
  "result": 10
}
```

Сложные API удобнее всего тестировать с помощью специальных инструментов. Наиболее популярные и бесплатные:

* Postman
* RapidAPI Client
* Insomnia

---

## Пример клиента на Next.js

Сделаем простой клиент, который:

* Отправляет запрос к нашему API
* Получает результат
* Отображает его на странице

Убедитесь, что:

* API сервер запущен локально по адресу
  `http://localhost:3001`
* Есть маршрут API:
  `/api/calc?a=2&b=3`

### Пустая страница Next.js

Открываем файл страницы, например:

```txt
app/page.jsx
```

И делаем из нее клиентский компонент:

```jsx
"use client";
```

### Получение данных из API

Для запроса к API в браузере используется функция `fetch`

Простейший пример запроса:

```js
fetch("http://localhost:3001/api/calc?a=2&b=3")
```

`fetch` возвращает Promise, поэтому используется `then` или `async/await`

### Пример страницы с запросом к API

Полный пример страницы `app/page.tsx`:

```jsx
"use client";
import { useState } from "react";

export default function Page() {
  const [result, setResult] = useState(null);
  const [loading, setLoading] = useState(false);

  const loadData = async () => {
    setLoading(true);

    const response = await fetch(
      "http://localhost:3001/api/calc?a=2&b=3"
    );

    const data = await response.json();

    setResult(data.result);
    setLoading(false);
  };

  return (
    <main className="p-8">
      <h1 className="text-xl mb-4">Пример клиента для API</h1>

      <button
        onClick={loadData}
        className="px-4 py-2 bg-blue-500 text-white rounded"
      >
        Получить результат
      </button>

      {loading && <p className="mt-4">Загрузка...</p>}

      {result !== null && (
        <p className="mt-4">
          Результат с сервера: <b>{result}</b>
        </p>
      )}
    </main>
  );
}
```

### Что здесь происходит

* `"use client"` - делаем компонент клиентским
* `useState` - хранит результат и состояние загрузки
* `fetch` - отправляет HTTP-запрос к API
* `response.json()` - преобразует ответ сервера из JSON
* Кнопка запускает запрос вручную
* Результат отображается на странице

### Проверка работы

1. Запустить API сервер:

   ```bash
   node index.js
   ```
2. Запустить Next.js проект:

   ```bash
   npm run dev
   ```
3. Открыть страницу в браузере
4. Нажать кнопку

Если все сделано правильно:

* Запрос уйдет на API сервер
* Сервер выполнит вычисление
* Клиент получит JSON
* Результат появится на странице
