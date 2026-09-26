# Lifetime, ownership и RAII

- [Время жизни объекта и scope](#время-жизни-объекта-и-scope)
- [Storage duration](#storage-duration)
- [Ссылки](#ссылки)
  - [Передача по значению и по ссылке](#передача-по-значению-и-по-ссылке)
  - [Возврат по ссылке](#возврат-по-ссылке)
- [Указатели](#указатели)
- [Массивы и арифметика указателей](#массивы-и-арифметика-указателей)
- [Деструктор и порядок разрушения](#деструктор-и-порядок-разрушения)
- [Dynamic allocation](#dynamic-allocation)
  - [Утечка памяти и dangling pointer](#утечка-памяти-и-dangling-pointer)
- [Ownership и RAII](#ownership-и-raii)
- [Копирование объекта](#копирование-объекта)
  - [Класс с собственным массивом](#класс-с-собственным-массивом)
  - [Copy constructor](#copy-constructor)
  - [Copy assignment](#copy-assignment)
- [Перемещение объекта](#перемещение-объекта)
  - [std::move и категории выражений](#stdmove-и-категории-выражений)
  - [Move constructor](#move-constructor)
  - [Move assignment](#move-assignment)
  - [Состояние после перемещения](#состояние-после-перемещения)
- [Rule of Three, Five и Zero](#rule-of-three-five-и-zero)
- [std::unique_ptr](#stdunique_ptr)
  - [Передача владельца и коллекция объектов](#передача-владельца-и-коллекция-объектов)
- [std::shared_ptr](#stdshared_ptr)
- [std::weak_ptr](#stdweak_ptr)
- [Диагностика ошибок памяти](#диагностика-ошибок-памяти)
- [Справочные материалы](#справочные-материалы)

## Время жизни объекта и scope

Время жизни объекта, или lifetime, описывает период его существования во время выполнения программы. Для обычной локальной переменной этот период начинается после завершения инициализации. Для объекта класса вызов деструктора начинает завершение его существования. Во время разрушения (удаления, destruction) действуют специальные правила доступа к объекту.

Scope - область текста программы, в которой доступно имя. Lifetime относится к объекту, scope - к имени.

```cpp
void printTitle() {
    std::string title{"Atlas"};
    {
        const std::string& alias = title;
        std::cout << alias << '\n';
    }
    std::cout << title << '\n';
}
```

Внутренний блок ограничивает scope имени `alias`. Объект `title` продолжает существовать после выхода из этого блока и уничтожается при выходе из функции.

## Storage duration

Storage duration - длительность хранения, то есть период, на который объекту выделена память. Время существования объекта укладывается в период доступности его памяти.

| Вид хранения | Пример                                    | Период хранения                     |
| ------------ | ----------------------------------------- | ----------------------------------- |
| Automatic    | обычная локальная переменная              | до выхода из соответствующего блока |
| Static       | глобальная переменная, локальная `static` | все выполнение программы            |
| Dynamic      | объект, созданный через `new`             | до освобождения выделенной памяти   |

```cpp
int nextId() {
    static int current = 0;
    current++;
    return current;
}
```

Локальная `static` инициализируется при первом проходе через ее объявление. Последующие вызовы используют тот же объект: `nextId()` возвращает `1`, затем `2`, затем `3`. Имя `current` доступно внутри функции.

Память для static-объекта существует всю программу. Момент его инициализации определяется правилами объявления. У локальной переменной с automatic storage каждый вызов функции создает собственный объект. Для `thread_local` предусмотрена отдельная длительность хранения, связанная с потоком. Потоки рассматриваются позднее.

## Ссылки

Ссылка, reference, связывает дополнительное имя с существующим объектом. В объявлении `int&` символ `&` входит в тип ссылки.

```cpp
int value{10};
int& alias = value;
alias = 20;
std::cout << value << '\n'; // 20
```

Ссылка получает объект при инициализации и сохраняет эту привязку. Присваивание через ссылку изменяет значение связанного объекта:

```cpp
int first{10};
int second{30};
int& alias = first;
alias = second;
second = 40;
std::cout << first << '\n'; // 30
```

`first` и `alias` обозначают один объект. Такое совместное обращение к объекту через разные имена называют aliasing.

`const T&` предоставляет доступ для чтения. Сам объект при этом может изменяться через другое имя:

```cpp
int value{10};
const int& view = value;
value = 12;
std::cout << view << '\n'; // 12
```

### Передача по значению и по ссылке

Параметр по значению - отдельный объект. При передаче существующего объекта класса он обычно создается копированием. Параметр `T&` дает функции доступ к исходному объекту для изменения, `const T&` - для чтения.

```cpp
void changeCopy(std::string text) { text += "!"; }
void changeOriginal(std::string& text) { text += "!"; }
void print(const std::string& text) { std::cout << text << '\n'; }
```

```cpp
std::string title{"Atlas"};
changeCopy(title);
print(title);              // Atlas
changeOriginal(title);
print(title);              // Atlas!
```

Небольшие значения вроде `int` обычно передают по значению. Для чтения строки или коллекции подходит `const T&`: функция обращается к исходным данным, сохраняя расходы на создание копии.

### Возврат по ссылке

Возврат `T&` предоставляет доступ к объекту, который продолжает существовать после вызова:

```cpp
int& first(std::vector<int>& values) {
    return values.front();
}

std::vector<int> numbers{10, 20};
first(numbers) = 99;
```

Предусловие `front()` - наличие хотя бы одного элемента. Полученная ссылка остается пригодной, пока соответствующий элемент существует и сохраняет адрес. Например, перераспределение памяти `vector` при росте завершает пригодность старых ссылок на его элементы.

Dangling reference - висячая ссылка на объект, lifetime которого уже завершился. Следующий фрагмент показывает ошибку:

```cpp
const std::string& brokenTitle() {
    std::string title{"Atlas"};
    return title; // ошибка lifetime: локальный объект уничтожается
}
```

Обращение к строке через результат такого вызова приводит к undefined behavior, или UB. При UB стандарт языка снимает требования к поведению программы: возможны случайные результаты и аварийное завершение. Для результата, созданного внутри функции, подходит возврат по значению:

```cpp
std::string makeTitle() {
    return std::string{"Atlas"};
}
```

Прямая привязка локальной `const`-ссылки к временному объекту продлевает его жизнь до конца жизни этой ссылки:

```cpp
const std::string& title = std::string{"Atlas"};
std::cout << title << '\n';
```

Это специальное правило инициализации. Копирование ссылки или возврат ссылки из функции само по себе сохраняет исходный срок жизни объекта.

## Указатели

Указатель (pointer) хранит адрес. Тип `T*` обозначает указатель на `T`. Выражение `&value` получает адрес объекта, `*pointer` обращается к объекту по адресу. Операцию `*` называют разыменованием (получение значения по ссылке).

```cpp
int value{42};
int* pointer = &value;
*pointer = 50;
std::cout << value << '\n'; // 50
```

Указатель сам является объектом: ему можно присвоить другой адрес или пустое значение `nullptr`.

```cpp
int first{10};
int second{20};
int* current = &first;
current = &second;
current = nullptr;
std::cout << (current == nullptr) << '\n'; // 1
```

`nullptr` выражает отсутствие объекта. Разыменование требует указателя на живой объект подходящего типа. Для параметра с допустимым отсутствием значения подходит проверка:

```cpp
void printOptional(const int* value) {
    if (value != nullptr) {
        std::cout << *value << '\n';
    }
}
```

Проверка на `nullptr` различает пустое значение и адрес. За актуальность адреса отвечает код, управляющий lifetime объекта.

Для доступа к полям и методам через указатель используют `->`:

```cpp
std::string title{"Atlas"};
const std::string* pointer = &title;
std::cout << pointer->size() << '\n';
```

`pointer->size()` соответствует `(*pointer).size()` (это одно и то же).

| Объявление                           | Доступ                               |
| ------------------------------------ | ------------------------------------ |
| `const int* pointer = &value;`       | чтение числа, изменение адреса       |
| `int* const pointer = &value;`       | изменение числа, фиксированный адрес |
| `const int* const pointer = &value;` | чтение числа, фиксированный адрес    |

## Массивы и арифметика указателей

`C array` хранит фиксированное количество элементов одного типа подряд в памяти. Для массива из трех элементов допустимы индексы `0`, `1`, `2`:

```cpp
int values[3]{10, 20, 30};
std::cout << values[1] << '\n'; // 20
```

В большинстве выражений имя массива преобразуется в указатель на первый элемент. Это array-to-pointer decay:

```cpp
int* begin = values;
int* end = values + 3;
for (int* current = begin; current != end; current++) {
    std::cout << *current << '\n';
}
```

Прибавление `1` к `int*` сдвигает адрес к следующему элементу `int`. Арифметика указателей ограничена одним массивом и позицией сразу после его конца. Позиция `end` служит границей для сравнения. Разыменование допускается только для элементов массива. Разность указателей внутри одного массива дает расстояние в элементах.

Указатель хранит адрес, размер массива передается отдельно:

```cpp
void printValues(const int* values, std::size_t count) {
    for (std::size_t index{0}; index < count; index++) {
        std::cout << values[index] << '\n';
    }
}
```

Для `std::size_t` подключают `<cstddef>`. Запись параметра `const int values[]` также обозначает указатель. Вызов должен передавать размер, соответствующий доступной памяти. Доступ `values[3]` для массива из трех элементов приводит к UB.

Для прикладного хранения подходят `std::array` из `<array>` и `std::vector` из `<vector>`. Они хранят размер и предоставляют range-for:

```cpp
std::array<int, 3> fixed{10, 20, 30};
std::vector<int> growing{10, 20, 30};
growing.push_back(40);
std::cout << fixed.size() << ' ' << growing.size() << '\n';
```

У `std::array` размер входит в тип; `std::vector` меняет количество элементов во время выполнения. Метод `at(index)` проверяет границы и сообщает об ошибке исключением `std::out_of_range`. Для `operator[]` корректность индекса обеспечивает вызывающий код.

## Деструктор и порядок разрушения

Деструктор - специальный метод `~Type()`, который выполняет действия при уничтожении объекта. У него отсутствуют параметры и тип результата.

```cpp
#include <iostream>
#include <string>

class Trace {
public:
    explicit Trace(const std::string& label) : name{label} {
        std::cout << "create " << name << '\n';
    }
    ~Trace() { std::cout << "destroy " << name << '\n'; }
private:
    std::string name;
};

int main() {
    Trace first{"first"};
    {
        Trace second{"second"};
    }
    std::cout << "outer scope\n";
}
```

Вывод:

```text
create first
create second
destroy second
outer scope
destroy first
```

Локальные объекты уничтожаются в обратном порядке завершения их создания. Выход через `return` также запускает разрушение локальных объектов. После тела деструктора автоматически уничтожаются поля класса в порядке, обратном порядку их объявления. Элементы массива уничтожаются от последнего к первому.

Такое привязанное к выходу из scope разрушение называют детерминированным: место освобождения ресурса определяется структурой программы.

## Dynamic allocation

`new` выделяет память, создает объект и возвращает указатель. `delete` уничтожает этот объект и освобождает память:

```cpp
std::string* title = new std::string{"Atlas"};
std::cout << *title << '\n';
delete title;
title = nullptr;
```

Переменная `title` и строка по ее адресу - два отдельных объекта. У локального указателя automatic storage duration, у созданной строки - dynamic storage duration. Выход указателя из scope сам по себе оставляет динамический объект в памяти.

Для массива используют согласованную пару `new[]` и `delete[]`:

```cpp
std::size_t count{4};
int* values = new int[count]{};
values[0] = 10;
delete[] values;
values = nullptr;
```

`{}` инициализирует элементы `int` нулями. Смешивание `new[]` с `delete` или `new` с `delete[]` приводит к UB. Удаление пустого указателя допустимо. Повторное освобождение одного выделения памяти приводит к UB. Обычный `new` при отказе выделения памяти выбрасывает `std::bad_alloc`.

### Утечка памяти и dangling pointer

Memory leak - утечка памяти: выделенный ресурс остается занятым после потери возможности его освободить. Следующий фрагмент показывает ошибку управления ресурсом:

```cpp
void leak() {
    int* value = new int{10};
    (void)value;
} // адрес потерян, выделенная память остается занятой
```

Dangling pointer - указатель на объект после завершения его lifetime. Он может появиться и после выхода локального объекта из блока, и после `delete`.

```cpp
int* owner = new int{10};
int* observer = owner;
delete owner;
owner = nullptr;
// observer теперь dangling pointer
```

Присваивание `nullptr` изменяет только `owner`. Старые копии адреса требуют отдельного контроля. Чтение `*observer` здесь - use-after-free, обращение к уже освобожденному объекту, которое приводит к UB.

## Ownership и RAII

Ownership - владение ресурсом, то есть ответственность за его освобождение. Ресурсом может быть выделенная память, открытый файл или захваченная блокировка.

Owning object управляет ресурсом. Non-owning pointer или reference предоставляет доступ к ресурсу, время жизни которого обеспечивает другой объект.

RAII расшифровывается как Resource Acquisition Is Initialization: получение ресурса связывают с инициализацией объекта, освобождение - с его деструктором. Например, `std::ofstream` открывает файл при создании и закрывает при уничтожении:

```cpp
#include <fstream>
#include <iostream>

void writeTitle() {
    std::ofstream output{"titles.txt"};
    if (output.fail()) {
        std::cerr << "Failed to open titles.txt\n";
        return;
    }
    output << "Atlas\n";
} // деструктор output закрывает файл
```

Относительный путь отсчитывается от рабочей директории процесса. Приемы чтения чисел через `std::ifstream` и проверки потока разобраны в [основах C++](01-cpp-foundations.md). Для явной проверки результата записи вызывают `output.close()` и проверяют состояние потока. Деструктор обеспечивает освобождение ресурса и при обычном выходе, и при раскрутке стека из-за исключения. Обработка исключений подробно рассматривается в отдельной теме.

Владелец динамической памяти может сам использовать `new` и `delete`. Такой класс реализует RAII, когда берет на себя полный цикл управления ресурсом, включая правила копирования и перемещения.

## Копирование объекта

Copy constructor создает новый объект из существующего. Copy assignment, копирующее присваивание, заменяет состояние уже существующего объекта:

```cpp
std::string first{"Atlas"};
std::string second{first}; // copy constructor
std::string third{"Guide"};
third = first;            // copy assignment
```

Автоматическое копирование класса выполняется по полям. Для `std::string` это копирование строкового значения. Для поля `int*` копируется адрес.

Shallow copy, поверхностное копирование, сохраняет общий адрес ресурса. Если два объекта считают один адрес своим единоличным ресурсом и оба выполняют `delete[]`, второе освобождение приводит к UB.

Deep copy, глубокое копирование, выделяет отдельный ресурс и копирует содержимое. В результате изменение данных одного объекта сохраняет данные другого.

### Класс с собственным массивом

`NumberBlock` хранит фиксированный набор целых чисел. Его инвариант: при положительном `count` поле `data` указывает на собственный массив из `count` элементов. Пустое состояние представлено `count == 0` и `data == nullptr`.

```cpp
#include <cstddef>
#include <stdexcept>
#include <utility>

class NumberBlock {
public:
    explicit NumberBlock(std::size_t size);
    ~NumberBlock();
    NumberBlock(const NumberBlock& other);
    NumberBlock& operator=(const NumberBlock& other);
    NumberBlock(NumberBlock&& other) noexcept;
    NumberBlock& operator=(NumberBlock&& other) noexcept;

    std::size_t size() const { return count; }
    int& at(std::size_t index);
    const int& at(std::size_t index) const;
private:
    std::size_t count;
    int* data;
};
```

Конструктор создает массив, деструктор освобождает его:

```cpp
NumberBlock::NumberBlock(std::size_t size)
    : count{size}, data{size > 0 ? new int[size]{} : nullptr} {}

NumberBlock::~NumberBlock() {
    delete[] data;
}
```

Условное выражение `condition ? first : second` вычисляет выбранную ветвь. При размере `0` класс сразу получает пустое состояние.

Две перегрузки `at()` предоставляют изменяемый доступ обычному объекту и доступ для чтения константному. Проверка предшествует обращению к памяти:

```cpp
int& NumberBlock::at(std::size_t index) {
    if (index >= count) {
        throw std::out_of_range{"NumberBlock index"};
    }
    return data[index];
}

const int& NumberBlock::at(std::size_t index) const {
    if (index >= count) {
        throw std::out_of_range{"NumberBlock index"};
    }
    return data[index];
}
```

### Copy constructor

Параметр `const NumberBlock&` дает доступ к исходному объекту. Делегирующий конструктор сначала создает собственный массив подходящего размера:

```cpp
NumberBlock::NumberBlock(const NumberBlock& other)
    : NumberBlock{other.count} {
    for (std::size_t index{0}; index < count; index++) {
        data[index] = other.data[index];
    }
}
```

Для пустого объекта цикл выполняет ноль итераций. Для заполненного объекта адреса массивов различаются, значения элементов совпадают.

### Copy assignment

Присваивание сначала подготавливает новый массив, затем освобождает прежний:

```cpp
NumberBlock& NumberBlock::operator=(const NumberBlock& other) {
    if (this == &other) {
        return *this;
    }
    int* replacement = other.count > 0 ? new int[other.count]{} : nullptr;
    for (std::size_t index{0}; index < other.count; index++) {
        replacement[index] = other.data[index];
    }
    delete[] data;
    data = replacement;
    count = other.count;
    return *this;
}
```

`this` - указатель на текущий объект; `*this` - сам объект. Проверка `this == &other` распознает присваивание объекта самому себе. Возврат `NumberBlock&` соответствует обычному поведению операции присваивания.

Если выделение памяти завершается исключением, исходное состояние сохраняется. Копирование отдельных `int` выполняется без исключений. Для массива элементов произвольного класса управление временным ресурсом потребовало бы дополнительной защиты.

## Перемещение объекта

Move semantics позволяет передать ресурс между объектами. Для `NumberBlock` перемещение переносит адрес массива и его размер. Исходный объект переходит в явно определенное пустое состояние.

### std::move и категории выражений

Выражение с именем переменной, например `block`, обычно является lvalue: оно обозначает объект с идентичностью. Временный результат `NumberBlock{3}` является prvalue. Выражение `std::move(block)` является xvalue: оно обозначает существующий объект как источник, ресурсы которого разрешено передать. Prvalue и xvalue относятся к rvalue.

`T&&` в обычном параметре move constructor - rvalue reference. Такая ссылка позволяет выбрать операцию перемещения.

`std::move` из `<utility>` выполняет преобразование категории выражения. Передачу ресурса выполняет выбранный конструктор или оператор присваивания:

```cpp
NumberBlock source{3};
NumberBlock target{std::move(source)}; // move constructor
NumberBlock another{1};
another = std::move(target);          // move assignment
```

Перемещение выбирается при наличии подходящей перегрузки. Если доступно только копирование через `const T&`, выражение со `std::move` может вызвать копирование. Для `const`-объекта `std::move` сохраняет `const`; обычная перегрузка с параметром `T&&` требует изменяемого источника.

### Move constructor

```cpp
NumberBlock::NumberBlock(NumberBlock&& other) noexcept
    : count{other.count}, data{other.data} {
    other.count = 0;
    other.data = nullptr;
}
```

Оба объекта продолжают существовать. Новый объект владеет прежним массивом, деструктор исходного объекта получает пустой указатель.

`noexcept` объявляет гарантию завершения функции без выхода через исключение. Это позволяет стандартным контейнерам выбирать перемещение при переносе элементов. Выход исключения из такой функции приводит к `std::terminate` и завершению программы.

### Move assignment

```cpp
NumberBlock& NumberBlock::operator=(NumberBlock&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    delete[] data;
    count = other.count;
    data = other.data;
    other.count = 0;
    other.data = nullptr;
    return *this;
}
```

Целевой объект сначала освобождает собственный массив. Затем он принимает ресурс источника. Проверка адресов сохраняет состояние при перемещающем присваивании самому себе.

### Состояние после перемещения

Moved-from object - объект, из которого выполнили перемещение. Его допустимые операции определяет контракт типа. У `NumberBlock` размер становится равным нулю. Объект допускает запрос размера, копирование, новое присваивание и уничтожение. Вызов `at()` для любого индекса выбрасывает исключение.

```cpp
NumberBlock original{2};
original.at(0) = 7;
NumberBlock copy{original};
copy.at(0) = 9;
NumberBlock moved{std::move(original)};
std::cout << original.size() << '\n'; // 0
std::cout << moved.at(0) << '\n';     // 7
std::cout << copy.at(0) << '\n';      // 9
original = NumberBlock{1};
original.at(0) = 5;
```

Для типов стандартной библиотеки обычно гарантируется корректное состояние с неопределенным содержимым после перемещения. Допустимы уничтожение, присваивание и операции с выполненными предусловиями. Например, у перемещенной строки можно запросить `size()`, обращение к первому символу требует проверки размера. Для `std::unique_ptr` явно гарантируется пустое состояние источника после передачи владения другому объекту.

## Rule of Three, Five и Zero

Special member functions - специальные функции класса, участвующие в создании, копировании, перемещении и уничтожении. Компилятор может объявлять их автоматически. Доступность конкретной операции зависит от полей и уже объявленных специальных функций.

**Rule of Three:** ручное управление ресурсом обычно требует согласованных деструктора, copy constructor и copy assignment.

**Rule of Five:** к этим трем операциям добавляют move constructor и move assignment. `NumberBlock` определяет все пять. Объявленный пользователем деструктор подавляет автоматическое объявление move-операций, требуемое поведение задают явно.

Иногда ресурс имеет единственного владельца и копирование по контракту запрещено. Такой интерфейс выражают через `= delete`:

```cpp
NumberBlock(const NumberBlock&) = delete;
NumberBlock& operator=(const NumberBlock&) = delete;
```

Это альтернативные объявления для варианта класса с передачей владения только через перемещение. Попытка копирования такого типа вызывает ошибку компиляции.

**Rule of Zero:** ресурсами управляют готовые RAII-поля, специальные функции внешнего класса формирует компилятор. Для массива значений подходит `std::vector`:

```cpp
#include <cstddef>
#include <vector>

class NumberList {
public:
    explicit NumberList(std::size_t size) : data(size) {}
    int& at(std::size_t index) { return data.at(index); }
    const int& at(std::size_t index) const { return data.at(index); }
private:
    std::vector<int> data;
};
```

`data(size)` создает `size` нулевых элементов. Для `vector<int>` запись `data{size}` участвовала бы в выборе конструктора списка значений, круглые скобки здесь явно выражают создание по количеству.

Копирование `NumberList` копирует содержимое `vector`, перемещение использует его move-операции, уничтожение освобождает его память (Rule of Zero - основной подход к прикладным классам лабораторных работ). Поле `unique_ptr` также управляет ресурсом автоматически. Содержащий его класс обычно допускает перемещение, а попытка автоматического копирования вызывает ошибку компиляции.

## std::unique_ptr

`std::unique_ptr<T>` из `<memory>` выражает единоличное владение. `std::make_unique<T>(arguments)` создает объект и сразу помещает его под управление владельца:

```cpp
#include <memory>
#include <string>

std::unique_ptr<std::string> title = std::make_unique<std::string>("Atlas");
std::cout << *title << ' ' << title->size() << '\n';
```

Деструктор `unique_ptr` уничтожает управляемый объект. Для передачи владения используют перемещение:

```cpp
auto first = std::make_unique<int>(42);
auto second = std::move(first);
if (first == nullptr) {
    std::cout << "Ownership transferred\n";
}
second.reset(); // освобождение объекта, second становится пустым
```

Копирующие операции `unique_ptr` запрещены его интерфейсом. Вызов `get()` предоставляет raw pointer для временного доступа. Ответственность за удаление сохраняется у `unique_ptr`:

```cpp
auto owner = std::make_unique<int>(42);
const int* observer = owner.get();
printOptional(observer);
```

После уничтожения управляемого объекта наблюдатель становится dangling pointer. Вызов `delete` для результата `get()` нарушает контракт владения.

### Передача владельца и коллекция объектов

Параметр `std::unique_ptr<T>` по значению выражает прием владения. Вектор может хранить такие владельцы:

```cpp
void addTitle(std::vector<std::unique_ptr<std::string>>& titles,
              std::unique_ptr<std::string> title) {
    titles.push_back(std::move(title));
}

std::vector<std::unique_ptr<std::string>> titles;
auto title = std::make_unique<std::string>("Atlas");
addTitle(titles, std::move(title));
```

Именованный параметр `title` внутри функции является lvalue, поэтому передача его ресурса в вектор выражена через `std::move`. При уничтожении вектора уничтожаются владельцы и их строки. Для функции, которой требуется только чтение строки, подходит параметр `const std::string&`.

Для динамического массива существует `std::unique_ptr<T[]>`:

```cpp
auto values = std::make_unique<int[]>(3);
values[0] = 10;
```

Вариант `unique_ptr<T[]>` использует `delete[]`. Размер массива хранится отдельно, `std::vector` объединяет владение, размер и операции изменения коллекции.

## std::shared_ptr

`std::shared_ptr<T>` из `<memory>` выражает совместное владение. Копии указателя участвуют в управлении одним объектом:

```cpp
auto first = std::make_shared<std::string>("Atlas");
auto second = first;
*second = "Guide";
std::cout << *first << '\n'; // Guide
first.reset();
std::cout << *second << '\n'; // Guide
```

`std::make_shared` создает объект и управляющую структуру со счетчиками владения. Копирование `shared_ptr` добавляет владельца, уничтожение или `reset()` убирает его. Последний владелец освобождает объект. Копирование указателя оставляет общий объект, поэтому изменение строки видно через обе копии.

Все участники совместного владения должны использовать одну управляющую структуру: новый владелец создается копированием существующего `shared_ptr`. Создание второго `shared_ptr` из `first.get()` заводит отдельное управление тем же адресом и приводит к повторному удалению.

`shared_ptr` подходит, когда lifetime действительно определяется несколькими владельцами. Для доступа к уже существующему объекту подходят ссылка или наблюдающий указатель. Для одного владельца - `unique_ptr`.

## std::weak_ptr

`std::weak_ptr<T>` наблюдает объект, находящийся под управлением `shared_ptr`. Его существование сохраняет сведения о владении. Срок жизни самого объекта определяется сильными владельцами `shared_ptr`.

Метод `lock()` пытается получить временного сильного владельца. Результат - `shared_ptr` на живой объект либо пустой `shared_ptr`:

```cpp
auto owner = std::make_shared<std::string>("Atlas");
std::weak_ptr<std::string> observer = owner;
if (auto locked = observer.lock()) {
    std::cout << *locked << '\n';
}
owner.reset();
if (observer.expired()) {
    std::cout << "Object destroyed\n";
}
```

Успешный `lock()` удерживает объект на все время существования `locked`. `expired()` сообщает о завершении совместного владения, для доступа к объекту используют результат `lock()`.

Если два объекта владеют друг другом через `shared_ptr`, возникает цикл сильных ссылок: счетчики удерживают оба объекта даже после удаления внешних владельцев. Наблюдающую связь в таком цикле выражают через `weak_ptr`. Например, родитель может владеть дочерним объектом, а дочерний объект - наблюдать родителя через `weak_ptr`.

## Диагностика ошибок памяти

Предупреждения компилятора помогают находить подозрительные операции, например возврат ссылки на локальный объект. AddressSanitizer проверяет обращения к памяти во время выполнения и обнаруживает многие выходы за границы, use-after-free и повторные освобождения. UndefinedBehaviorSanitizer выявляет отдельные категории UB.

Для Clang или GCC с поддержкой соответствующих sanitizers:

```bash
clang++ -std=c++20 -Wall -Wextra -Wpedantic -g -O1 \
    -fsanitize=address,undefined -fno-omit-frame-pointer main.cpp -o app
./app
```

Отчет sanitizer указывает место обнаруженного обращения и часто содержит сведения о выделении и освобождении памяти. Диагностика относится к выполненным путям программы, полнота проверки зависит от входных данных. Поддержка поиска утечек зависит от платформы и конфигурации инструмента.

## Справочные материалы

- Lifetime: [проект стандарта C++ - basic.life](https://eel.is/c++draft/basic.life).
- Длительность хранения: [cppreference - Storage duration](https://en.cppreference.com/w/cpp/language/storage_duration).
- Ссылки: [cppreference - Reference declaration](https://en.cppreference.com/w/cpp/language/reference).
- Выделение и освобождение: [cppreference - new](https://en.cppreference.com/w/cpp/language/new), [delete](https://en.cppreference.com/w/cpp/language/delete).
- Копирование и перемещение: [cppreference - Rule of three/five/zero](https://en.cppreference.com/w/cpp/language/rule_of_three), [std::move](https://en.cppreference.com/w/cpp/utility/move).
- RAII: [C++ Core Guidelines - R.1](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-raii).
- Специальные функции: [C++ Core Guidelines - C.20](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-zero).
- Единоличное владение: [проект стандарта C++ - unique.ptr](https://eel.is/c++draft/unique.ptr).
- Совместное владение: [cppreference - shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr).
- Наблюдение: [проект стандарта C++ - weak_ptr observers](https://eel.is/c++draft/util.smartptr.weak.obs).
- Проверка памяти: [Clang - AddressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html), [UndefinedBehaviorSanitizer](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html).
