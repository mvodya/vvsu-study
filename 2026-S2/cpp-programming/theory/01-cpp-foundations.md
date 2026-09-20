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

Тело функции состоит из инструкций (statements):

```cpp
int value = 10;
value = value + 5;
std::cout << value << '\n';
```

Выражение (expression) вычисляет значение:

```cpp
value + 5
value > 10
value * 2
```

Имена переменных, функций и типов действуют внутри определенной области видимости (scope).

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

Фигурные скобки создают новый блок и новую область видимости.

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

Форма `{}` позволяет компилятору обнаружить сужающее преобразование (narrowing conversion), при котором значение может потерять часть данных:

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

`auto` позволяет компилятору определить тип по начальному значению:

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

### Объявление и определение

Declaration (объявление) сообщает компилятору имя функции, тип результата и параметры:

```cpp
double average(double first, double second);
```

Definition (определение) содержит тело функции:

```cpp
double average(double first, double second) {
    return (first + second) / 2.0;
}
```

Объявление позволяет вызвать функцию, тело которой находится в другом `.cpp` файле.

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

Оператор `>>` читает форматированное значение. Для строки чтение завершается на пробельном символе: пробеле, табуляции или переводе строки.

`std::getline` читает строку до символа перевода строки либо до указанного разделителя.

## struct

`struct` объединяет несколько связанных значений в один пользовательский тип.

```cpp
struct Book {
    std::string title;
    int year;
    double rating;
};
```

Агрегатная инициализация задает значения полей по порядку их объявления:

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

Это пример композиции: объект `Segment` содержит два объекта `Point`.

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

К значениям `enum class` обращаются через имя перечисления:

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

`std::endl` добавляет перевод строки и сбрасывает буфер потока: накопленный вывод передается дальше, например в терминал.

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

Относительный путь вычисляется от текущей рабочей папки процесса:

```cpp
std::ifstream input{"data.txt"};
std::ifstream input2{"input/data.txt"};
```

При запуске программы из IDE текущая рабочая папка задается настройками запуска.

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

Этот подход удобен, когда парсинг - разбор строки на отдельные значения - выполняется отдельно.

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

Построчный парсинг файла:

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

Такой способ разделяет чтение файла и парсинг конкретной строки.

## Парсинг с разделителем

Пусть строка имеет формат:

```text
alpha;42;18.75
```

Поля можно извлечь через `std::getline` с разделителем:

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

Для корректных учебных входных данных такого парсинга достаточно.

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

Namespace (пространство имен) группирует связанные имена:

```cpp
namespace math {
    int square(int value) {
        return value * value;
    }
}
```

Вызов с полным именем, включающим пространство имен:

```cpp
int value = math::square(5);
```

`using` добавляет выбранное имя в текущую область видимости:

```cpp
using math::square;

int value = square(5);
```

Пространство имен удобно использовать для типов и функций одной библиотеки или части программы.

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

`.cpp` содержит определения функций.

`#pragma once` обеспечивает однократное включение содержимого заголовка при обработке одного исходного файла.

## #include

Препроцессор обрабатывает `#include` перед основной компиляцией.

```cpp
#include <iostream>
#include "math.hpp"
```

Форма `<...>` обычно используется для стандартных и системных заголовочных файлов.

Форма `"..."` обычно используется для заголовочных файлов проекта.

Препроцессор подставляет содержимое подключаемого заголовка в место директивы `#include`.

## Препроцессор

Препроцессор работает с директивами, начинающимися с `#`.

Основные примеры:

```cpp
#include <iostream>
#define APP_VERSION "1.0"
```

Директива `#define` создает макрос. В этом примере препроцессор заменит `APP_VERSION` на `"1.0"`:

```cpp
#define APP_VERSION "1.0"

std::cout << APP_VERSION << '\n';
```

После работы препроцессора компилятор получает код с раскрытыми `#include` и макросами.

Результат работы препроцессора можно получить отдельно:

```bash
g++ -std=c++20 -E main.cpp -o main.i
```

Для программных констант курса применяется обычный механизм C++:

```cpp
const int maxItems = 100;
```

## Translation unit

Translation unit (единица трансляции, TU) - код одного исходного файла после обработки препроцессором, включая содержимое подключенных заголовков.

Каждый `.cpp` файл обрабатывается отдельно и дает свою TU.

Упрощенная схема:

```text
исходный файл
-> препроцессор
-> TU
-> компилятор
-> объектный файл
```

Для проекта:

```text
main.cpp -> main.o
math.cpp -> math.o
```

Каждая TU компилируется самостоятельно.

Затем компоновщик (linker) объединяет объектные файлы в исполняемый файл:

```text
main.o
math.o
-> компоновщик
-> исполняемый файл
```

Модель всей сборки:

```text
исходный файл
-> обработка препроцессором
-> TU
-> компиляция
-> объектный файл
-> компоновка
-> исполняемый файл
```

Подробные стадии преобразования исходного кода описаны в справочнике cppreference в разделе Phases of translation.

## Отдельная компиляция

Для GCC:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -c main.cpp -o main.o
g++ -std=c++20 -Wall -Wextra -Wpedantic -c math.cpp -o math.o
```

Флаг `-c` останавливает сборку на объектном файле.

Компоновка:

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

Команда `clang++` управляет стадиями сборки: обработкой препроцессором, компиляцией, ассемблированием и компоновкой.

## Объявление, определение и символ

Объявление вводит имя и сообщает его тип:

```cpp
int square(int value);
```

Определение содержит реализацию:

```cpp
int square(int value) {
    return value * value;
}
```

При компиляции сведения об именах функций и глобальных объектов попадают в объектные файлы. Эти имена называют символами (symbols). По ним компоновщик связывает вызов функции из одной TU с ее определением в другой.

Например:

```text
main.cpp
-> вызывает math::square

math.cpp
-> определяет math::square
```

Компоновщик связывает вызов с соответствующим определением.

## Ошибки компоновки

### undefined reference

Пример команды:

```bash
g++ main.o -o app
```

`main.o` содержит вызов `math::square`.

Определение `math::square` находится в `math.o`.

При таком наборе входных файлов компоновщик сообщает `undefined reference`. В зависимости от инструментов сообщение может также содержать `undefined symbol`.

Корректная команда включает оба объектных файла:

```bash
g++ main.o math.o -o app
```

### multiple definition

Если два объектных файла содержат определения одной и той же обычной функции, доступной из других файлов:

```text
first.o -> определение process()
second.o -> определение process()
```

компоновщик получает несколько определений одного символа и сообщает `multiple definition` или `duplicate symbol`.

## One Definition Rule

One Definition Rule (правило одного определения, ODR) задает, где и сколько раз можно определять функции, переменные и типы.

Для обычной функции, доступной из других файлов, программа содержит одно определение:

```cpp
int process(int value) {
    return value * 2;
}
```

Объявление может встречаться в нескольких TU через заголовочный файл:

```cpp
int process(int value);
```

`inline` позволяет размещать одинаковое определение функции в заголовочном файле:

```cpp
inline int doubleValue(int value) {
    return value * 2;
}
```

Классы, структуры и перечисления обычно определяются в заголовочных файлах и включаются в несколько TU.

## Связывание имен

Связывание (linkage) определяет, могут ли объявления в разных TU обозначать одну и ту же функцию или переменную.

Обычная функция, объявленная вне других функций, имеет внешнее связывание (external linkage):

```cpp
int calculate(int value) {
    return value * 2;
}
```

Такую функцию можно объявить и вызвать в другом `.cpp` файле.

Безымянное пространство имен задает внутреннее связывание (internal linkage):

```cpp
namespace {

int helper(int value) {
    return value + 1;
}

}
```

Имя `helper` относится к функции только внутри этой TU.

Внутреннее связывание удобно для вспомогательных функций конкретного `.cpp` файла.

## Ошибки и предупреждения при сборке

Сообщение компилятора может указывать на ошибку в синтаксисе, типах и других частях исходного кода.

Пример ошибки компиляции:

```cpp
int value{"text"};
```

Предупреждение (warning) сообщает о подозрительной конструкции. При обычных настройках компилятор продолжает сборку после предупреждения.

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

Эти флаги включают дополнительные предупреждения компилятора.

Сообщения компоновщика относятся к символам и связям между объектными файлами:

```text
undefined reference
multiple definition
```

## Базовая работа с отладчиком

Отладчик (debugger) позволяет остановить выполнение программы в выбранной точке и изучить ее состояние.

Основные операции:

```text
точка останова
-> запуск
-> остановка
-> просмотр переменных
-> выполнение по шагам
-> просмотр стек вызовов
```

Точка останова (breakpoint) ставится на строку исходного кода.

Step over выполняет текущую строку целиком, включая вызванные функции.

Step into входит внутрь вызываемой функции для выполнения по шагам.

Стек вызовов (call stack) показывает цепочку активных вызовов функций.

Просмотр значений переменных помогает отслеживать изменение состояния программы.

Для добавления отладочной информации GCC и Clang поддерживают флаг:

```bash
-g
```

Например:

```bash
g++ -std=c++20 -g main.cpp -o app
```

## Справочные материалы

- Справочник языка C++: [cppreference - C++ language](https://en.cppreference.com/cpp/language)
- `std::string`: [cppreference - std::string](https://en.cppreference.com/cpp/string/basic_string)
- Файловые потоки: [cppreference - std::basic_fstream](https://en.cppreference.com/cpp/io/basic_fstream)
- Стадии трансляции: [cppreference - Phases of translation](https://en.cppreference.com/cpp/language/translation_phases)
- Объявления, определения и ODR: [cppreference - Definitions and ODR](https://en.cppreference.com/cpp/language/definition)
- Параметры GCC: [GCC - Option Summary](https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html)
- Стадии сборки Clang: [Clang Command Guide](https://clang.llvm.org/docs/CommandGuide/clang.html)
