# Ассоциативные и hash-контейнеры

- [Ключ и значение](#ключ-и-значение)
- [std::set](#stdset)
- [std::map](#stdmap)
- [std::pair и structured bindings](#stdpair-и-structured-bindings)
- [Comparator и порядок ключей](#comparator-и-порядок-ключей)
  - [Составной ключ](#составной-ключ)
  - [Поиск диапазона](#поиск-диапазона)
- [Hash-контейнеры](#hash-контейнеры)
  - [Hash пользовательского ключа](#hash-пользовательского-ключа)
  - [Buckets, load factor и reserve](#buckets-load-factor-и-reserve)
- [Итераторы и удаление](#итераторы-и-удаление)
- [Топ по значению](#топ-по-значению)
- [Выбор контейнера](#выбор-контейнера)
- [Справочные материалы](#справочные-материалы)

## Ключ и значение

Ассоциативный контейнер находит элемент по ключу. Ключом может быть число, строка или пользовательский тип

| Контейнер                    | Содержимое                            | Порядок обхода                               | Header            |
| ---------------------------- | ------------------------------------- | -------------------------------------------- | ----------------- |
| `std::set<Key>`              | Уникальные ключи                      | По comparator                                | `<set>`           |
| `std::map<Key, T>`           | Уникальные ключи и связанные значения | По comparator                                | `<map>`           |
| `std::unordered_set<Key>`    | Уникальные ключи                      | Зависит от реализации и состояния контейнера | `<unordered_set>` |
| `std::unordered_map<Key, T>` | Уникальные ключи и связанные значения | Зависит от реализации и состояния контейнера | `<unordered_map>` |

У `set` сам элемент служит ключом. У `map` ключ и связанное значение имеют отдельные типы. Например, название цвета может быть ключом, а число использований - значением

Варианты `multiset` и `multimap` допускают несколько элементов с эквивалентными ключами. Для учета повторений через счетчик подходит `map<Key, int>`

## std::set

`set` хранит по одному экземпляру каждого ключа. По умолчанию обход идет по возрастанию через `std::less<Key>`

```cpp
std::set<int> levels{8, 2, 8, 5};
levels.insert(3);
levels.erase(5);
for (int level : levels) std::cout << level << ' ';
```

Результат: `2 3 8`. `insert` добавляет ключ, `erase(key)` удаляет совпадение и возвращает число удаленных элементов: ноль или один

```cpp
auto [position, inserted] = levels.insert(3);
std::cout << *position << ' ' << inserted << '\n';
std::cout << levels.contains(8) << '\n';
```

`insert` возвращает пару из итератора и признака вставки. При повторном ключе итератор указывает на существующий элемент, `inserted` равен `false`. `contains` появился в C++20 и возвращает `bool`

Ключ доступен через итератор только для чтения. Его изменение требует удаления старого элемента и вставки нового: положение элемента связано со значением ключа

## std::map

`map` хранит пары ключ-значение. Индекс в `operator[]` обозначает ключ

```cpp
std::map<std::string, int> counts;
counts["blue"]++;
counts["red"] += 2;
counts["blue"]++;
std::cout << counts.at("blue") << '\n';
```

Первое обращение `counts["blue"]` вставляет ключ и создает `int` со значением `0`. Следующие обращения возвращают ссылку на тот же счетчик

| Операция                              | При существующем ключе | При отсутствующем ключе                           |
| ------------------------------------- | ---------------------- | ------------------------------------------------- |
| `values[key]`                         | Ссылка на значение     | Вставка значения через инициализацию по умолчанию |
| `values.at(key)`                      | Ссылка на значение     | `std::out_of_range`                               |
| `values.find(key)`                    | Итератор на пару       | `values.end()`                                    |
| `values.contains(key)`                | `true`                 | `false`                                           |
| `values.try_emplace(key, args...)`    | Сохранение значения    | Создание значения из аргументов                   |
| `values.insert_or_assign(key, value)` | Присваивание значения  | Вставка пары                                      |

`operator[]` может менять размер контейнера, поэтому доступен для изменяемого `map`. Для чтения подходят `find` и `at`, для проверки присутствия - `contains`

```cpp
auto found = counts.find("green");
if (found != counts.end()) {
  std::cout << found->second << '\n';
}
std::cout << counts.size() << '\n';
```

После поиска размер остается равен двум. `find` контейнера использует его структуру поиска. Обычный `std::find` последовательно просматривает диапазон

```cpp
std::map<int, std::string> labels;
labels.try_emplace(7, "draft");
labels.try_emplace(7, "copy");
labels.insert_or_assign(7, "final");
std::cout << labels.at(7) << '\n';
```

`insert` и `emplace` также сохраняют существующее значение при совпадении ключа. `try_emplace` создает связанный объект только при вставке. Вычисление переданных выражений происходит до вызова функции

## std::pair и structured bindings

`std::pair<First, Second>` из `<utility>` объединяет два значения. Поля называются `first` и `second`

```cpp
std::pair<std::string, int> item{"blue", 4};
auto [name, count] = item;
count++;
std::cout << name << ' ' << count << ' ' << item.second << '\n';
```

Здесь structured binding создает локальную копию пары и дает имена ее частям. Результат: `blue 5 4`

Элемент `map<Key, T>` имеет тип `std::pair<const Key, T>`. Ключ сохраняется при изменении связанного значения

```cpp
for (auto &[name, count] : counts) count++;
for (const auto &[name, count] : counts) {
  std::cout << name << ' ' << count << '\n';
}
```

`auto &` связывает имена с частями исходного элемента. `const auto &` дает доступ для чтения без копирования пары

## Comparator и порядок ключей

Comparator - вызываемый объект, который отвечает, расположен ли первый ключ перед вторым. `set` и `map` обычно реализованы через сбалансированное дерево. Стандарт задает поведение и сложность операций

```cpp
std::set<int, std::greater<int>> priorities{2, 8, 5};
for (int priority : priorities) std::cout << priority << ' ';
```

`std::greater<int>` из `<functional>` задает убывание: `8 5 2`

Comparator должен задавать strict weak ordering - строгий слабый порядок:

- `comp(a, a)` равен `false`
- При `comp(a, b) == true` обратное сравнение дает `false`
- Из `comp(a, b)` и `comp(b, c)` следует `comp(a, c)`
- Эквивалентность ключей транзитивна

Два ключа эквивалентны, когда оба сравнения `comp(a, b)` и `comp(b, a)` дают `false`. Именно эта эквивалентность определяет уникальность в ordered containers. Сравнение через `<=` нарушает первое требование

Строки со стандартным comparator сравниваются лексикографически. Например, строка `"10"` предшествует строке `"2"`. Числовой порядок требует числовых ключей или соответствующего comparator

### Составной ключ

Составной ключ объединяет несколько полей. Сравнение по очереди учитывает каждое значимое поле

```cpp
struct Cell {
  int row;
  int column;
  bool operator==(const Cell &) const = default;
  bool operator<(const Cell &other) const {
    return std::tie(row, column) < std::tie(other.row, other.column);
  }
};
std::map<Cell, std::string> marks;
marks[{2, 1}] = "blue";
marks[{1, 3}] = "red";
```

`std::tie` из `<tuple>` создает кортеж ссылок. Сравнение кортежей лексикографическое: сначала `row`, затем `column` при равенстве строк. Обход даст клетки `{1, 3}`, `{2, 1}`

При сравнении только `row` все клетки одной строки стали бы эквивалентными ключами. Набор полей comparator определяет смысл уникальности

### Поиск диапазона

`lower_bound(key)` возвращает первый элемент с ключом, который расположен на позиции `key` или после нее по comparator. `upper_bound(key)` возвращает первый элемент строго после ключа. При выходе за диапазон обе операции возвращают `end()`

```cpp
std::map<int, std::string> pages{{10, "A"}, {20, "B"}, {30, "C"}};
auto last = pages.upper_bound(20);
for (auto it = pages.lower_bound(10); it != last; it++) {
  std::cout << it->first << ' ' << it->second << '\n';
}
```

Пример выводит ключи из интервала `[10, 20]`. Начальный поиск занимает `O(log n)`, обход найденных `k` элементов - `O(k)`

## Hash-контейнеры

Hash-функция преобразует ключ в число типа `std::size_t`. Контейнер использует его для выбора bucket - группы элементов, среди которых выполняется поиск по равенству ключей

```text
ключ -> hash -> bucket -> сравнение ключей -> элемент
```

Коллизия возникает при совпадении hash разных ключей. Контейнер различает такие ключи через проверку равенства. Несколько hash также могут вести в один bucket

```cpp
std::unordered_set<int> seen;
for (int value : {7, 3, 7, 9}) {
  if (seen.insert(value).second) std::cout << value << ' ';
}
```

Пример выводит `7 3 9`: результат определяется порядком входных значений. Сам обход `seen` имеет порядок, зависящий от реализации и состояния контейнера

```cpp
std::unordered_map<std::string, int> counts;
for (const auto &word : {"red", "blue", "red"}) counts[word]++;
for (const auto &[word, count] : counts) {
  std::cout << word << ' ' << count << '\n';
}
```

`operator[]`, `at`, `find`, `contains`, `try_emplace` и `insert_or_assign` имеют тот же смысл, что у `map`. Для стандартных чисел и строк доступен `std::hash<Key>` из `<functional>`

### Hash пользовательского ключа

Для ключа `Cell` зададим отдельный вызываемый объект:

```cpp
struct CellHash {
  std::size_t operator()(const Cell &cell) const {
    auto row = std::hash<int>{}(cell.row);
    auto column = std::hash<int>{}(cell.column);
    return row ^ (column + 0x9e3779b9u + (row << 6) + (row >> 2));
  }
};
std::unordered_map<Cell, int, CellHash> visits;
visits[{2, 3}]++;
visits[{2, 3}]++;
std::cout << visits.at({2, 3}) << '\n';
```

`operator()` позволяет вызвать объект как функцию. Формула смешивает hash обоих полей с учетом их позиций. Операции выполняются с беззнаковым `std::size_t`

Для равных ключей hash обязан совпадать. Равенство здесь задает `operator==`, который сравнивает оба поля. Для разных ключей совпадение hash допустимо, частые коллизии увеличивают время поиска

Hash и проверка равенства должны сохранять результат для ключа во время его хранения. При собственной проверке равенства, например без учета регистра, hash должен учитывать ту же эквивалентность. Значения `std::hash` предназначены для работы внутри запуска программы

### Buckets, load factor и reserve

`bucket_count()` возвращает число buckets. `load_factor()` равен отношению числа элементов к числу buckets. `max_load_factor()` задает порог заполнения, используемый при росте

```cpp
std::unordered_map<int, int> samples;
samples.max_load_factor(0.75f);
samples.reserve(100);
for (int id = 0; id < 100; id++) samples[id] = id * 2;
std::cout << samples.size() << ' ' << samples.bucket_count() << '\n';
```

`reserve(n)` подготавливает buckets для `n` элементов с учетом текущего `max_load_factor`. Число элементов сохраняется. `rehash(n)` запрашивает как минимум `n` buckets с учетом текущего размера и порога заполнения

Подготовка buckets сокращает число перестроений при массовой вставке. Распределение ключей по buckets также зависит от качества hash

## Итераторы и удаление

| Операция            | `set`, `map`                                            | `unordered_set`, `unordered_map`                                                 |
| ------------------- | ------------------------------------------------------- | -------------------------------------------------------------------------------- |
| Вставка             | Существующие итераторы и ссылки сохраняются             | Итераторы сохраняются при сохранении buckets, rehash делает их недействительными |
| `erase`             | Недействительны итераторы и ссылки на удаленный элемент | Недействительны итераторы и ссылки на удаленный элемент                          |
| `clear`             | Все элементы удалены                                    | Все элементы удалены                                                             |
| `reserve`, `rehash` | Отсутствуют                                             | После вызова получать итераторы заново                                           |

При rehash ссылки и указатели на существующие элементы сохраняются. Итератор связан с обходом контейнера и после перестроения требует обновления

```cpp
std::unordered_map<int, std::string> names{{7, "sensor"}};
const auto &name = names.at(7);
names.rehash(names.bucket_count() * 2);
auto found = names.find(7);
std::cout << name << ' ' << found->second << '\n';
```

Удаление при обходе использует итератор, возвращаемый `erase`:

```cpp
std::map<std::string, int> stock{{"blue", 0}, {"red", 4}};
for (auto it = stock.begin(); it != stock.end();) {
  if (it->second == 0) it = stock.erase(it);
  else it++;
}
```

Итераторы ordered containers поддерживают шаг вперед и назад, итераторы hash-контейнеров - шаг вперед. Доступ по числовой позиции требует последовательного перемещения

## Топ по значению

`map` упорядочен по ключу. Для рейтинга по счетчику можно скопировать пары в `vector` и отсортировать их через `std::sort` из `<algorithm>`

```cpp
using Score = std::pair<std::string, int>;
bool higherScore(const Score &left, const Score &right) {
  if (left.second != right.second) return left.second > right.second;
  return left.first < right.first;
}
```

`higherScore` задает убывание счетчика. При равенстве счетчиков ключ задает воспроизводимый порядок

```cpp
std::unordered_map<std::string, int> scores{{"blue", 4}, {"red", 7}, {"green", 7}};
std::vector<Score> rating{scores.begin(), scores.end()};
std::sort(rating.begin(), rating.end(), higherScore);
if (rating.size() > 2) rating.resize(2);
for (const auto &[name, score] : rating) {
  std::cout << name << ' ' << score << '\n';
}
```

Результат: `green 7`, `red 7`. Копирование занимает `O(n)`, сортировка - `O(n log n)`. Вектор содержит изменяемые ключи в обычных `pair`, поэтому алгоритм может переставлять элементы

## Выбор контейнера

| Задача                                   | Контейнер       |
| ---------------------------------------- | --------------- |
| Уникальные ключи с упорядоченным обходом | `set`           |
| Уникальные ключи с проверкой присутствия | `unordered_set` |
| Значения по ключу и поиск интервала      | `map`           |
| Счетчики и точный поиск по ключу         | `unordered_map` |

Поиск, обычная вставка и удаление по ключу в ordered containers занимают `O(log n)`. В hash-контейнерах эти операции занимают в среднем `O(1)`, в худшем случае - `O(n)`. Полный обход занимает `O(n)`

Эти оценки описывают работу контейнера. Сравнение длинных строк и вычисление hash дополнительно зависят от длины ключа. Память hash-контейнера включает таблицу buckets

## Справочные материалы

- [Ordered associative containers](https://eel.is/c++draft/associative.reqmts)
- [Unordered associative containers](https://eel.is/c++draft/unord.req)
- [std::map](https://en.cppreference.com/w/cpp/container/map)
- [std::unordered_map](https://en.cppreference.com/w/cpp/container/unordered_map)
- [std::sort](https://en.cppreference.com/w/cpp/algorithm/sort)
