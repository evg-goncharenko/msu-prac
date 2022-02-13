### Task 3

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

<b> [task3.py](./task3.py): </b><br>

You need to write a function `longestCommonPrefix(x)`, which takes a list of strings and returns
the largest common prefix for strings in the list of strings. Whitespace characters at the beginning of
the line are not taken into account. You cannot change the input list. If there is no such prefix, you need
to return an empty string. <br><br>

Example of work:
```python
print(longestCommonPrefix(["flo","flow","flight"])) # fl
print(longestCommonPrefix(["dog","racecar","car"])) #
print(longestCommonPrefix([" flo","flow","fl"])) # fl
```
<br>

Running tests:
```bash
python run.py unittest task3
```