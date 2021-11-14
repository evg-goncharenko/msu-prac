from collections import Counter
ssru = "абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
def count(p):
    counter = Counter(p)
    maxx = max(counter.values())
    summ = sum(counter.values())
    counter = counter.most_common()
    number = 1
    for i in counter:
        print(number, i[0], "*" * round(round(i[1]/summ, 3)*10), round(i[1]/summ, 3), 'numb:', i[1])
        number += 1
    print()

file = open("text.txt", "r", encoding = "utf-8").read()
p = ''.join(x.lower() for x in file if x in ssru)
count(p)
