from collections import Counter
ss = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
ssru = "абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
def count_and_print(p):
    c = Counter(p)
    maxx = max(c.values())
    summ = sum(c.values())
    c = c.most_common()
    for i in c:
        print(i[0], "*" * round(round(i[1]/summ, 3)*10), round(i[1]/summ, 3))
    print()

f = open("text.txt", "r", encoding = "utf-8").read()
p = ''.join(x.lower() for x in f if x in ss or x in ssru)
print("Общее:")
count_and_print(p)


p = ''.join(x.lower() for x in f if x in ss)
if len(p) != 0:
  print("English:")
  count_and_print(p)
 
p = ''.join(x.lower() for x in f if x in ssru)
if len(p) != 0:
  print("Русские:")
  count_and_print(p)
