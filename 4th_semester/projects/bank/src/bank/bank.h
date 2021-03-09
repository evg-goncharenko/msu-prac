#ifndef BANK_H
#define BANK_H

#include <iostream>
#include <cstring>
#include <vector>
#include <map>

#define INDIVIDUAL 0
#define LEGAL_ENTITY 1

#define CARD 2
#define SAVINGS_ACCOUNT 3

#define LARGE_SUM_OF_MONEY 8000000

using std::string;

class Savings_accounts {
    string ID_;
    double interest_rate_;
    double amount_of_money_;
public:
    Savings_accounts(const string &id, const double rate, const double money);
    Savings_accounts() {};
    
    friend std::ostream& operator << (std::ostream &, Savings_accounts &cl);
    friend std::istream& operator >> (std::istream &, Savings_accounts &cl);
    
    void account_capitalization();
    
    string get_id() const;
    double get_interest_rate() const;
    double get_money() const;
    
    void set_money(double new_value);
};

class Cards {
    string ID_;
    double limit_;
    double amount_of_money_;
    
public:
    Cards(const string &id, const double money);
    Cards() {};
    
    friend std::ostream& operator << (std::ostream &, Cards &cl);
    friend std::istream& operator >> (std::istream &, Cards &cl);
    
    string get_id() const;
    double get_money() const;
    void set_money(double new_value);
};

class Client {
    string full_name_;
    int ID_;
    int client_type_;
    std::map<string, Savings_accounts> accounts_;
    std::map<string, Cards> cards_;
public:
    Client(const string &new_name, const int new_client_type,
           const int new_ID);
    Client(const Client &other);
    Client() {};
    ~Client() {};
    
    Client &operator = (const Client &other);
    Savings_accounts &operator[](const string &ID);
    
    friend std::ostream& operator << (std::ostream &, Client &cl);
    friend std::istream& operator >> (std::istream &, Client &cl);
    
    void add_new_account(string ID_account, double interest_rate);
    void add_new_card(string ID_card);
    
    const string get_name();
    const int get_ID();
    const int get_client_type();
    
    void set_name(const string name);
    void set_ID(const int ID);
    void set_client_type(const int client_type);
    
    void get_accounts();
    void get_cards();
    void get_info();
    
    void withdraw_money(string account_ID, double value, int type_storage);
    void add_money(string account_ID, double value, int type_storage);
    
    Savings_accounts &return_account(const string ID);
};


class Bank {
    std::map<int, Client> clients_;
    int ID_;
public:
    Bank();
    
    void add_client(const string &name, const int type);
    Client &return_client(const int ID);
    void all_bank_information();
    void print_of_lost_people(const string name);
};

#endif // BANK_H
