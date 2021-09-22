### Задача 3

<b> [task3.py](./task3.py): </b><br>

Требуется написать функцию `longestCommonPrefix(x)`, которая принимает список строк и возвращает наибольший общий префикс для строк в списке строк. Пробельные символы в начале строки не учитывать. Изменять входной список нельзя. Если такого префикса нет, требуется вернуть пустую строку. <br><br>

Пример работы:
```python
print(longestCommonPrefix(["flo","flow","flight"])) # fl
print(longestCommonPrefix(["dog","racecar","car"])) #
print(longestCommonPrefix([" flo","flow","fl"])) # fl
```
<br>

Запуск тестов:
```bash
python run.py unittest task3
```