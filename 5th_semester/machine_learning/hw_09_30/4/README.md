### Task 4

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

<b> [task4.py](./task4.py): </b><br>

The student has a credit card of the bank, which holds the money in dollars. It is required to implement
the `BankCard` class with the following interface: <br><br>

`a = BankCard(total_sum)` <br>
total_sum - total amount of money on the student's card at the initial time <br>

<br> `a(sum_spent)` <br>
sum_spent - amount the student wants to spend; <br>
when calling this function, sum_spent is subtracted from the current total_sum; <br>
if there is no such amount on the card, you need to generate a ValueError exception and print Not enough money
to spend sum_spent dollars. If the withdrawal attempt was successful, you should write: <br>
"You spent sum_spent dollars. total_sum dollars are left." <br>

<br> `print(a)` <br>
When calling the print function, the following message should be output from the class object: <br>
"To learn the balance you should put the money on the card, spent some money or get the bank data.
The last procedure is not free and costs 1 dollar." <br>

<br> `a.balance` <br>
With such a call, the current balance of the card should decrease by 1, and total_sum should be returned
without taking into account the dollar; <br>
If the balance cannot be checked (there are not enough funds), you need to generate a ValueError exception
and print "Not enough money to learn the balance." <br>

<br> `a.put(sum_put)` <br>
Put sum_put dollars on the card; you should also write: <br>
"You put sum_put dollars. total_sum dollars are left."
<br>

Example of work:
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

Running tests:
```bash
python run.py unittest task4
```