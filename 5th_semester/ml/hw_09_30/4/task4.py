class BankCard:
    total_sum = 0
    def __init__(self, total_sum):
        self.total_sum = total_sum
    @property
    def balance(self):
        if self.total_sum > 0:
            self.total_sum -= 1
        else:
            print("Not enough money to learn the balance.")
            raise ValueError
        return self.total_sum
    def put(self, sum_put):
        self.total_sum += sum_put
        print("You put", sum_put, "dollars.", self.total_sum, "dollars are left.", )
    def __call__(self, sum_spent):
        if self.total_sum - sum_spent >= 0:
            self.total_sum -= sum_spent
        else:
            print("Not enough money to spent", sum_spent, "dollars.")
            raise ValueError
        print("You spent", sum_spent, "dollars.", self.total_sum, "dollars are left.")
    def __repr__(self):
        return 'To learn the balance you should put the money on the card, spent some money or get the bank data. The last procedure is not free and costs 1 dollar.'

if __name__ == "__main__":
    a = BankCard(10)
    print(a.total_sum) # 10
    a(5) # You spent 5 dollars. 5 dollars are left
    print(a.total_sum) # 5
    print(a)
    """
    To learn the balance you should put the money on the card, 
    spent some money or get the bank data.
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

