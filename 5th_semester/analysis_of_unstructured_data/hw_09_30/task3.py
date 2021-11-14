from collections import Counter
ss = "абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ "
def count_and_print(p, numb):
    c = Counter(p)
    maxx = max(c.values())
    summ = sum(c.values())
    c = c.most_common(numb)
    for i in c:
        print(i[0], "*" * round(round(i[1]/summ, 3)*10), round(i[1]/summ, 3))
    print()
f = open("text.txt", "r", encoding = "utf-8").read()
p = " ".join(f.split())
p = " " + p
print("Биграммы:")
bigrams = []
for i in range(0, len(p) - 1):
    if p[i] in ss and p[i+1] in ss:
        bigrams.append(p[i].lower() + p[i + 1].lower())
        print(p[i].lower(), p[i + 1].lower(), sep = "")
print("Частота: ")
count_and_print(bigrams, 5)

print("Триграммы:")
trigrams = []
for i in range(0, len(p) - 2):
    if p[i] in ss and p[i+1] in ss and p[i+2] in ss:
        trigrams.append(p[i].lower() + p[i + 1].lower() + p[i + 2].lower())
        print(p[i].lower(), p[i + 1].lower(), p[i + 2].lower(), sep = "")
print("Частота: ")      
count_and_print(trigrams, 5)
