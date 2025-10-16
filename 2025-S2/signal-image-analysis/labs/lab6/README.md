# Лабораторная 6 - Разметка изображений и обучение нейронной сети YOLO для детектирования объектов

## Задания к выполнению

### 1. Подготовка датасета

1. Собрать изображения по выбранной теме (например, разные животные, объекты из видео, или кадры с вами на фото)
2. При необходимости извлечь кадры из видео через OpenCV:

   ```python
   import cv2, os
   cap = cv2.VideoCapture("video.mp4")
   os.makedirs("frames", exist_ok=True)
   i = 0
   while True:
       ret, frame = cap.read()
       if not ret: break
       cv2.imwrite(f"frames/frame_{i:04}.jpg", frame)
       i += 1
   cap.release()
   ```
3. Разделить данные на три подвыборки:

   * `train` - ≈ 70 %
   * `val` - ≈ 20 %
   * `test` - ≈ 10 %

Структура каталога:

```
dataset/
 ├── images/
 │    ├── train/
 │    ├── val/
 │    └── test/
 └── labels/
      ├── train/
      ├── val/
      └── test/
```

### 2. Разметка в Label Studio

1. Установить Label Studio:

   ```bash
   pip install label-studio
   label-studio start
   ```

   После установки откройте [http://localhost:8080](http://localhost:8080) в браузере
2. Создать новый проект, загрузить изображения или папку frames
3. Добавить теги объектов (например, "wolf", "hare")
4. Разметить все изображения прямоугольниками bounding box
5. Экспортировать разметку в формате **YOLO v5/v8 (JSON или TXT)**

### 3. Подготовка к обучению YOLO

1. Создать файл `data.yaml` в корне проекта:

   ```yaml
   path: ./dataset
   train: images/train
   val: images/val
   test: images/test

   names:
     0: wolf
     1: hare
   ```
2. Проверить, что каждое изображение имеет файл разметки с тем же именем (например, `frame_0010.jpg` <-> `frame_0010.txt`)

### 4. Обучение YOLO

1. Установить библиотеку Ultralytics:

   ```bash
   pip install ultralytics
   ```
2. Запустить обучение:

   ```python
   from ultralytics import YOLO
   model = YOLO("yolov8n.pt")  # базовая модель
   model.train(data="data.yaml", epochs=100, imgsz=640)
   ```
3. После обучения появится папка `runs/detect/train*/`, в которой будет файл `best.pt`.

### 5. Проверка и демонстрация

1. Проверить качество обученной модели:

   ```python
   model.val()
   ```
2. Протестировать на изображении или видео:

   ```python
   results = model.predict(source="test_video.mp4", show=True)
   ```
3. Сохранить результаты в видео или кадры с обнаруженными объектами.