# Шаблоны классов и concepts

- [Шаблон класса и конкретный тип](#шаблон-класса-и-конкретный-тип)
- [Методы вне определения класса](#методы-вне-определения-класса)
- [Параметр-значение](#параметр-значение)
- [Concepts и ограничения](#concepts-и-ограничения)
- [Requires-expression](#requires-expression)
- [Ограничения отдельных методов](#ограничения-отдельных-методов)
- [Контракт контейнера и пустое состояние](#контракт-контейнера-и-пустое-состояние)
- [Особый случай vector<bool>](#особый-случай-vectorbool)
- [constexpr и consteval](#constexpr-и-consteval)
- [if constexpr](#if-constexpr)
- [Специализация и стоимость шаблонов](#специализация-и-стоимость-шаблонов)
- [Справочные материалы](#справочные-материалы)

## Шаблон класса и конкретный тип

Шаблон класса задает семейство типов с общей структурой. Аргумент шаблона определяет тип полей и операций

```cpp
template<class T>
class Box {
  T value_;
public:
  explicit Box(T value) : value_{value} {}
  const T &get() const { return value_; }
  void set(const T &value) { value_ = value; }
};
```

```cpp
Box<int> count{12};
Box<double> length{2.5};
count.set(15);
std::cout << length.get() << '\n';
```

`Box<int>` и `Box<double>` - разные типы. Каждый объект хранит собственное значение. Само имя `Box` обозначает шаблон. Внутри определения класса `Box` также обозначает текущую специализацию

Запись `Box count{12}` использует class template argument deduction: в данном примере тип выводится из конструктора. Явная запись `Box<int>` помогает видеть выбранный тип в учебном коде

## Методы вне определения класса

Для определения метода снаружи повторяется список параметров шаблона, а имя класса записывается с аргументами

```cpp
template<class T>
class Value {
  T data{};
public:
  T get() const;
};

template<class T>
T Value<T>::get() const { return data; }
```

Обе части размещаются в header с `#pragma once`. Правила видимости определения совпадают с шаблонами функций из главы 07

Обычный метод шаблонного класса использует параметры класса. Метод также может иметь собственный параметр шаблона

```cpp
template<class T>
struct Reading {
  T value;
  template<class U>
  U as() const { return static_cast<U>(value); }
};

// Внутри функции:
Reading<double> reading{3.8};
int whole = reading.as<int>(); // 3
```

## Параметр-значение

Аргументом шаблона может быть значение, известное при компиляции. Например, размер фиксированного буфера

```cpp
template<class T, std::size_t N>
struct Buffer {
  std::array<T, N> values{};
  constexpr std::size_t size() const { return N; }
};

// Внутри функции:
Buffer<int, 4> samples;
static_assert(sizeof(samples.values) >= 4 * sizeof(int));
```

Нужны `<array>` и `<cstddef>`. `std::array<T, N>` хранит элементы внутри себя. `Buffer<int, 4>` и `Buffer<int, 8>` являются разными типами. Для размера, который приходит во время выполнения, подходит `std::vector<T>`

`static_assert` проверяет логическое константное выражение при компиляции. Ложное условие останавливает сборку с диагностикой

## Concepts и ограничения

Concept - именованное условие применимости шаблона. Concepts доступны начиная с C++20. Условие проверяется при компиляции для конкретных аргументов

```cpp
template<class T>
concept Numeric = std::is_arithmetic_v<T> && !std::same_as<T, bool>;

template<Numeric T>
T twice(T value) { return value + value; }
```

Нужны `<type_traits>` и `<concepts>`. Type trait `std::is_arithmetic_v<T>` дает константу `true` для встроенных целочисленных и вещественных типов, включая `bool`. Вторая часть условия исключает `bool`. Пользовательские числовые классы требуют отдельного расширения контракта

`std::same_as<T, U>` проверяет совпадение типов. `std::integral<T>` ограничивает типы целочисленными, включая `bool`. `std::floating_point<T>` выбирает вещественные типы

Ограничение также записывается отдельным requires-clause:

```cpp
template<class T>
requires Numeric<T>
T triple(T value) { return value + value + value; }
```

Для этих примеров значения выбираются в диапазоне, где арифметика представима. Constraints проверяют типы и выражения. Значения, поступившие от пользователя, требуют проверок во время выполнения

## Requires-expression

Requires-expression описывает допустимые операции над типом. Параметры внутри `requires(...)` служат для проверки выражений

```cpp
template<class T>
concept Printable = requires(std::ostream &out, const T &value) {
  { out << value } -> std::same_as<std::ostream &>;
};
```

Нужны `<ostream>` и `<concepts>`. Фигурные скобки задают compound requirement, часть после `->` ограничивает тип результата. Проверяемое выражение выполняется только при обычном вызове соответствующей операции программы

```cpp
template<Printable T>
void write(std::ostream &out, const T &value) {
  out << value;
}
```

Requires-clause ограничивает объявление. Requires-expression дает логическое значение по допустимости выражений. Именованный concept связывает эти две формы понятным именем

Стандартный `std::equality_comparable<T>` описывает сравнение равенства, `std::totally_ordered<T>` добавляет полный порядок. Они включают требования к выражениям и их смыслу. Семантические свойства сравнения обеспечивает автор типа, компилятор проверяет доступную ему часть контракта

## Ограничения отдельных методов

Один контейнер может хранить разные типы, а отдельные операции требуют дополнительных свойств

```cpp
template<class T>
struct Pair {
  T first, second;
  bool equal() const requires std::equality_comparable<T> {
    return first == second;
  }
  long double mean() const requires Numeric<T> {
    return static_cast<long double>(first) / 2
         + static_cast<long double>(second) / 2;
  }
};
```

`Pair<bool>` поддерживает `equal()`, вызов `mean()` отклоняется по ограничению `Numeric`. У `Pair<int>` доступны обе операции. Вещественное преобразование до арифметики сохраняет дробную часть среднего и избегает сложения двух `int` с возможным переполнением

При инстанцировании класса тела обычных методов обычно инстанцируются по мере необходимости. Ограничение метода явно задает его доступность и позволяет получить диагностику на уровне интерфейса

## Контракт контейнера и пустое состояние

Для универсального контейнера заранее задаются допустимые размеры, способ доступа и результат операций на пустой последовательности

| Операция | Один из допустимых контрактов |
| --- | --- |
| Подсчет | `0` для пустой последовательности |
| Поиск | `std::nullopt` при отсутствии результата |
| Минимум и максимум | Исключение `std::domain_error` для пустой последовательности |
| Среднее | Вещественный результат, исключение для пустой последовательности |
| Доступ по индексу | Проверка границы, `std::out_of_range` при ошибке |

`std::optional` введен в главе 07. Для исключений нужен `<stdexcept>`. Базовые `throw`, `try` и `catch` уже встречались при проверке инвариантов, систематический разбор следует в главе 09

Для арифметики с вещественными типами контракт может требовать конечные значения. `NaN` нарушает ожидаемые свойства порядка, бесконечность требует отдельной модели результата. Проверку конечности предоставляет `std::isfinite` из `<cmath>`

## Особый случай vector<bool>

`std::vector<bool>` использует специальную реализацию для битов. Доступ к изменяемому элементу возвращает proxy-объект, который умеет читать и менять бит. Возврат `bool&` для такого элемента дает ошибку компиляции

Для единого интерфейса с небольшими копируемыми значениями можно разделить чтение и запись

```cpp
template<class T>
class Sequence {
  std::vector<T> values;
public:
  explicit Sequence(std::size_t size) : values(size) {}
  T get(std::size_t index) const { return values.at(index); }
  void set(std::size_t index, const T &value) { values.at(index) = value; }
};
```

Нужны `<vector>` и `<cstddef>`. `get()` возвращает значение `T`, `set()` присваивает через интерфейс контейнера. `at()` проверяет границы. Такой подход подходит и для `Sequence<bool>`

## constexpr и consteval

`constexpr` разрешает вычисление функции в константном выражении при подходящих аргументах. `constexpr` у переменной требует константной инициализации

```cpp
template<class T>
constexpr T square(T value) { return value * value; }

constexpr int count = square(4);
static_assert(count == 16);
```

Та же функция может работать с данными времени выполнения. Само наличие `constexpr` у функции сохраняет обе возможности

```cpp
int value;
std::cin >> value;
std::cout << square(value) << '\n';
```

Для фрагмента нужен `<iostream>`, входное значение должно давать представимый квадрат. `consteval` требует константного вычисления обычного вызова такой функции

```cpp
consteval unsigned bit(unsigned index) {
  if (index >= std::numeric_limits<unsigned>::digits) throw "Bit index out of range";
  return 1u << index;
}
constexpr unsigned mask = bit(5);
static_assert(mask == 32);
```

Нужен `<limits>`. Ширину типа дает `std::numeric_limits<unsigned>::digits`. Путь с `throw` исключает константное вычисление и приводит к диагностике

## if constexpr

Внутри шаблона `if constexpr` выбирает ветвь по константному условию. После подстановки типов отброшенная ветвь с зависимым кодом остается без инстанцирования

```cpp
template<class T>
void debug(const T &value) {
  if constexpr (std::is_pointer_v<T>) {
    if (value) std::cout << *value << '\n';
  } else {
    std::cout << value << '\n';
  }
}
```

Нужны `<iostream>` и `<type_traits>`. `debug(7)` использует обычный вывод. Для указателя на доступный для чтения `int` выбирается разыменование. Для `void*` выбранная ветвь дает ошибку: ее контракт требует подходящий указуемый тип

Обе ветви должны иметь корректный синтаксис. Обычный `if` проверяет допустимость обеих ветвей при инстанцировании. `if constexpr` позволяет описать разные допустимые операции для разных типов

## Специализация и стоимость шаблонов

Полная специализация задает отдельную реализацию для конкретных аргументов. Ее объявляют после основного шаблона и до первого использования соответствующей специализации

```cpp
template<class T>
struct Label { static constexpr const char *name = "value"; };

template<>
struct Label<bool> { static constexpr const char *name = "flag"; };
```

Для отдельного поведения функций часто достаточно обычной перегрузки. Частичная специализация классов позволяет выбирать семейства аргументов, например указатели. Это расширение рассматривается по необходимости

Шаблоны дают проверку интерфейсов и выбор реализации при компиляции. Известный тип помогает оптимизатору встроить вызов, фиксированный размер позволяет хранить данные внутри объекта. Увеличение числа специализаций может увеличить время сборки и размер программы

Константное вычисление гарантируется контекстом вроде `constexpr`-инициализации или `static_assert`. Само использование шаблона оставляет обработку входных данных во времени выполнения. Ускорение конкретной программы устанавливается измерением

## Справочные материалы

- [C++ draft - Class templates](https://eel.is/c++draft/temp.class)
- [C++ draft - Constraints](https://eel.is/c++draft/temp.constr.decl)
- [C++ draft - Requires expressions](https://eel.is/c++draft/expr.prim.req)
- [C++ draft - Standard concepts](https://eel.is/c++draft/concepts)
- [C++ draft - vector<bool>](https://eel.is/c++draft/vector.bool)
- [C++ draft - constexpr и consteval](https://eel.is/c++draft/dcl.constexpr)
- [C++ draft - if constexpr](https://eel.is/c++draft/stmt.if)
