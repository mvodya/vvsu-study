# Примеры для презентации

`digits_012.npz`: 360 изображений рукописных цифр 0, 1 и 2, по 120 на класс. images имеет форму (360,8,8), значения от 0 до 16. labels содержит правильные классы, source_rows - номера строк исходного файла с нуля

Источник: [Optical Recognition of Handwritten Digits, UCI](https://archive.ics.uci.edu/dataset/80/optical+recognition+of+handwritten+digits), E. Alpaydin и C. Kaynak, 1998, [CC BY 4.0](https://creativecommons.org/licenses/by/4.0/). Копия исходного тестового набора распространяется в [scikit-learn](https://scikit-learn.org/stable/modules/generated/sklearn.datasets.load_digits.html), файл [digits.csv.gz](https://github.com/scikit-learn/scikit-learn/blob/main/sklearn/datasets/data/digits.csv.gz)

Взяты первые 120 строк каждого выбранного класса, значения не изменены. В презентации этот учебный поднабор заново разделяется на train, validation и test. Это не официальная оценка на исходном тесте UCI и не проверка переноса на новых авторов почерка

Для Colab файл загружается через Files рядом с ноутбуком. Локально он находится в папке data
