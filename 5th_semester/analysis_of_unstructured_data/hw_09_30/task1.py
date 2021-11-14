'''
Алгоритм:
- Убираем в предложении знаки препинания
- Составляем список без стоп-слов
- Ставим слова в начальную форму
- Формируем отсортированный словарь

Библиотеки: nltk, re, operator, pymorphy2
- stopwords - множество стоп-слов
- re.sub(pattern, replacement, original_string) - для удаления знаков препинания 
  pattern - знаки препинания или шаблон выражений, которые мы хотим заменить,
  replacement - строка, которая будет заменять шаблон.
- Модуль operator включает в себя функцию itemgetter(). Эта функция возвращает вызываемый объект, 
  который возвращает элемент из объекта. Каждый словарь имеет доступ к методу items(). Эта функция возвращает
  пары "ключ-значение" словаря в виде списка кортежей. Мы можем отсортировать список кортежей, используя 
  функцию itemgetter() для извлечения второго значения кортежа, то есть значения ключей в словаре.
- Нормальную (начальную) форму слова можно получить через атрибутив Parse.normal_form
'''

import nltk
from nltk import word_tokenize
from nltk.util import pr
nltk.download('stopwords')
from nltk.corpus import stopwords
import re
import operator
import pymorphy2
morph = pymorphy2.MorphAnalyzer()

sentence = "Мама мыла белую раму. Белая рама стала еще белее." # исходное предложение
stop = set(stopwords.words('russian')) # создаем множество русских слов
sentence = re.sub(r'[^\w\s]','', sentence) # удаляем знаки препинания
# составляем список из слов в нижнем регистре без стоп-слов:
sentence = [i for i in sentence.lower().split() if i not in stop]
morph_sentence = [morph.parse(word)[0].normal_form for word in sentence] # постановка слов в начальную форму
sentence = morph_sentence
words = {i:sentence.count(i) for i in sentence} # составляем словарь из слов
words = sorted(words.items(), key=operator.itemgetter(0)) # сортируем 
words = {k: v for k, v in words} # формируем обратно словарь
inc = 1
for word in words.items():
  print(f'{inc}. {word[0]} - {word[1]}')
  inc += 1
''' 
Вывод:
1. белый - 3
2. мама - 1
3. мыло - 1
4. рама - 2
5. стать - 1
'''