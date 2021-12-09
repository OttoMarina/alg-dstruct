Задание D20: "Сумма размеров"

Условие:
Заданы конечное множество A, размеры s(a) всех элементов A и
положительное целое число B. Существует ли такое подмножество A, что сумма
размеров его элементов равна B?

Исходные данные:
В первой строке текстового файла input.txt записано целое положительное число B, 0 < B < 2^32. 
Далее записано целое неотрицательное число N, 0 < N < 1000 - количество
элементов множества A, и N натуральных чисел ai <= 2^16. Выведите в текстовый файл
output.txt те элементы множества A, сумма которых равна B. Если таких элементов нет,
выведите 0. Если задача имеет не единственное решение, выведите любое из них.

Пример
input.txt     |     output.txt
10                 2 3 5
5
2 3 4 5 9