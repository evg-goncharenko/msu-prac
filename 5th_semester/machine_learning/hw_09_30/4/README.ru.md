### Задача 4

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

<b> [task4.py](./task4.py): </b><br>

У студента есть кредитная карта банка, на который лежат деньги в долларах. Требуется реализовать
класс `BankCard` со следующим интерфейсом: <br><br>

`a = BankCard(total_sum)` <br>
total_sum - общая сумма денег на карте у студента в начальный момент времени <br>

<br> `a(sum_spent)` <br>
sum_spent - сумма, которую студент хочет потратить; <br>
при вызове данной функции sum_spent вычитается из текущей total_sum; <br>
если такой суммы на карте нет, требуется сгенерировать исключение ValueError и напечатать
Not enough money to spent sum_spent dollars. Если попытка снятия денег была успешной, следует написать: <br>
"You spent sum_spent dollars. total_sum dollars are left." <br>

<br> `print(a)` <br>
При вызове функции print от объекта класса должно выводиться следующее сообщение: <br>
"To learn the balance you should put the money on the card, spent some money or get the bank data.
The last procedure is not free and costs 1 dollar." <br>

<br> `a.balance` <br>
При таком вызове текущий баланс карты должен уменьшаться на 1, а возвращаться должна total_sum уже
без учета доллара; <br>
Если баланс проверить невозможно (недостаточно средств), требуется сгенерировать исключение ValueError
и напечатать "Not enough money to learn the balance." <br>

<br> `a.put(sum_put)` <br>
Положить sum_put долларов на карту; также следует написать: <br>
"You put sum_put dollars. total_sum dollars are left."
<br>

Пример работы:
```python
a = BankCard(10)
print(a.total_sum) # 10
a(5) # You spent 5 dollars. 5 dollars are left.
print(a.total_sum) # 5
print(a)
"""
To learn the balance you should put the money on the card, spent some money or get the bank data.
The last procedure is not free and costs 1 dollar.""" 
print(a.balance) # 4
try:
    a(5) # Not enough money to spent 5 dollars.
except ValueError:
    pass
a(4) # You spent 4 dollars. 0 dollars are left.
try:
    a.balance # Not enough money to learn the balance.
except ValueError:
    pass
a.put(2) # You put 2 dollars. 2 dollars are left.
print(a.total_sum) # 2
print(a.balance) # 1
```
<br>

Запуск тестов:
```bash
python run.py unittest task4
```