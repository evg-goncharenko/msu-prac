### Task 6

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

<b> [task6.py](./task6.py):</b><br>

When a student read a textbook on linear algebra and analytic geometry, he wondered how many words and in
what quantity are used in this book.<br>
It is required to write a function `check(s, filename)`, which takes as input a string - a sequence of words
separated by a space and the file name; the function should output to the file for each unique word in this
line the number of its repetitions (case-insensitive) in the format "word quantity" (see the output example).<br>
Words should be output alphabetically, each unique word should be output only once.<br><br>

Example of work:
```python
check("a aa abC aa ac abc bcd a", "file.txt")
"""
a2
aa 2
abc 2
ac 1
bcd 1
"""
```
<br>

Running tests:
```bash
python run.py python_intro_public_test
```