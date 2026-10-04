# Статический и динамический полиморфизм

- [Полиморфизм](#полиморфизм)
- [Статический полиморфизм](#статический-полиморфизм)
- [Динамический полиморфизм](#динамический-полиморфизм)
  - [Перегрузка и virtual вызов](#перегрузка-и-virtual-вызов)
- [Абстрактный класс и интерфейс](#абстрактный-класс-и-интерфейс)
- [Работа через базовую ссылку и указатель](#работа-через-базовую-ссылку-и-указатель)
- [Полиморфное владение и коллекции](#полиморфное-владение-и-коллекции)
- [Object slicing](#object-slicing)
- [Composition и inheritance](#composition-и-inheritance)
- [Интерфейс в header и реализация в source](#интерфейс-в-header-и-реализация-в-source)
- [RTTI: dynamic\_cast и typeid (EXT)](#rtti-dynamic_cast-и-typeid-ext)
- [Справочные материалы](#справочные-материалы)

## Полиморфизм

Полиморфизм позволяет применять общий интерфейс к значениям разных типов. Конкретная операция определяется правилами выбора реализации

| Вид          | Момент выбора | Основа выбора              | Средства C++        |
| ------------ | ------------- | -------------------------- | ------------------- |
| Статический  | компиляция    | статические типы выражений | перегрузки, шаблоны |
| Динамический | выполнение    | динамический тип объекта   | virtual функции     |

## Статический полиморфизм

Перегрузки из главы 04 позволяют использовать одно имя для разных типов. Рассмотрим готовые операции вывода:

```cpp
struct Point { int x, y; };
void show(int value) { std::cout << value << '\n'; }
void show(Point value) { std::cout << value.x << ' ' << value.y << '\n'; }

// Внутри функции:
show(7);
show(Point{2, 5});
```

Для примера нужен `<iostream>`. Компилятор выбирает перегрузку по типу аргумента. Значение аргумента может поступить из файла во время выполнения. Статический выбор относится к реализации операции, а само выполнение функции может происходить во время работы программы

Перегрузка операторов из главы 02 также участвует в статическом выборе операций. Шаблоны описывают семейства функций и типов, конкретные варианты формируются при компиляции. Их синтаксис и применение рассматриваются в главах 07 и 08

## Динамический полиморфизм

В главе 05 введены virtual функции. При вызове через базовую ссылку или указатель реализация определяется динамическим типом объекта. Один участок кода может обработать разнородную коллекцию

```cpp
struct Sound {
    virtual ~Sound() = default;
    virtual void play() const { std::cout << "sound\n"; }
};
struct Bell : public Sound {
    void play() const override { std::cout << "ding\n"; }
};
void playSound(const Sound& sound) { sound.play(); }
```

`playSound` принимает `const Sound&` и вызывает `Bell::play()` для объекта `Bell`. Для примера нужен `<iostream>`

### Перегрузка и virtual вызов

В одном выражении могут последовательно работать оба механизма. Добавим к предыдущему примеру перегрузки:

```cpp
void inspect(const Sound&) { std::cout << "Sound overload\n"; }
void inspect(const Bell&) { std::cout << "Bell overload\n"; }

// Внутри функции:
Bell bell;
const Sound& base = bell;
inspect(bell); // Bell overload
inspect(base); // Sound overload
base.play();   // ding
```

У `base` статический тип `const Sound`, поэтому перегрузка `inspect` выбирается для `Sound`. Virtual вызов `play()` обращается к реализации динамического типа `Bell`. Набор доступных операций определяется базовым интерфейсом

## Абстрактный класс и интерфейс

Pure virtual function (чистая виртуальная функция) объявляется с `= 0`. Класс остается абстрактным, пока хотя бы одна итоговая реализация его virtual функций является pure virtual. Создание отдельного объекта такого класса вызывает ошибку компиляции. Ссылки и указатели на абстрактный тип служат для работы с производными объектами

```cpp
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
};
```

Virtual вызов pure virtual функции для создаваемого или разрушаемого объекта из конструктора или деструктора приводит к undefined behavior. Вызовы обязательных операций интерфейса выполняются после полного создания конкретного объекта

Интерфейс описывает доступные операции и их контракт. В C++ интерфейс часто представляется абстрактным классом с public pure virtual функциями и virtual destructor. Отдельное ключевое слово для интерфейса в этой форме записи отсутствует

Для `Shape::area()` контракт задает площадь в квадратных единицах. Конкретные фигуры хранят собственные размеры:

```cpp
class Rectangle : public Shape {
    double width_;
    double height_;
public:
    Rectangle(double width, double height) : width_{width}, height_{height} {
        if (!(width > 0.0) || !(height > 0.0)) {
            throw std::invalid_argument{"Positive dimensions required"};
        }
    }
    double area() const override { return width_ * height_; }
};
```

Для проверки аргументов нужен `<stdexcept>`. В примерах размеры считаются конечными числами, произведение которых помещается в `double`

```cpp
class Circle : public Shape {
    double radius_;
public:
    explicit Circle(double radius) : radius_{radius} {
        if (!(radius > 0.0)) {
            throw std::invalid_argument{"Positive radius required"};
        }
    }
    double area() const override {
        return std::numbers::pi * radius_ * radius_;
    }
};
```

`std::numbers::pi` объявлена в `<numbers>` начиная с C++20. Для `Circle` также используется `<stdexcept>`. Радиус в примерах конечный, результат площади помещается в `double`

`Rectangle` и `Circle` являются concrete classes: они предоставляют реализации обязательных операций и позволяют создавать объекты. Абстрактный класс также может содержать поля, конструкторы и обычные функции с реализацией

## Работа через базовую ссылку и указатель

Функция принимает базовую ссылку и вызывает общий интерфейс:

```cpp
void printArea(const Shape& shape) {
    std::cout << shape.area() << '\n';
}

Rectangle rectangle{3.0, 4.0};
Circle circle{2.0};
printArea(rectangle); // 12
printArea(circle);    // примерно 12.5664
```

Ссылка обозначает исходную фигуру. Объект сохраняет размеры и динамический тип. `const Shape&` разрешает вызовы const операций интерфейса

Указатель позволяет дополнительно представить отсутствие объекта через `nullptr`:

```cpp
void printAreaIfPresent(const Shape* shape) {
    if (shape == nullptr) {
        return;
    }
    std::cout << shape->area() << '\n';
}
```

`Shape&` и `Shape*` в таких функциях предоставляют доступ к объекту. Ответственность за его lifetime остается у владельца. После разрушения фигуры сохраненная ссылка или указатель становится dangling. Разыменование dangling pointer приводит к undefined behavior

## Полиморфное владение и коллекции

`std::unique_ptr<Derived>` преобразуется в `std::unique_ptr<Base>` при передаче владения, если соответствующее public преобразование указателей доступно. В примере с одиночным public наследованием и стандартными deleters это позволяет хранить разные фигуры в одной коллекции:

```cpp
std::vector<std::unique_ptr<Shape>> shapes;
shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));
shapes.push_back(std::make_unique<Circle>(2.0));

for (const auto& shape : shapes) {
    std::cout << shape->area() << '\n';
}
```

Нужны `<memory>`, `<vector>` и `<iostream>`. Контейнер хранит владельцев, каждый владелец управляет отдельным объектом с собственным динамическим типом. В этом примере каждый элемент содержит фигуру

```cpp
std::unique_ptr<Shape> first = std::make_unique<Circle>(1.0);
shapes.push_back(std::move(first));
```

После перемещения `first` содержит `nullptr`. Фигура сохраняет свой тип и состояние, владение переходит к элементу `shapes`. При уничтожении контейнера уничтожаются его элементы и все принадлежащие им фигуры

Получение доступа отделяется от передачи владения:

```cpp
printArea(*shapes.front());
const Shape* selected = shapes.front().get();
printAreaIfPresent(selected);
```

`get()` возвращает наблюдающий указатель. Его срок использования ограничен lifetime фигуры. Удаление соответствующего владельца из контейнера уничтожает фигуру и делает `selected` dangling

## Object slicing

Object slicing возникает при копировании производного объекта в отдельный объект базового типа. Новый объект содержит только базовую часть

Для демонстрации используется конкретный базовый класс с реализацией операции:

```cpp
struct Label {
    virtual ~Label() = default;
    virtual std::string text() const { return "Label"; }
};
struct WarningLabel : public Label {
    std::string text() const override { return "Warning"; }
};
```

Для примера нужен `<string>`

```cpp
WarningLabel warning;
Label copy = warning;
const Label& reference = warning;

std::cout << copy.text() << '\n';      // Label
std::cout << reference.text() << '\n'; // Warning
```

Динамический тип `copy` - `Label`. Ссылка `reference` продолжает обозначать исходный `WarningLabel`

Передача параметра по значению также создает объект базового типа:

```cpp
void printLabel(Label label) {
    std::cout << label.text() << '\n';
}

printLabel(warning); // Label
```

Для сохранения полиморфного поведения параметр объявляется `const Label&`. Аналогичный выбор возникает у контейнеров: `std::vector<Label>` хранит значения `Label`, `std::vector<std::unique_ptr<Label>>` хранит владельцев полиморфных объектов

У абстрактного `Shape` попытка создать отдельное базовое значение завершается ошибкой компиляции. Для полиморфных операций используются базовые ссылки, указатели и owning pointers с корректным удалением

## Composition и inheritance

Composition связывает объект с составляющими через поля. Отношение **has-a** означает, что объект содержит другой объект или владеет им. Наследование **is-a** означает, что объект удовлетворяет контракту базового типа

Прямое поле подходит для части с заранее известным типом:

```cpp
struct Point {
    double x{};
    double y{};
};

struct PositionedRectangle {
    Point position;
    Rectangle rectangle;
};
```

`PositionedRectangle` содержит положение и прямоугольник. Их lifetime управляется самим объектом

Composition также может включать полиморфное владение. Например, элемент рисунка содержит фигуру и ее положение:

```cpp
class DrawingItem {
    Point position_;
    std::unique_ptr<Shape> shape_;
public:
    DrawingItem(Point position, std::unique_ptr<Shape> shape)
        : position_{position}, shape_{std::move(shape)} {
        if (shape_ == nullptr) {
            throw std::invalid_argument{"Shape required"};
        }
    }
    Point position() const { return position_; }
    double area() const { return shape_->area(); }
};
```

Здесь нужны `<memory>`, `<utility>` и `<stdexcept>`. Конструктор принимает владение фигурой. `area()` делегирует расчет объекту через общий интерфейс

```cpp
std::vector<DrawingItem> drawing;
drawing.emplace_back(Point{1.0, 2.0}, std::make_unique<Circle>(2.0));
drawing.emplace_back(Point{5.0, 0.0}, std::make_unique<Rectangle>(3.0, 4.0));

for (const auto& item : drawing) {
    std::cout << item.position().x << ' ' << item.area() << '\n';
}
```

`DrawingItem` использует Rule of Zero. Поле `unique_ptr` определяет семантику владения: объект поддерживает перемещение, попытка копирования вызывает ошибку компиляции. После перемещения исходный `DrawingItem` содержит пустой `shape_`. Вызов `area()` требует присутствия фигуры. Для перемещенного объекта доступны разрушение и присваивание нового значения перемещением

Выбор связи определяется моделью: `Circle` является `Shape`, `DrawingItem` содержит `Shape`. Public inheritance подходит для взаимозаменяемых реализаций общего контракта. Composition подходит для объединения самостоятельных частей и делегирования операций

## Интерфейс в header и реализация в source

Объявления классов размещаются в headers. Тела функций можно вынести в `.cpp`. Ниже - самостоятельный вариант `Shape` и `Rectangle` для программы из нескольких translation units

`shape.hpp`:

```cpp
#pragma once

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

class Rectangle : public Shape {
    double width_;
    double height_;
public:
    Rectangle(double width, double height);
    double area() const override;
};
```

`shape.cpp`:

```cpp
#include "shape.hpp"
#include <stdexcept>

Rectangle::Rectangle(double width, double height)
    : width_{width}, height_{height} {
    if (!(width > 0.0) || !(height > 0.0)) {
        throw std::invalid_argument{"Positive dimensions required"};
    }
}

double Rectangle::area() const {
    return width_ * height_;
}
```

`override` записывается в объявлении внутри класса. Вынесенное definition использует имя `Rectangle::area` и сохраняет завершающий `const`

`main.cpp`:

```cpp
#include "shape.hpp"
#include <iostream>
#include <memory>

int main() {
    std::unique_ptr<Shape> shape = std::make_unique<Rectangle>(3.0, 4.0);
    std::cout << shape->area() << '\n';
}
```

Сборка из каталога с этими файлами:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -c main.cpp -o main.o
g++ -std=c++20 -Wall -Wextra -Wpedantic -c shape.cpp -o shape.o
g++ main.o shape.o -o app
./app
```

Программа выводит `12`. Для объявленной обычной virtual функции требуется definition. Если тело объявленного деструктора или используемой функции осталось за пределами сборки, linker может сообщить `undefined reference` или `undefined symbols`. Сообщение о `vtable` также служит поводом проверить definitions virtual функций и состав object files

## RTTI: dynamic_cast и typeid (EXT)

RTTI предоставляет информацию о типе объекта во время выполнения. Для полиморфного базового типа `dynamic_cast` выполняет проверяемое преобразование к производному типу:

```cpp
const Shape* shape = &circle;
if (const auto* found = dynamic_cast<const Circle*>(shape)) {
    std::cout << found->area() << '\n';
}
```

Успешное преобразование возвращает указатель на соответствующий объект. При несовпадении типа результат преобразования указателя равен `nullptr`. Для преобразования ссылки `dynamic_cast<const Circle&>(value)` при несовпадении типа выбрасывается `std::bad_cast` из `<typeinfo>`

`typeid` позволяет сравнить динамический тип полиморфного объекта с конкретным типом:

```cpp
const Shape& shapeReference = circle;
std::cout << (typeid(shapeReference) == typeid(Circle)) << '\n'; // 1
```

Для примера нужен `<typeinfo>`. Результат `typeid(...).name()` зависит от реализации и подходит для вспомогательной диагностики. Общие операции иерархии выражаются virtual функциями, проверяемое преобразование подходит для операций, которым требуется конкретный производный тип

## Справочные материалы

- Наследование: [C++ draft - Derived classes](https://eel.is/c++draft/class.derived)
- Virtual функции и `override`: [C++ draft - Virtual functions](https://eel.is/c++draft/class.virtual)
- Абстрактные классы: [C++ draft - Abstract classes](https://eel.is/c++draft/class.abstract)
- Порядок инициализации: [C++ draft - Initializing bases and members](https://eel.is/c++draft/class.base.init)
- Поведение при создании и разрушении: [C++ draft - Construction and destruction](https://eel.is/c++draft/class.cdtor)
- Полиморфное удаление: [C++ Core Guidelines - C.35](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c35-a-base-class-destructor-should-be-either-public-and-virtual-or-protected-and-non-virtual)
- Slicing: [C++ Core Guidelines - C.67](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c67-a-polymorphic-class-should-suppress-public-copymove)
- `std::unique_ptr`: [C++ draft - unique_ptr](https://eel.is/c++draft/unique.ptr)
- RTTI: [C++ draft - dynamic_cast](https://eel.is/c++draft/expr.dynamic.cast), [C++ draft - typeid](https://eel.is/c++draft/expr.typeid)
