# task11.py
numbers = input("Введите три числа через запятую: ")
a, b, c = map(float, numbers.split(","))

result = int((a + c) // b)
print(f"Результат вычисления: {result}")
