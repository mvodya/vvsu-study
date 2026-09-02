# Основы C++ и модель компиляции

[[*TOC*]]

## Структура программы

Выполнение программы начинается с функции `main`.

```cpp
#include <iostream>

int main() {
    std::cout << "Hello\n";
    return 0;
}
```

`return 0` сообщает операционной системе об успешном завершении программы (см. [exit коды для Linux](https://man7.org/linux/man-pages/man3/sysexits.h.3head.html)).

Тело функции состоит из statements:

```cpp
int value = 10;
value = value + 5;
std::cout << value << '\n';
```

Expression вычисляет значение:

```cpp
value + 5
value > 10
value * 2
```

Имена переменных, функций и типов действуют внутри определенного scope.

```cpp
int main() {
    int outer = 10;

    { // scope start
        int inner = 20;
        std::cout << outer + inner << '\n';
    } // scope end

    // Здесь доступен outer

    return 0;
}
```

Фигурные скобки создают новый блок и новый scope.

## Базовые типы

Часто используемые фундаментальные типы:

```cpp
bool enabled = true;

char letter = 'A';

int count = 42;
long long largeCount = 1'000'000'000LL;

float ratio = 0.5f;
double temperature = 21.7;
```

`bool` хранит логическое значение.

Целочисленные типы хранят целые числа.

`float` и `double` хранят числа с плавающей точкой.

`char` хранит символ.

Размер типа можно получить через `sizeof`:

```cpp
std::cout << sizeof(int) << '\n';
std::cout << sizeof(double) << '\n';
```

## Переменные и инициализация

Для создания объекта сразу задавайте его начальное значение:

```cpp
int count = 0;
double value = 12.5;
bool active = true;
```

или

```cpp
int count{0};
double value{12.5};
bool active{true};
```

Форма `{}` позволяет компилятору диагностировать narrowing conversion:

```cpp
double source{12.5};
int value{source}; // narrowing -> compiler diagnostic

int a = 2.2; // OK (a будет равен 2)
int b{2.2};  // error: type 'double' cannot be narrowed to 'int' in initializer list
```

Явное преобразование типа записывается через `static_cast`:

```cpp
double source{12.5};
int value{static_cast<int>(source)};
```

## const

`const` обозначает значение, которое сохраняется после инициализации:

```cpp
const int maxCount = 100;
const double pi = 3.1415926535;
```

Именованные константы подходят для порогов, коэффициентов и других значений с предметным смыслом:

```cpp
const int maxAttempts = 5;
const double taxRate = 0.2;
```

## auto

`auto` позволяет компилятору вывести тип из initializer:

```cpp
auto count = 10;        // int
auto price = 15.75;     // double
auto enabled = true;    // bool
```

`auto` удобно применять, когда тип ясно следует из выражения:

```cpp
std::string text = "example";
auto length = text.size();
```

## Условия

### if

```cpp
if (value > 100) {
    std::cout << "high\n";
} else if (value > 50) {
    std::cout << "medium\n";
} else {
    std::cout << "low\n";
}
```

Условие должно давать логическое значение.

Логические операции:

```cpp
a && b
a || b
!a
```

Операции сравнения:

```cpp
a == b
a != b
a < b
a <= b
a > b
a >= b
```

### switch

`switch` удобен для выбора по одному дискретному значению:

```cpp
switch (command) {
case 1:
    std::cout << "open\n";
    break;

case 2:
    std::cout << "save\n";
    break;

default:
    std::cout << "unknown\n";
    break;
}
```

## Циклы

### for

```cpp
for (int i = 0; i < 10; ++i) {
    std::cout << i << '\n';
}
```

### while

```cpp
int value = 1;

while (value < 100) {
    value *= 2;
}
```

### range-for

Range-for проходит по элементам последовательности:

```cpp
std::string text = "hello";

for (char ch : text) {
    std::cout << ch << '\n';
}
```

### break и continue

`break` завершает текущий цикл:

```cpp
for (int i = 0; i < 100; ++i) {
    if (i == 10) {
        break;
    }
}
```

`continue` переходит к следующей итерации:

```cpp
for (int i = 0; i < 10; ++i) {
    if (i == 5) {
        continue;
    }

    std::cout << i << '\n';
}
```

## Функции

Функция объединяет отдельную операцию программы.

```cpp
int square(int value) {
    return value * value;
}
```

`int` перед именем функции задает тип возвращаемого значения.

`value` является параметром.

```cpp
int result = square(5);
```

`5` является аргументом вызова.

### Declaration и definition

Declaration сообщает компилятору имя функции, тип результата и параметры:

```cpp
double average(double first, double second);
```

Definition содержит тело функции:

```cpp
double average(double first, double second) {
    return (first + second) / 2.0;
}
```

Declaration позволяет вызвать функцию из translation unit, где ее definition располагается в другом файле.

### Перегрузка функций

Одно имя может иметь несколько вариантов параметров:

```cpp
int maxValue(int first, int second) {
    return first > second ? first : second;
}

double maxValue(double first, double second) {
    return first > second ? first : second;
}
```

Компилятор выбирает подходящую функцию по аргументам вызова.

### Аргументы по умолчанию

```cpp
int multiply(int value, int factor = 2);

int multiply(int value, int factor) {
    return value * factor;
}
```

Вызовы:

```cpp
multiply(10);
multiply(10, 5);
```

## std::string

`std::string` представляет строку.

```cpp
#include <string>

std::string first = "Hello";
std::string second = "world";

std::string result = first + " " + second;
```

Полезные операции:

```cpp
std::string text = "abcdef";

auto length = text.size();
char first = text[0];
std::string part = text.substr(2, 3);
```

Вывод:

```cpp
std::cout << text << '\n';
```

Чтение одного значения:

```cpp
std::string word;
std::cin >> word;
```

Чтение всей строки:

```cpp
std::string line;
std::getline(std::cin, line);
```

Оператор `>>` читает форматированное значение. Для строки чтение завершается на whitespace.

`std::getline` читает строку до символа перевода строки либо до указанного delimiter.

## struct

`struct` объединяет несколько связанных значений в один пользовательский тип.

```cpp
struct Book {
    std::string title;
    int year;
    double rating;
};
```

Создание объекта через aggregate initialization:

```cpp
Book book{"Dune", 1965, 4.8};
```

Доступ к полям:

```cpp
std::cout << book.title << '\n';
std::cout << book.year << '\n';
```

Структуры могут содержать другие структуры:

```cpp
struct Point {
    double x;
    double y;
};

struct Segment {
    Point begin;
    Point end;
};
```

Это пример composition.

## enum class

`enum class` задает ограниченный набор именованных значений.

```cpp
enum class Direction {
    North,
    East,
    South,
    West
};
```

Использование:

```cpp
Direction direction = Direction::North;
```

С `switch`:

```cpp
switch (direction) {
case Direction::North:
    std::cout << "north\n";
    break;

case Direction::East:
    std::cout << "east\n";
    break;

case Direction::South:
    std::cout << "south\n";
    break;

case Direction::West:
    std::cout << "west\n";
    break;
}
```

`enum class` сохраняет значения внутри собственного scope:

```cpp
Direction::North
Direction::South
```

## Потоки ввода и вывода

Заголовок:

```cpp
#include <iostream>
```

Основные стандартные потоки:

```cpp
std::cin
std::cout
std::cerr
```

Пример:

```cpp
int value{};

std::cout << "Enter value: ";
std::cin >> value;

std::cout << "Value: " << value << '\n';
```

`std::cerr` используется для сообщений об ошибках:

```cpp
std::cerr << "File error\n";
```

`'\n'` добавляет перевод строки.

`std::endl` добавляет перевод строки и выполняет flush потока.

## Работа с файлами

Файловые потоки находятся в `<fstream>`.

### Чтение

```cpp
#include <fstream>
#include <iostream>

int main() {
    std::ifstream input{"data.txt"};

    if (input.fail()) {
        std::cerr << "Failed to open data.txt\n";
        return 1;
    }

    int value{};

    while (input >> value) {
        std::cout << value << '\n';
    }

    return 0;
}
```

`std::ifstream` открывает файл для чтения.

Условие:

```cpp
while (input >> value)
```

выполняет чтение и использует состояние потока как условие продолжения цикла.

### Запись

```cpp
#include <fstream>

int main() {
    std::ofstream output{"result.txt"};

    output << "alpha " << 15 << '\n';
    output << "beta " << 27 << '\n';

    return 0;
}
```

`std::ofstream` открывает файл для записи.

### Добавление в конец файла

```cpp
std::ofstream output{"log.txt", std::ios::app};
output << "new entry\n";
```

### Явное закрытие

Файловый поток закрывает связанный файл при разрушении объекта.

Досрочное закрытие выполняется через:

```cpp
output.close();
```

### Пути к файлам

Относительный путь вычисляется от current working directory процесса:

```cpp
std::ifstream input{"data.txt"};
std::ifstream input2{"input/data.txt"};
```

При запуске программы из IDE current working directory задается конфигурацией запуска.

## Чтение структурированных данных

Пусть файл содержит:

```text
A12 18.5 4
B07 21.0 9
C03 14.25 2
```

Каждая строка содержит строку, вещественное число и целое число.

Чтение можно выполнить напрямую:

```cpp
std::ifstream input{"data.txt"};

std::string code;
double value{};
int count{};

while (input >> code >> value >> count) {
    std::cout
        << code << ' '
        << value << ' '
        << count << '\n';
}
```

Поток сам преобразует текстовое представление чисел в соответствующие числовые типы.

## Построчное чтение

`std::getline` подходит для обработки данных строками:

```cpp
std::ifstream input{"data.txt"};

std::string line;

while (std::getline(input, line)) {
    std::cout << line << '\n';
}
```

Этот подход удобен, когда дальнейший parsing выполняется отдельно.

## std::istringstream

`std::istringstream` создает входной поток поверх строки.

Заголовок:

```cpp
#include <sstream>
```

Пример:

```cpp
std::string line{"A12 18.5 4"};

std::istringstream stream{line};

std::string code;
double value{};
int count{};

stream >> code >> value >> count;
```

Построчный parsing файла:

```cpp
std::ifstream input{"data.txt"};

std::string line;

while (std::getline(input, line)) {
    std::istringstream stream{line};

    std::string code;
    double value{};
    int count{};

    if (stream >> code >> value >> count) {
        std::cout << code << ' ' << value << ' ' << count << '\n';
    }
}
```

Такой способ разделяет чтение файла и parsing конкретной строки.

## Parsing с delimiter

Пусть строка имеет формат:

```text
alpha;42;18.75
```

Поля можно извлечь через `std::getline` с delimiter:

```cpp
std::string line = "alpha;42;18.75";
std::istringstream stream{line};

std::string name;
std::string countText;
std::string valueText;

std::getline(stream, name, ';');
std::getline(stream, countText, ';');
std::getline(stream, valueText, ';');
```

Преобразование строк в числа:

```cpp
int count = std::stoi(countText);
double value = std::stod(valueText);
```

Основные функции преобразования:

```cpp
std::stoi
std::stol
std::stoll

std::stof
std::stod
std::stold
```

Для корректных учебных входных данных такого parsing достаточно.

## Преобразование числового поля в bool

Пусть файл хранит `0` и `1`:

```text
A 0
B 1
```

Чтение:

```cpp
std::string code;
int flagValue{};

input >> code >> flagValue;

bool flag{static_cast<bool>(flagValue)};
```

## Namespace

Namespace группирует связанные имена:

```cpp
namespace math {
    int square(int value) {
        return value * value;
    }
}
```

Вызов с qualified name:

```cpp
int value = math::square(5);
```

`using` добавляет выбранное имя в текущий scope:

```cpp
using math::square;

int value = square(5);
```

Namespace особенно полезен для типов и функций отдельной предметной области или библиотеки.

## Заголовочные и исходные файлы

Типичная многомодульная программа:

```text
main.cpp
math.hpp
math.cpp
```

### math.hpp

```cpp
#pragma once

namespace math {

int square(int value);

}
```

### math.cpp

```cpp
#include "math.hpp"

namespace math {

int square(int value) {
    return value * value;
}

}
```

### main.cpp

```cpp
#include "math.hpp"

#include <iostream>

int main() {
    std::cout << math::square(8) << '\n';
    return 0;
}
```

Заголовочный файл содержит интерфейс модуля: объявления типов и функций.

`.cpp` содержит definitions функций.

`#pragma once` обеспечивает однократное включение содержимого заголовка в одну translation unit.

## #include

Препроцессор обрабатывает `#include` перед основной компиляцией.

```cpp
#include <iostream>
#include "math.hpp"
```

Форма `<...>` обычно используется для стандартных и системных headers.

Форма `"..."` обычно используется для headers проекта.

Содержимое подключаемого header участвует в формировании translation unit.

## Preprocessor

Preprocessor работает с директивами, начинающимися с `#`.

Основные примеры:

```cpp
#include <iostream>
#define APP_VERSION "1.0"
```

Macro заменяет preprocessing tokens:

```cpp
#define APP_VERSION "1.0"

std::cout << APP_VERSION << '\n';
```

После preprocessing компилятор получает результат раскрытия `#include` и macros.

Результат preprocessing можно получить отдельно:

```bash
g++ -std=c++20 -E main.cpp -o main.i
```

Для программных констант курса применяется обычный механизм C++:

```cpp
const int maxItems = 100;
```

## Translation unit

Каждый `.cpp` файл проходит preprocessing отдельно.

Упрощенная схема:

```text
source file
-> preprocessor
-> translation unit
-> compiler
-> object file
```

Для проекта:

```text
main.cpp -> main.o
math.cpp -> math.o
```

Каждая translation unit компилируется самостоятельно.

Затем linker объединяет object files:

```text
main.o
math.o
-> linker
-> executable
```

Модель всей сборки:

```text
source
-> preprocessing
-> translation unit
-> compilation
-> object file
-> linking
-> executable
```

Cppreference описывает C++ translation phases и отдельную обработку translation units.

## Отдельная компиляция

Для GCC:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -c main.cpp -o main.o
g++ -std=c++20 -Wall -Wextra -Wpedantic -c math.cpp -o math.o
```

Флаг `-c` выполняет компиляцию до object file.

Linking:

```bash
g++ main.o math.o -o app
```

Запуск в Linux и macOS:

```bash
./app
```

`clang++` поддерживает аналогичную схему:

```bash
clang++ -std=c++20 -c main.cpp -o main.o
clang++ -std=c++20 -c math.cpp -o math.o
clang++ main.o math.o -o app
```

Clang также разделяет preprocessing, compilation, assembly и linking на стадии, которыми управляет compiler driver.

## Declaration, definition и symbol

Declaration вводит имя и сообщает его тип:

```cpp
int square(int value);
```

Definition предоставляет реализацию:

```cpp
int square(int value) {
    return value * value;
}
```

При компиляции функций и глобальных объектов создаются symbols, которые linker использует для связывания translation units.

Например:

```text
main.cpp
-> вызывает math::square

math.cpp
-> определяет math::square
```

Linker связывает вызов с соответствующей definition.

## Linker errors

### undefined reference

Пример команды:

```bash
g++ main.o -o app
```

`main.o` содержит вызов `math::square`.

Definition `math::square` находится в `math.o`.

При таком наборе входных файлов linker сообщает `undefined reference`.

Корректная команда включает оба object files:

```bash
g++ main.o math.o -o app
```

### multiple definition

Если два object files содержат обычную definition одной функции с external linkage:

```text
first.o -> definition process()
second.o -> definition process()
```

linker получает несколько definitions одного symbol и сообщает `multiple definition`.

## One Definition Rule

One Definition Rule задает правила количества definitions сущностей программы.

Для обычной функции с external linkage программа содержит одну definition:

```cpp
int process(int value) {
    return value * 2;
}
```

Declaration может встречаться в нескольких translation units через header:

```cpp
int process(int value);
```

`inline` позволяет размещать одинаковую definition функции в header:

```cpp
inline int doubleValue(int value) {
    return value * 2;
}
```

Классы, структуры и перечисления обычно определяются в headers и включаются в несколько translation units.

Подробное описание declarations, definitions и ODR:

## Linkage

Функция в namespace scope обычно имеет external linkage:

```cpp
int calculate(int value) {
    return value * 2;
}
```

Такой symbol может участвовать в linking между translation units.

Unnamed namespace задает internal linkage:

```cpp
namespace {

int helper(int value) {
    return value + 1;
}

}
```

`helper` принадлежит одной translation unit.

Internal linkage удобно для вспомогательных сущностей реализации конкретного `.cpp`.

## Ошибки компиляции, warnings и linker errors

Compiler diagnostic может сообщать о синтаксисе, типах и других свойствах исходного кода.

Пример ошибки компиляции:

```cpp
int value{"text"};
```

Warning сообщает о подозрительной конструкции, для которой compiler продолжает обработку.

Полезный набор для GCC и Clang:

```text
-Wall
-Wextra
-Wpedantic
```

Пример:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic main.cpp
```

GCC содержит эти флаги среди основных warning options.

Linker diagnostics относятся к symbols и связи между object files:

```text
undefined reference
multiple definition
```

## Базовая работа с debugger

Debugger позволяет остановить выполнение программы в выбранной точке и изучить ее состояние.

Основные операции:

```text
breakpoint
-> запуск
-> остановка
-> просмотр переменных
-> выполнение по шагам
-> просмотр call stack
```

Breakpoint ставится на строку исходного кода.

Step over выполняет текущую строку.

Step into входит внутрь вызываемой функции.

Call stack показывает цепочку активных вызовов функций.

Просмотр значений переменных помогает отслеживать изменение состояния программы.

Для добавления debug information GCC и Clang поддерживают флаг:

```bash
-g
```

Например:

```bash
g++ -std=c++20 -g main.cpp -o app
```

## Справочные материалы

- C++ language reference: [cppreference - C++ language](https://en.cppreference.com/cpp/language)
- `std::string`: [cppreference - std::string](https://en.cppreference.com/cpp/string/basic_string)
- Файловые потоки: [cppreference - std::basic_fstream](https://en.cppreference.com/cpp/io/basic_fstream)
- Translation phases: [cppreference - Phases of translation](https://en.cppreference.com/cpp/language/translation_phases)
- Declarations, definitions и ODR: [cppreference - Definitions and ODR](https://en.cppreference.com/cpp/language/definition)
- GCC options: [GCC - Option Summary](https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html)
- Clang compilation stages: [Clang Command Guide](https://clang.llvm.org/docs/CommandGuide/clang.html)
