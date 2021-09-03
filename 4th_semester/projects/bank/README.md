## Bank

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

How to build:
```bash
make all
```

How to launch:
```bash
./prog
```

This project simulates the operation of the bank's online services. In the beginning, this is the user authorization. After a successful login, the client will be printed with all their basic information, for example, information about their bank accounts. Next, the client can perform operations with finances and accounts, for example:
- Transactions of cardholders using an electronic account:
    - Receiving cash;
    - Money transfering;
    - Payment of bills;
- Transfers from account to account without plastic cards;
- Money transfers without opening an account.

### Architecture:
An integral part of all financial transactions in bank is the transaction (`Transaction`). The transaction consists of three parts: 
<br><b> Request -> Task Completion -> Response </b><br>
It is important to understand that transaction is an operation with funds in the bank. For example, this includes paying for a purchase with a bank card or withdrawing cash from an ATM. There are two types of transactions:
- Internal transaction (any transfers within the same bank)
- External transaction (transfers between banks)
<br><br>

Another important component of the architecture is the bank accounts and their hierarchy. Base class of all accounts will be the bank account `Account`, which can exist independently. `Savings_account` and `Card` includes the bank account object and, of course, they inherit all its properties. A distinctive feature of a savings account is the interest rate, and a bank card - a unique number and CVV code.
