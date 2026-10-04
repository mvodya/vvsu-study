# Наследование и виртуальные функции

- [Базовый и производный классы](#базовый-и-производный-классы)
- [Доступ к членам](#доступ-к-членам)
- [Режимы наследования](#режимы-наследования)
- [Public inheritance и базовый тип](#public-inheritance-и-базовый-тип)
- [Конструктор базового класса](#конструктор-базового-класса)
  - [Порядок создания и разрушения](#порядок-создания-и-разрушения)
- [Обычные методы и скрытие имен](#обычные-методы-и-скрытие-имен)
- [Виртуальные функции](#виртуальные-функции)
  - [override и final](#override-и-final)
- [Виртуальный деструктор](#виртуальный-деструктор)
- [Virtual вызовы при создании и разрушении](#virtual-вызовы-при-создании-и-разрушении)
- [Справочные материалы](#справочные-материалы)

## Базовый и производный классы

Наследование создает класс на основе существующего класса. Базовый класс задает общие данные и операции. Производный класс содержит базовую часть, добавляет собственное состояние и операции

```cpp
class Device {
    int id;
public:
    explicit Device(int id) : id{id} {}
    int number() const { return id; }
};
class Screen : public Device {
    int brightness = 50;
public:
    explicit Screen(int id) : Device{id} {}
    int level() const { return brightness; }
};
```

В записи `class Screen : public Device` имя после `:` обозначает базовый класс. `public` задает режим наследования. Объект `Screen` содержит подобъект `Device` с полем `id` и свое поле `brightness`

```cpp
Screen screen{7};
std::cout << screen.number() << ' ' << screen.level() << '\n'; // 7 50
```

Вызов `number()` использует унаследованную операцию базового класса. Для вывода нужен `<iostream>`. Каждый объект `Screen` имеет собственную базовую часть

## Доступ к членам

Модификаторы доступа определяют, какой код может обращаться к члену класса:

| Доступ      | Сам класс и его друзья | Производный класс                      | Внешний код |
| ----------- | ---------------------- | -------------------------------------- | ----------- |
| `public`    | доступен               | доступен                               | доступен    |
| `protected` | доступен               | доступен с правилами protected-доступа | закрыт      |
| `private`   | доступен               | закрыт                                 | закрыт      |

Поле `Device::id` существует внутри объекта `Screen`. Прямой доступ к нему имеют методы и друзья `Device`. Производный класс получает значение через `number()`

`protected` подходит для операции, предназначенной для производных классов:

```cpp
class Counter {
    int value = 0;
protected:
    void increment() { value++; }
public:
    int current() const { return value; }
};
class ClickCounter : public Counter {
public:
    void click() { increment(); }
};
```

`ClickCounter::click()` вызывает защищенную операцию для своего объекта. Внешний код вызывает `click()` и `current()`. Попытка вызвать `counter.increment()` из `main` дает ошибку компиляции

Для доступа из метода производного класса к protected члену через объект его статический тип должен быть этим производным классом или дальнейшим наследником. Собственный объект удовлетворяет этому правилу. Данные по умолчанию сохраняются в `private`, изменение выполняют операции, поддерживающие инвариант

## Режимы наследования

Модификатор после `:` управляет доступностью унаследованного интерфейса:

| Наследование | `public` члены базы в производном классе | `protected` члены базы | Преобразование к базе из внешнего кода |
| ------------ | ---------------------------------------- | ---------------------- | -------------------------------------- |
| `public`     | `public`                                 | `protected`            | доступно                               |
| `protected`  | `protected`                              | `protected`            | закрыто                                |
| `private`    | `private`                                | `private`              | закрыто                                |

Во всех случаях прямой доступ к `private` членам базы остается у самой базы и ее друзей. Режим наследования отличается от доступа к собственным полям класса

```cpp
class PublicScreen : public Device {
public:
    PublicScreen() : Device{1} {}
};
class PrivateScreen : private Device {
public:
    PrivateScreen() : Device{2} {}
    int deviceNumber() const { return number(); }
};
```

У `PublicScreen` внешний код вызывает `number()`. У `PrivateScreen` внешний код использует собственную операцию `deviceNumber()`, доступ к `number()` закрыт

У `class` режим наследования по умолчанию `private`, у `struct` - `public`. Для открытых иерархий курса `public` записывается явно. Private inheritance подходит для отдельных случаев использования реализации базы. Для обычного включения готового объекта в другой объект применяется композиция

## Public inheritance и базовый тип

Public inheritance выражает отношение **is-a**: производный объект выполняет контракт базового типа. Например, `Screen` предоставляет операции `Device` с тем же смыслом

```cpp
Screen screen{7};
Device& device = screen;
Device* pointer = &screen;
std::cout << device.number() << ' ' << pointer->number() << '\n';
```

Преобразование к базовой ссылке или указателю называется upcast. Здесь оно обращается к базовому подобъекту исходного `Screen`. Оба имени обозначают части одного объекта. Срок использования ссылки и указателя ограничен lifetime объекта

```cpp
void printDevice(const Device& device) {
    std::cout << device.number() << '\n';
}
```

Функция принимает как `Device`, так и `Screen`. Через базовую ссылку доступны операции базового интерфейса. Метод `level()` относится к интерфейсу `Screen`

## Конструктор базового класса

Конструктор производного класса инициализирует базовую часть через список инициализации:

```cpp
class NamedScreen : public Device {
    std::string name;
public:
    NamedScreen(int id, std::string name)
        : Device{id}, name{std::move(name)} {}
};
```

Для примера нужны `<string>` и `<utility>`. Сначала создается `Device`, затем `name`, после этого выполняется тело конструктора `NamedScreen`

При отсутствии записи базы в списке инициализации вызывается ее default constructor. У `Device` задан только конструктор с параметром, поэтому производный конструктор обязан передать идентификатор. Присваивание в теле конструктора происходит после создания базовой части

### Порядок создания и разрушения

```cpp
struct Base {
    Base() { std::cout << "base +\n"; }
    ~Base() { std::cout << "base -\n"; }
};
struct Part {
    Part() { std::cout << "part +\n"; }
    ~Part() { std::cout << "part -\n"; }
};
struct Child : public Base {
    Part part;
    Child() { std::cout << "child +\n"; }
    ~Child() { std::cout << "child -\n"; }
};
```

Создание локального `Child child;` и выход из блока дают последовательность:

```text
base +
part +
child +
child -
part -
base -
```

Для вывода нужен `<iostream>`. Поля создаются в порядке объявления. При разрушении сначала выполняется тело деструктора производного класса, затем поля в обратном порядке, затем базовая часть. Здесь объект уничтожается как локальный `Child`

## Обычные методы и скрытие имен

Обычный метод выбирается по статическому типу выражения. Производный класс может объявить метод с тем же именем:

```cpp
struct Logger {
    void print() const { std::cout << "base\n"; }
};
struct FileLogger : public Logger {
    void print() const { std::cout << "file\n"; }
};

// Внутри функции:
FileLogger file;
const Logger& base = file;
file.print();         // file
base.print();         // base
file.Logger::print(); // base
```

Для вывода нужен `<iostream>`. Имя `FileLogger::print` скрывает имя из базы при поиске через `FileLogger`. Квалификация `Logger::print()` явно выбирает базовую операцию

Скрытие затрагивает также перегрузки с другими параметрами:

```cpp
struct Writer {
    void print(int value) { std::cout << value << '\n'; }
};
struct TextWriter : public Writer {
    using Writer::print;
    void print(const std::string& text) { std::cout << text << '\n'; }
};
```

Нужны `<string>` и `<iostream>`. `using Writer::print` включает базовую перегрузку в доступный набор: `writer.print(7)` и `writer.print(std::string{"ready"})` выбирают подходящие варианты. При удалении `using` имя `TextWriter::print` скрывает базовый набор, вызов с `7` дает ошибку компиляции

## Виртуальные функции

Virtual функция позволяет производному классу задать реализацию, вызываемую через базовую ссылку или указатель. Изменим обычную операцию вывода:

```cpp
struct Message {
    virtual ~Message() = default;
    virtual void print() const { std::cout << "message\n"; }
};
struct Alert : public Message {
    void print() const override { std::cout << "alert\n"; }
};

// Внутри функции:
Alert alert;
const Message& message = alert;
message.print(); // alert
```

Для вывода нужен `<iostream>`. Вызов через `Message&` выбирает реализацию фактического объекта `Alert`. Это основа динамического полиморфизма следующей главы. Само наследование уже используется в предыдущих примерах с обычными методами

### override и final

`override` требует совпадения с virtual функцией базы. Компилятор проверяет параметры, квалификаторы и совместимость типа результата. Для базовой `void print() const` запись `void print() override` вызывает ошибку: у нее отсутствует завершающий `const`

```cpp
struct FinalAlert final : public Message {
    void print() const override { std::cout << "final alert\n"; }
};
```

`final` у класса завершает дальнейшее наследование. `final` у virtual функции завершает дальнейшее переопределение этой функции. Для базового объявления используют `virtual`, для производного - `override`. Переопределенная функция сохраняет virtual поведение

Квалифицированный вызов `alert.Message::print()` явно запускает реализацию базы. Overriding virtual функции и скрытие обычного метода имеют разные правила выбора при обращении через базовую ссылку

## Виртуальный деструктор

Владелец `std::unique_ptr<Message>` удаляет объект через базовый тип. Public virtual destructor обеспечивает разрушение фактического объекта и его базовой части:

```cpp
std::unique_ptr<Message> message = std::make_unique<Alert>();
message->print();
message.reset();
```

Для примера нужен `<memory>`. При `reset()` выполняется цепочка разрушения `Alert`, затем `Message`. Деструктор производного класса также является virtual. Объявление `virtual ~Message() = default` подходит для интерфейса с обычным автоматическим освобождением полей

Удаление производного объекта через базовый указатель со стандартным deleter требует virtual destructor базы. Нарушение этого требования в рассматриваемой модели приводит к undefined behavior

## Virtual вызовы при создании и разрушении

Virtual вызов для создаваемого или разрушаемого объекта из конструктора или деструктора выбирает реализацию текущего класса. Например, вызов `print()` из конструктора `Message` выбирает `Message::print()`, даже если создается `Alert`

Операции, которым требуется полностью созданный производный объект, вызывают после завершения конструирования. Порядок инициализации базы и полей остается тем же, что в примере с обычными классами

## Справочные материалы

- Наследование: [C++ draft - Derived classes](https://eel.is/c++draft/class.derived)
- Доступ при наследовании: [C++ draft - Accessibility of base classes](https://eel.is/c++draft/class.access.base)
- Protected-доступ: [C++ draft - Protected member access](https://eel.is/c++draft/class.protected)
- Скрытие имен: [C++ draft - Member name lookup](https://eel.is/c++draft/class.member.lookup)
- Инициализация базы: [C++ draft - Initializing bases and members](https://eel.is/c++draft/class.base.init)
- Virtual функции: [C++ draft - Virtual functions](https://eel.is/c++draft/class.virtual)
- Конструкторы и деструкторы: [C++ draft - Construction and destruction](https://eel.is/c++draft/class.cdtor)
