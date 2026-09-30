# Анализ сигналов и изображений (2026-S2)

Материалы практических занятий для магистрантов

## Лабораторные работы

### 1. Преобразование Фурье

- [Задание и варианты](labs/lab1/README.md)
- [Теория](labs/lab1/THEORY.md)
- [Презентация](labs/lab1/lab1_theory_present.ipynb)
- [Шаблон](labs/lab1/lab1.ipynb)

### 2. Корреляция сигналов

- [Задание и варианты](labs/lab2/README.md)
- [Теория](labs/lab2/THEORY.md)
- [Презентация](labs/lab2/lab2_theory_present.ipynb)
- [Шаблон](labs/lab2/lab2.ipynb)

### 3. Цифровая фильтрация

- [Задание и данные](labs/lab3/README.md)
- [Теория](labs/lab3/THEORY.md)
- [Презентация](labs/lab3/lab3_theory_present.ipynb)
- [Шаблон](labs/lab3/lab3.ipynb)

### 4. Обучаемый фильтр

- [Задание](labs/lab4/README.md)
- [Теория](labs/lab4/THEORY.md)
- [Презентация](labs/lab4/lab4_theory_present.ipynb)
- [Шаблон](labs/lab4/lab4.ipynb)

## Среда выполнения

Работы можно выполнять в Google Colab или локально в Jupyter

### Google Colab

Файл `.ipynb` загружается через меню **File > Upload notebook** в [Google Colab](https://colab.research.google.com/). Также поддерживается открытие ноутбуков из GitHub

В работе 3 дополнительно загружается файл своего варианта. В работу 4 переносится код расчета выбранного фильтра из ноутбука работы 3. Для обучения достаточно CPU. Eсли PyTorch отсутствует, он устанавливается ячейкой `%pip install torch`

### Локальный Jupyter

Команды из каталога курса:

```bash
conda env create -f environment.yml
conda activate signal
python -m ipykernel install --sys-prefix --name signal --display-name "Python (signal)"
jupyter lab
```

Для существующего окружения после добавления зависимостей работы 4:

```bash
conda env update -n signal -f environment.yml
conda activate signal
```

Ядро ноутбука - **Python (signal)**

## PLAN

- [*] 24 сентября: лабораторная 1 - сигналы, преобразование фурье
- [*] 24 сентября: лабораторная 2 - корреляция
- [x] 30 сентября: лабораторная 3 - цифровая фильтрация
- [x] 30 сентября: лабораторная 4 - нейросетевая обработка звука
- [ ] 7 октября: лабораторная 5 - обработка изображений и хромакей
- [ ] 7 октября: лабораторная 6 - классификация изображений
- [ ] 14 октября: лабораторная 7 - детектирование объектов
- [ ] 14 октября: лабораторная 8 - классификация звука (предварительно)
