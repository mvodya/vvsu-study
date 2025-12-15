# Лабораторная работа 7

## Основные задания

### Задание 1

Имеется список объектов Фонда с указанием уровня угрозы:

```python
objects = [
    ("Containment Cell A", 4),
    ("Archive Vault", 1),
    ("Bio Lab Sector", 3),
    ("Observation Wing", 2)
]
```

Используя `sorted` и лямбда-выражение, отсортируйте объекты по возрастанию уровня угрозы

### Задание 2

Дан список сотрудников Фонда с количеством проведенных смен и стоимостью одной смены:

```python
staff_shifts = [
    {"name": "Dr. Shaw", "shift_cost": 120, "shifts": 15},
    {"name": "Agent Torres", "shift_cost": 90, "shifts": 22},
    {"name": "Researcher Hall", "shift_cost": 150, "shifts": 10}
]
```

Используя `map` и лямбда-выражение, создайте список общей стоимости работы каждого сотрудника

Затем найдите максимальную стоимость с помощью `max`

### Задание 3

Дан список персонала с уровнем допуска:

```python
personnel = [
    {"name": "Dr. Klein", "clearance": 2},
    {"name": "Agent Brooks", "clearance": 4},
    {"name": "Technician Reed", "clearance": 1}
]
```

Используя `map` и лямбда-выражение, создайте новый список, где каждому сотруднику добавляется категория допуска:

* `"Restricted"` - уровень 1
* `"Confidential"` - уровни 2–3
* `"Top Secret"` - уровень 4 и выше

Результат должен быть списком словарей

### Задание 4

Дан список зон Фонда с указанием времени активности (в часах):

```python
zones = [
    {"zone": "Sector-12", "active_from": 8, "active_to": 18},
    {"zone": "Deep Storage", "active_from": 0, "active_to": 24},
    {"zone": "Research Wing", "active_from": 9, "active_to": 17}
]
```

Используя `filter` и лямбда-выражение, выберите зоны, которые полностью работают в дневной период (с 8 до 18 включительно)

### Задание 5

Фонд анализирует служебные отчеты. Некоторые отчеты содержат внешние ссылки, которые должны быть удалены перед архивированием

```python
reports = [
    {"author": "Dr. Moss", "text": "Analysis completed. Reference: http://external-archive.net"},
    {"author": "Agent Lee", "text": "Incident resolved without escalation."},
    {"author": "Dr. Patel", "text": "Supplementary data available at https://secure-research.org"},
    {"author": "Supervisor Kane", "text": "No anomalies detected during inspection."},
    {"author": "Researcher Bloom", "text": "Extended observations uploaded to http://research-notes.lab"},
    {"author": "Agent Novak", "text": "Perimeter secured. No external interference observed."},
    {"author": "Dr. Hargreeve", "text": "Full containment log stored at https://internal-db.scp"},
    {"author": "Technician Moore", "text": "Routine maintenance completed successfully."},
    {"author": "Dr. Alvarez", "text": "Cross-reference materials: http://crosslink.foundation"},
    {"author": "Security Officer Tan", "text": "Shift completed without incidents."},
    {"author": "Analyst Wright", "text": "Statistical model published at https://analysis-hub.org"},
    {"author": "Dr. Kowalski", "text": "Behavioral deviations documented internally."},
    {"author": "Agent Fischer", "text": "Additional footage archived: http://video-storage.sec"},
    {"author": "Senior Researcher Hall", "text": "All test results verified and approved."},
    {"author": "Operations Lead Grant", "text": "Emergency protocol draft shared via https://ops-share.scp"}
]
```

Используя `filter` и лямбда-выражение:

1. Отберите отчеты, содержащие ссылки (`http` или `https`)
2. Преобразуйте их так, чтобы вместо ссылки отображалось `[ДАННЫЕ УДАЛЕНЫ]`

### Задание 6

Дан список SCP-объектов с указанием их класса содержания:

```python
scp_objects = [
    {"scp": "SCP-096", "class": "Euclid"},
    {"scp": "SCP-173", "class": "Euclid"},
    {"scp": "SCP-055", "class": "Keter"},
    {"scp": "SCP-999", "class": "Safe"},
    {"scp": "SCP-3001", "class": "Keter"}
]
```

Используя `filter` и лямбда-выражение, сформируйте список SCP-объектов, которые требуют усиленных мер содержания

> ⚠️ К объектам с усиленными мерами относятся все SCP, **класс которых не равен `"Safe"`**

Результат должен быть списком словарей исходного формата

### Задание 7

Дан список инцидентов с количеством задействованного персонала:

```python
incidents = [
    {"id": 101, "staff": 4},
    {"id": 102, "staff": 12},
    {"id": 103, "staff": 7},
    {"id": 104, "staff": 20}
]
```

Используя `sorted` и лямбда-выражение:

1. Отсортируйте инциденты по количеству персонала
2. Оставьте только три наиболее ресурсоемких инцидента

### Задание 8

Дан список протоколов безопасности и их уровней критичности:

```python
protocols = [
    ("Lockdown", 5),
    ("Evacuation", 4),
    ("Data Wipe", 3),
    ("Routine Scan", 1)
]
```

Используя `map` и лямбда-выражение, создайте новый список строк вида:

```text
"Protocol Lockdown - Criticality 5"
```

### Задание 9

Имеется список смен охраны с указанием длительности (в часах):

```python
shifts = [6, 12, 8, 24, 10, 4]
```

Используя `filter` и лямбда-выражение, выберите только те смены, которые:

* длятся не менее 8 часов
* не превышают 12 часов

### Задание 10

Дан список сотрудников с результатами психологической оценки (от 0 до 100):

```python
evaluations = [
    {"name": "Agent Cole", "score": 78},
    {"name": "Dr. Weiss", "score": 92},
    {"name": "Technician Moore", "score": 61},
    {"name": "Researcher Lin", "score": 88}
]
```

Используя `max` и лямбда-выражение, определите сотрудника с наивысшей оценкой

Результатом должно быть имя сотрудника и его балл

## Оформление отчета

Отчет оформляется строго по СТО.

Не забудьте добавить страницу "Задание" с копией содержимого этого файла (с правильным оформлением списков и т.д.)

В отчете должно быть объяснено как работает ваша программа (каждое отдельное задание)