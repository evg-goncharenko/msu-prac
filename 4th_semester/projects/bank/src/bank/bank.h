#ifndef BANK_H
#define BANK_H

#include <cstring>
#include <ctime>
#include <iostream>
#include <map>
#include <vector>

#define INDIVIDUAL 0
#define LEGAL_ENTITY 1

#define ACCOUNT 2
#define CARD 3
#define SAVINGS_ACCOUNT 4

#define INTERNAL_TRANSACTION 5
#define EXTERNAL_TRANSACTION 6

#define LARGE_SUM_OF_MONEY 8000000

#define LIMIT_INTERNAL_TRANSACTION 2000000
#define LIMIT_EXTERNAL_TRANSACTION 1000000

using std::string;

class Account {
    string ID_;
    double amount_of_money_;

public:
    Account(const string &id, const double money);

    Account(){};
    ~Account(){};

    Account(const Account &c);

    Account &operator=(Account &c);

    string get_id() const;
    double get_money() const;
    void set_money(double new_value);
};

class Savings_account : public Account {
    double interest_rate_;

public:
    Savings_account(const double rate);
    Savings_account(){};
    ~Savings_account(){};

    friend std::ostream &operator<<(std::ostream &, Savings_account &cl);
    friend std::istream &operator>>(std::istream &, Savings_account &cl);

    void account_capitalization();
    double get_interest_rate() const;
};

class Card : public Account {
    string cv_code_;
    string card_number_;

public:
    Card(const string &cv_code_, const string &card_number_);
    Card(){};
    ~Card(){};

    friend std::ostream &operator<<(std::ostream &, Card &cl);
    friend std::istream &operator>>(std::istream &, Card &cl);

    string get_cv_code() const;
    string get_card_numb() const;
};

class Client {
    string full_name_;
    int client_type_;
    int ID_;
    int login_;
    int password_;
    std::vector<string> accounts_IDs;

public:
    Client(const string &new_name, const int new_client_type,
           const int new_ID, const int new_login, const int new_password);
    Client(const Client &other);
    Client(){};
    ~Client(){};

    Client &operator=(const Client &other);
    Savings_account &operator[](const string &ID);

    friend std::ostream &operator<<(std::ostream &, Client &cl);
    friend std::istream &operator>>(std::istream &, Client &cl);

    void add_new_account(string account_ID);
    void add_new_savings_account(string account_ID, double interest_rate);
    void add_new_card(string card_ID);

    const string get_name();
    const int get_ID();
    const int get_client_type();
    const int get_login();
    const int get_password();

    void set_name(const string name);
    void set_ID(const int ID);
    void set_client_type(const int client_type);

    void print_accounts();
    std::vector<string> get_accounts();
    void get_cards();
    void get_info();
};

class Transaction {
    string addressee_;
    string sender_;
    tm *time_;
    double amount_of_money_transfer_;

public:
    bool is_legal_transaction();
    bool enough_money();
    bool is_limited(int type_transaction);

    virtual void add_money(Account &S, double current_amount) = 0;
    virtual void add_money(Savings_account &S, double current_amount) = 0;
    virtual void add_money(Card &S, double current_amount) = 0;

    void add_money();

    void set_addressee(string addresse);
    void set_sender(string sender);

    virtual void c2c_transaction(Account &S, Account &A, double current_amount) = 0;
};

class External_transaction : public Transaction {
    // TODO
};

class Internal_transaction : public Transaction {
    void add_money(Account &S, double current_amount);
    void add_money(Savings_account &S, double current_amount){};
    void add_money(Card &S, double current_amount){};

    void c2c_transaction(Account &S, Account &A, double current_amount);
};

class Bank {
    std::map<string, Account> accounts_;
    std::map<string, Savings_account> savings_accounts_;
    std::map<string, Card> cards_;
    std::map<int, Client> clients_;
    Transaction **transactions_array;
    size_t transactions_number;
    int ID_;
    void my_realloc();

public:
    Bank();

    void add_client(const string &name, const int type, const int login, const int password);
    Client &return_client(const int ID);
    bool is_authorization(const int login, const int password);
    Client &client_authorization(const int login, const int password);
    void all_bank_information();
    void print_of_lost_people(const string name);
    void internal_transaction(int ID1, int ID2, double current_amount);
};

#endif  // BANK_H
