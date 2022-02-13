### Task 5

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

<b> [task5.py](./task5.py): </b><br>

A positive integer is called prime if it has exactly two different divisors, that is, it is divisible only by
one and by itself. For example, the number 2 is prime because it is divisible only by 1 and 2. The number 4,
for example, is not prime because it has three divisors - 1, 2, 4. Also, the number 1 is not prime. <br>
It is required to implement a generator function `pramac()`, which will generate prime numbers in ascending
order, starting with the number 2. <br><br>

Example of work:
```python
for i in primes(): 
    print(i)
        if i > 3: 
            break
"""
2
3
5
"""
l = itertools.takewhile(lambda x : x <= 17, primes())
print(list(l)) # [2, 3, 5, 7, 11, 13, 17]
```
<br>

Running tests:
```bash
python run.py unittest task5
```