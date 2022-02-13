### Task 2

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

<b> [task2.py](./task2.py): </b><br>

It is required to write the function `is_palindrome(x)`, which takes a positive integer and check whether it
is a palindrome, i.e. whether the first digit matches the last, the second — with the penultimate, etc.
You cannot represent a number as a sequence (string, list, etc.). Output `YES` or `NO` respectively.
Leading zeros are not taken into account (numbers ending with 0, except for the number 0, are not automatically
palindromes). <br><br>

Example of work:
```python
print(is_palindrome(121)) # YES
print(is_palindrome(120)) # NO
print(is_palindrome(10)) # NO
```
<br>

Running tests:
```bash
python run.py unittest task2
```