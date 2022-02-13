from task4 import BankCard
import pytest


def test_bank_card():
	a = BankCard(100)
	assert a.total_sum == 100
	assert a.__repr__() == "To learn the balance you should put the money on the card, spent some money or get the bank data. The last procedure is not free and costs 1 dollar."
	a(50)
	assert a.total_sum == 50
	assert a.balance == 49
	try:
		a(50)
	except ValueError:
		pass
	a.put(30)
	assert a.balance == 78
