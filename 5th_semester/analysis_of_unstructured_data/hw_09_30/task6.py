from collections import Counter
cap_letters = set("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ")
valid_characters = set("абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ.,-?!")
cap_vowel_letters = set("АУОИЭЫЯЮЕЁ")
letters = set("абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ-")
vowel_letters = set("ауоиэыяюеё")
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
        if j not in valid_characters:
            t = False
    if t and tt:
        al.append(i)
    else:
        t = True
    tt = False
total_number_of_words = len(al)

al = " ".join(al)
quant = []
sent = make_sentence(al)
number_of_sentence = 0
for p, n in enumerate(sent):
    quant.append(len(n.rstrip().split(" ")))
    number_of_sentence += 1
print("Кол-во предложений:", number_of_sentence)
print("Общее количество слов:", total_number_of_words)

ff = []
for i in al:
    if (i in vowel_letters or i in cap_vowel_letters):
        ff.append(i.lower())

letters = Counter(letter for letter in ff)
summ_vowel_letters = 0
for i in letters.items():
    summ_vowel_letters += i[1]
print("Кол-во слогов:", summ_vowel_letters)
ASL = total_number_of_words / number_of_sentence
ASW = summ_vowel_letters / total_number_of_words
FRE = 206.835 - 1.52 * ASL - 65.14 * ASW
print("Индекс Флеша:", FRE)
