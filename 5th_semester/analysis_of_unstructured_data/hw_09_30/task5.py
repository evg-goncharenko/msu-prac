cap_letters = set("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ")
ss = set("абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ.,-?!")
letters = set("абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ-")
punct = set(".!?")

def make_sentence(f):
    end = False
    sent = []
    temp = ""
    for i in f:
        if i == "\n":
            i = " "
        if end and i in cap_letters:
            sent.append(temp)
            end = False
            temp = ""
        if end and i != " " and i not in cap_letters:
            end = False
        if i in punct:
            end = True
        temp += i
    if temp != "":
        sent.append(temp)
    return sent


f = open("text.txt", "r", encoding = "utf-8").read()
p = " ".join(f.split())
words = p.split(" ")
al = []
t = True
tt = False
for i in words:
    for j in i:
        if j in letters:
            tt = True
        if j not in ss:
            t = False
    if t and tt:
        al.append(i)
    else:
        t = True
    tt = False
sss = len(al)

al = " ".join(al)
quant = []
sent = make_sentence(al)
for p, n in enumerate(sent):
    print(p + 1, n)
    quant.append(len(n.rstrip().split(" ")))
print()
print("Общее количество слов = ", sss)
print("Среднее количество слов =", round(sum(quant)/len(quant), 2))

ff = ""
for i in al:
    if (i in letters or i == " "):
        ff += i
sent = ff.split(" ")
quant = []
for p, n in enumerate(sent):
    quant.append(len(n))
print("Среднее количество букв =", round(sum(quant)/len(quant), 2))


