### Task 1

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

<b> [task1.py](./task1.py): </b><br>

Write a function `hello(x)`, which takes 1 parameter, which is equal to None by default. <br>
If an empty string is passed as this parameter or a call occurs without arguments, the function returns
the string "Hello!", otherwise, the function outputs "Hello, x!", where x is the value of
the function parameter. <br><br>

Example of work:
```python
print(hello()) # Hello!
print(hello('')) # Hello!
print(hello('Masha')) # Hello, Masha!
```
<br>

Running tests:
```bash
python run.py unittest task1
```