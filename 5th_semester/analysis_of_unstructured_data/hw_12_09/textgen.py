# Автор: Логинов Борис, 425 группа, ВМК МГУ
# Используемые библиотеки: Только random для генерации псевдо-случайных чисел
# Остальное реализовано вручную, в том числе и деление на предложения
# Для получения более полной информации о программе - см. файл report.pdf

'''НАЧАЛО БЛОКА ЗАДАВАЕМЫХ ПОЛЬЗОВАТЕЛЕМ ПАРАМЕТРОВ'''
# Если задано "undef", то такие параметры будут загружаться из файла

# settingsFileName - имя файла, откуда загружаются параметры (перечисл. ниже)
# Если параметры не нужно загружать из файла, то settingsFileName = "undef"
settingsFileName = "settings.txt"

# filterFuncName - имя функции фильтрации
# Можно использовать только функции фильтрации, которые есть в программе
filterFuncName = "undef"

# trainFileName - имя файла с текстом, по которому обучается программа
trainFileName = "undef"

# resultFileName - имя файла, куда сохраняем результат работы программы
# Если результат в файл сохранять не нужно, то resultFileName = "undef"
# Если resultFileName нужно считать из файла, то resultFileName = "undef"
resultFileName = "undef"

# previousCnt - длина цепочки (количество предыдущих слов, учитываемых при
# построении марковской цепи)
previousCnt = "undef"

# sentanceCnt - количество предложений, которое нужно сгенерировать
sentanceCnt = "undef"

# maxSentanceLen - максимальная длинна предложения (в словах)
maxSentanceLen = "undef"

# minSentanceLen - минимальная длинна предложения (в словах)
minSentanceLen = "undef"
'''КОНЕЦ БЛОКА ЗАДАВАЕМЫХ ПОЛЬЗОВАТЕЛЕМ ПАРАМЕТРОВ'''

# Импортируем ГПСЧ
import random

# Функция для загрузки параметров
def loadParams(filename):
    global filterFuncName
    global trainFileName
    global resultFileName    
    global previousCnt
    global sentanceCnt
    global maxSentanceLen
    global minSentanceLen
    
    tempFile = open(filename, "r", encoding = "utf-8")
    data = ""
    for line in tempFile:
        for c in line:
            data += c
        data += "\n"            
    tempFile.close()    
    
    data = "".join(data.split("\r"))
    data = "".join(data.split(" "))
    data = data.split("\n")
    params = []
    for x in data:
        temp = x.split("=")
        if len(temp) == 2:
            params.append(temp)
    for x in params:
        if x[0] == "filterFuncName" and filterFuncName == "undef":
            filterFuncName = x[1]
        elif x[0] == "trainFileName" and trainFileName == "undef":
            trainFileName = x[1]
        elif x[0] == "resultFileName" and resultFileName == "undef":
            resultFileName = x[1]            
        elif x[0] == "previousCnt" and previousCnt == "undef":
            if (not (x[1].isnumeric())):
                previousCnt = 1
            elif x[1].isnumeric() and int(x[1]) <= 0:
                previousCnt = 1
            elif x[1].isnumeric():
                previousCnt = int(x[1])
        elif x[0] == "sentanceCnt" and sentanceCnt == "undef":
            if (not (x[1].isnumeric())):
                sentanceCnt = 1
            elif x[1].isnumeric() and int(x[1]) <= 0:
                sentanceCnt = 1
            elif x[1].isnumeric():
                sentanceCnt = int(x[1])
        elif x[0] == "maxSentanceLen" and maxSentanceLen == "undef":
            if (not (x[1].isnumeric())):
                maxSentanceLen = 1
            elif x[1].isnumeric() and int(x[1]) <= 0:
                maxSentanceLen = 1
            elif x[1].isnumeric():
                maxSentanceLen = int(x[1])
        elif x[0] == "minSentanceLen" and minSentanceLen == "undef":
            if (not (x[1].isnumeric())):
                minSentanceLen = 1
            elif x[1].isnumeric() and int(x[1]) <= 0:
                minSentanceLen = 1
            elif x[1].isnumeric():
                minSentanceLen = int(x[1])
    return

# Проверка является ли символ русской буквой
def RU(s):
    tmp = s.lower()
    if (tmp >= 'а' and tmp <= 'я') or tmp == 'ё':
        return True
    return False

# Отсутствие фильтрации текста
def noFilter(sentance):
    return sentance

# Простая фильтрация текста
def basicFilter(sentance):
    s_filtered = ""
    for x in sentance:
        if x.isalpha():
            s_filtered += x
        elif x.isspace():
            if len(s_filtered) > 0 and s_filtered[-1] != " ":
                s_filtered += " "
    return s_filtered

# Простая фильтрация текста, оставляем только русские буквы
def basicFilterRU(sentance):
    s_filtered = ""
    for x in sentance:
        if RU(x):
            s_filtered += x.lower()
        elif x.isspace():
            if len(s_filtered) > 0 and s_filtered[-1] != " ":
                s_filtered += " "
    return s_filtered

# Улучшенная фильтрация текста, генерируется более читаемый текст
# Оставляются знаки препинания и дефисы в словах
# Знаки препинания учавствуют в словах, чтобы получался более читаемый текст
def extendedFilterRU(sentance):
    s_filtered = ""
    lastRu = False
    addIfRu = ""
    prevChar = ""
    for x in sentance:
        if len(addIfRu) > 0 and RU(x):
            s_filtered += addIfRu
            addIfRu = ""
        
        if RU(x):
            s_filtered += x.lower()
            lastRu = True
        elif x == "," or x == "!" or x == "?" or x == "…":
            if lastRu:
                s_filtered += x
            lastRu = False
        elif x == "-":
            if len(prevChar) > 0 and RU(prevChar):
                addIfRu = "-"
        elif x.isspace():
            if len(s_filtered) > 0 and s_filtered[-1] != " ":
                s_filtered += " "
            lastRu = False
        
        prevChar = x
    return s_filtered

# Функция для разбиения текста на предложения
def getSentances(filename, filterFunc = noFilter):
    file = open(filename, "r", encoding = "utf-8")

    s = ""
    sentances = []

    strongEndFlag = 0
    weakEndFlag = 0

    for line in file:
        for c in line:
            if c == '?' or c == '!' or c == '…':
                strongEndFlag = 1
            elif c == '.' and weakEndFlag == 0:
                weakEndFlag = 1
            elif c == '.' and weakEndFlag == 1:
                strongEndFlag = 1
            elif (strongEndFlag and c.isalnum()) or (weakEndFlag and c.isupper()):
                anySymFlag = False
                for x in s:
                    if x.isalnum():
                        anySymFlag = True
                if anySymFlag:
                    sentances.append(filterFunc(s))
                
                s = ""
                strongEndFlag = 0
                weakEndFlag = 0
            elif weakEndFlag and c.islower():
                weakEndFlag = 0
        
            if c != '\n' and c != '\r':
                s += c
            elif c == '\n':
                s += ' '
    
    anySymFlag = False
    for x in s:
        if x.isalnum():
            anySymFlag = True
    if anySymFlag:
        sentances.append(filterFunc(s))

    file.close()
    return sentances

# Функция разбиения предложения на слова (ключи)
# Дополнительно добавляется ключ-начало предложения
def sentanceToKeys(sentance):
    oneLetterWords = "аявсикоу"
    data = sentance.split(" ")
    keys = []
    keys.append("{ss}")
    for x in data:
        if len(x) > 0:
            if len(x) == 1 and (not (x in oneLetterWords)):
                continue
            keys.append(x)
    return keys

# Функция построения марковской цепи
# Параметр prevCnt - сколько предыдущих слов учитываем
# Параметр keys - все ключи из текста, включая начала предложений
def buildMarkovChain(keys, prevCnt = 1):
    startTuples = dict()
    result = dict()
    l = len(keys)
    
    for i in range(l - prevCnt):
        tmpTuple = tuple(keys[i : i + prevCnt])
        if keys[i] == "{ss}":
            if tmpTuple in startTuples:
                startTuples[tmpTuple] += 1
            else:
                startTuples[tmpTuple] = 1
        tmpKey = keys[i + prevCnt]
        if tmpTuple in result:
            if tmpKey in result[tmpTuple]:
                result[tmpTuple][tmpKey] += 1
            else:
                result[tmpTuple][tmpKey] = 1
        else:
            result[tmpTuple] = dict()
            result[tmpTuple][tmpKey] = 1
    
    return (result, startTuples)

# Функция получения случайного слова из данного словаря
# Учитывается "вес" каждого слова
def getRandomKey(anyDict):
    s = sum(anyDict.values())
    r = random.randint(0, s - 1)
    for x in anyDict.keys():
        r -= anyDict[x]
        if r < 0:
            return x
    return "ERROR"

# Делаем словарь всех фукнций фильтрации, которые есть в программе
filterFuncDict = dict()
filterFuncDict["noFilter"] = noFilter
filterFuncDict["basicFilter"] = basicFilter
filterFuncDict["basicFilterRU"] = basicFilterRU
filterFuncDict["extendedFilterRU"] = extendedFilterRU

# Если задан файл для загрузки параметров, то загружаем оттуда параметры
if settingsFileName != "undef" and len(settingsFileName) > 0:
    loadParams(settingsFileName)

# Проверяем, что все параметры загрузились корректно
# Если это не так, выдаем сообщение об ошибке и завершаем программу
if (not (filterFuncName in filterFuncDict)):
    print("Задана функция фильтрации, которой нет в программе.")
    exit(0)
if trainFileName == "undef":
    print("Не задан параметр trainFileName.")
    exit(0)
if previousCnt == "undef" or type(previousCnt) != int:
    print("Не задан параметр previousCnt.")
    exit(0)
if sentanceCnt == "undef" or type(sentanceCnt) != int:
    print("Не задан параметр sentanceCnt.")
    exit(0)
if maxSentanceLen == "undef" or type(maxSentanceLen) != int:
    print("Не задан параметр maxSentanceLen.")
    exit(0)
if minSentanceLen == "undef" or type(minSentanceLen) != int:
    print("Не задан параметр minSentanceLen.")
    exit(0)
if minSentanceLen > maxSentanceLen:
    print("Параметр minSentanceLen не может превосходить maxSentanceLen.")
    exit(0)


# Разбиваем текст на предложения
sentances = getSentances(trainFileName, filterFuncDict[filterFuncName])

# Выделяем все ключи из текста в список allkeys
allkeys = []

for x in sentances:
    tmp = sentanceToKeys(x)
    if len(tmp) > 1:
        for y in tmp:
            allkeys.append(y)
allkeys.append("{ss}")

# Строим марковскую цепь
data, start = buildMarkovChain(allkeys, prevCnt = previousCnt)

# Генерируем предложения
s = ""
generated = []
generatedCnt = 0
while generatedCnt < sentanceCnt:
    wordCnt = 0
    startFrom = getRandomKey(start)
    for x in startFrom:
        if x != "{ss}":
            s += " " + x
            wordCnt += 1
    curr = startFrom
    while True:
        tmpWord = getRandomKey(data[curr])
        while wordCnt < minSentanceLen and tmpWord == "{ss}" and len(data[curr]) > 1:
            tmpWord = getRandomKey(data[curr])
        if wordCnt >= maxSentanceLen and ("{ss}" in data[curr]):
            tmpWord = "{ss}"
        if tmpWord != "{ss}":
            s += " " + tmpWord
            wordCnt += 1
            tmpList = list(curr[1:])
            tmpList.append(tmpWord)
            curr = tuple(tmpList)
        else:
            if s[-1] == ",":
                s = s[:-1]
            if not (s[-1] == "…" or s[-1] == "?" or s[-1] == "!" or s[-1] == "."):
                s += "."
            if wordCnt >= minSentanceLen and wordCnt <= maxSentanceLen:
                generated.append(s)
                generatedCnt += 1
            s = ""
            break

# Выводим результат на экран
print("Сгенерированные предложения:")
cntGen = 1
for x in generated:
    print(cntGen, ": ", x[1:], sep = "")
    cntGen += 1

# Записываем результат в файл
if resultFileName != "undef":
    resultFile = open(resultFileName, "w", encoding = "utf-8")
    
    resultFile.write("Сгенерированные предложения:\n")
    cntGenFile = 1
    for x in generated:
        resultFile.write(str(cntGenFile) + ": " + x[1:] + "\n")
        cntGenFile += 1
    
    resultFile.close()
