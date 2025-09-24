# Лабораторная 2 - Корреляция сигналов

Смоделировать сигналы по формулам для 100 значений, вычислить автокорреляционную функцию для x и y и взаимную корреляционную функцию.

| Вариант | t      | Сигнал X                                                                                                                                                | Сигнал Y              |
| ------- | ------ | ------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------- |
| 1       | 0...10 | $x = 15\sin\left(\frac{\pi t}{5} + \frac{\pi}{8}\right) + 25\cos\left(\frac{\pi t}{15}\right) + 3\sin\left(\frac{\pi t}{45} + \frac{\pi}{10}\right)$    | $y = 6x^2 + 3x$       |
| 2       | 0...15 | $x = 10\sin\left(\frac{\pi t}{6} + \frac{\pi}{7}\right) + 22\cos\left(\frac{\pi t}{16}\right) + 13\sin\left(\frac{\pi t}{41} + \frac{\pi}{11}\right)$   | $y = 20x - 2x^2$      |
| 3       | 0...20 | $x = 5\sin\left(\frac{\pi t}{7} + \frac{\pi}{11}\right) + 19\cos\left(\frac{\pi t}{17}\right) + 23\sin\left(\frac{\pi t}{39} + \frac{\pi}{12}\right)$   | $y = 2x - x^3$        |
| 4       | 0...25 | $x = 20\sin\left(\frac{\pi t}{8} + \frac{\pi}{3}\right) + 16\cos\left(\frac{\pi t}{19}\right) + 33\sin\left(\frac{\pi t}{37} + \frac{\pi}{14}\right)$   | $y = -4x^3 + 5x + 8$  |
| 5       | 0...30 | $x = 30\sin\left(\frac{\pi t}{9} + \frac{\pi}{15}\right) + 13\cos\left(\frac{\pi t}{11}\right) + 7\sin\left(\frac{\pi t}{33} + \frac{\pi}{15}\right)$   | $y = x^3 - 2x^2$      |
| 6       | 0...35 | $x = 40\sin\left(\frac{\pi t}{11} + \frac{\pi}{19}\right) + 11\cos\left(\frac{\pi t}{13}\right) + 17\sin\left(\frac{\pi t}{57} + \frac{\pi}{16}\right)$ | $y = -2x^2 + 7x$      |
| 7       | 0...40 | $x = 50\sin\left(\frac{\pi t}{12} + \frac{\pi}{21}\right) + 8\cos\left(\frac{\pi t}{23}\right) + 27\sin\left(\frac{\pi t}{67} + \frac{\pi}{17}\right)$  | $y = x^3 + 3x$        |
| 8       | 0...45 | $x = 60\sin\left(\frac{\pi t}{13} + \frac{\pi}{33}\right) + 7\cos\left(\frac{\pi t}{29}\right) + 37\sin\left(\frac{\pi t}{79} + \frac{\pi}{18}\right)$  | $y = (x - 20)^2 + 3x$ |

Требуется:

1. Задать сигналы $x(t)$ и $y(t)$
2. Сгенерировать точки
3. Построить графики сигналов $x$ и $y$
4. Вычислить автокорреляцию для $x$ и $y$
5. Вычислить взаимную корреляцию для $x$ и $y$
6. Построить графики:
   1. Автокорреляции для $x$
   2. Автокорреляции для $y$
   3. Взаимной корреляции
