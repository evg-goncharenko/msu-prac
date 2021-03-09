#include "bank.h"

using std::string;

// SAVINGS_ACCOUNTS

Savings_accounts::Savings_accounts(const string &id, const double rate, const double money) {
    ID_ = id;
    interest_rate_ = rate;
    amount_of_money_ = money;
}

std::ostream& operator << (std::ostream &out, Savings_accounts &c) {
    out << "{ID: " << c.ID_ << ", interest_rate: " << c.interest_rate_ << ", amount_of_money: " << c.amount_of_money_ << "}";
    return out;
}

std::istream& operator >> (std::istream &in, Savings_accounts &c) {
    in >> c.ID_ >> c.interest_rate_ >> c.amount_of_money_;
    return in;
}

void Savings_accounts::account_capitalization() {
    amount_of_money_ *= (1 + interest_rate_ / 100);
}

string Savings_accounts::get_id() const {
    return ID_;
}
double Savings_accounts::get_interest_rate() const {
    return interest_rate_;
}
double Savings_accounts::get_money() const {
    return amount_of_money_;
}

void Savings_accounts::set_money(double new_value) {
    amount_of_money_ = new_value;
}

// CARDS

Cards::Cards(const string &id, const double money) {
    ID_ = id;
    amount_of_money_ = money;
}

std::ostream& operator << (std::ostream &out, Cards &c) {
    out << "{ID: " << c.ID_ << ", limit: " << c.limit_ << ", amount_of_money: " << c.amount_of_money_ << "}";
    return out;
}

std::istream& operator >> (std::istream &in, Cards &c) {
    in >> c.ID_ >> c.limit_ >> c.amount_of_money_;
    return in;
}

string Cards::get_id() const {
    return ID_;
}
double Cards::get_money() const {
    return amount_of_money_;
}

void Cards::set_money(double new_value) {
    amount_of_money_ = new_value;
}

// CLIENT

Client::Client(const string &new_name, const int new_client_type,
               const int new_ID){
    full_name_ = new_name;
    client_type_ = new_client_type;
    ID_ = new_ID;
}

Client::Client(const Client &other) {
    full_name_ = other.full_name_;
    client_type_ = other.client_type_;
    ID_ = other.ID_;
    accounts_ = other.accounts_;
    cards_ = other.cards_;
}

Client& Client::operator = (const Client &other) {
    full_name_ = other.full_name_;
    client_type_ = other.client_type_;
    ID_ = other.ID_;
    accounts_ = other.accounts_;
    cards_ = other.cards_;
    return *this;
}

Savings_accounts& Client::operator[](const string &ID) {
    return accounts_.at(ID);
}

std::ostream& operator << (std::ostream &out, Client &cl) {
    out << "{full_name: " << cl.full_name_ << ", ID: " << cl.ID_ << ", type: " << cl.client_type_ << "}";
    return out;
}

std::istream& operator >> (std::istream &in, Client &cl) {
    in >> cl.full_name_ >> cl.ID_ >> cl.client_type_;
    return in;
}

void Client::add_new_account(string ID_account, double interest_rate) {
    Savings_accounts new_account(ID_account, interest_rate, 0);
    accounts_.insert(std::pair<string, Savings_accounts>(ID_account, new_account));
}
void Client::add_new_card(string ID_card) {
    Cards new_card(ID_card, 0);
    cards_.insert(std::pair<string, Cards>(ID_card, new_card));
}

const string Client::get_name() {
    return full_name_;
}
const int Client::get_ID() {
    return ID_;
}
const int Client::get_client_type() {
    return client_type_;
}

void Client::set_name(const string name) {
    full_name_ = name;
}
void Client::set_ID(const int ID) {
    ID_ = ID;
}
void Client::set_client_type(const int client_type) {
    client_type_ = client_type;
}

void Client::get_accounts() {
    if (accounts_.empty()) {
        std::cout << "This customer doesn't have any savings accounts yet" << std::endl;
    } else {
        for (auto it = accounts_.begin(); it != accounts_.end(); ++it) {
            std::cout << "account ID: " << it -> first << std::endl;
            std::cout << "account interes_rate: " << it -> second.get_interest_rate()
            << std::endl;
            std::cout << "account amount_of_money: " << it -> second.get_money()
            << std::endl << std::endl;
        }
    }
}
void Client::get_cards() {
    if (cards_.empty()) {
        std::cout << "This customer doesn't have any cards yet" << std::endl;
    } else {
        for (auto it = cards_.begin(); it != cards_.end(); ++it) {
            std::cout << "card ID: " << it -> first << std::endl;
            std::cout << "card amount_of_money: " << it -> second.get_money()
            << std::endl << std::endl;
        }
    }
}

void Client::get_info() {
    std::cout << "Full name of the client: " << full_name_ << std::endl;
    std::cout << "Client ID: " << ID_ << std::endl;
    
    if (client_type_ == INDIVIDUAL) {
        std::cout << "The client is an individual" << std::endl;
    } else if (client_type_ == LEGAL_ENTITY) {
        std::cout << "The client is a legal entity" << std::endl;
    }
    
    get_accounts();
    get_cards();
}

void Client::withdraw_money(string account_ID, double value, int type_storage) {
    if (value <= 0) {
        std::cout << "The entered amount of money must be positive" << std::endl;
    } else if (value > LARGE_SUM_OF_MONEY && client_type_ == LEGAL_ENTITY) {
        std::cout << "An individual can't make a transaction for more than 8 million" << std::endl;
        return;
    }
    
    double current_amount_of_money;
    if (type_storage == CARD) {
        current_amount_of_money = cards_.at(account_ID).get_money();
        if (current_amount_of_money < value) {
            std::cout << "There are not enough funds on the card" << std::endl;
        } else {
            cards_.at(account_ID).set_money(current_amount_of_money - value);
        }
    } else if (type_storage == SAVINGS_ACCOUNT) {
        current_amount_of_money = accounts_.at(account_ID).get_money();
        if (current_amount_of_money < value) {
            std::cout << "There are not enough funds on the savings account" << std::endl;
        } else {
            accounts_.at(account_ID).set_money(current_amount_of_money - value);
        }
    }
}
void Client::add_money(string account_ID, double value, int type_storage) {
    if (value <= 0) {
        std::cout << "The entered amount of money must be positive" << std::endl;
    } else if (value > LARGE_SUM_OF_MONEY && client_type_ == INDIVIDUAL) {
        std::cout << "An individual can't make a transaction for more than 8 million" << std::endl;
        return;
    }
    
    double current_amount_of_money;
    if (type_storage == CARD) {
        current_amount_of_money = cards_.at(account_ID).get_money();
        cards_.at(account_ID).set_money(current_amount_of_money + value);
        
    } else if (type_storage == SAVINGS_ACCOUNT) {
        current_amount_of_money = accounts_.at(account_ID).get_money();
        accounts_.at(account_ID).set_money(current_amount_of_money + value);
    }
}

Savings_accounts &Client::return_account(string ID) {
    return accounts_.at(ID);
}

// BANK

Bank::Bank() : ID_(0) {};

void Bank::add_client(const string &name, const int type) {
    Client new_client(name, type, ID_);
    clients_.insert(std::pair<int, Client>(ID_, new_client));
    ++ID_;
};

Client &Bank::return_client(const int ID) {
    return clients_.at(ID);
}

void Bank::all_bank_information() {
    if (clients_.empty()) {
        std::cout << "No bank customers" << std::endl;
    } else {
        for (auto it = clients_.begin(); it != clients_.end(); ++it) {
            it->second.get_info();
        }
    }
}

void Bank::print_of_lost_people(string name) {
    if (clients_.empty()) {
        std::cout << "No bank customers" << std::endl;
    } else {
        for (auto it = clients_.begin(); it != clients_.end(); ++it) {
            if ( it->second.get_name()== name) {
                it->second.get_info();
            }
        }
    }
}
