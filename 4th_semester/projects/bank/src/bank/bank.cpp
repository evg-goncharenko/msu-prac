#include "bank.h"

using std::string;

// ACCOUNT

Account::Account(const string &id, const double money) {
    ID_ = id;
    amount_of_money_ = money;
}

Account::Account(const Account &c) {
    ID_ = c.ID_;
    amount_of_money_ = c.amount_of_money_;
}

Account& Account::operator = (Account &c) {
    ID_ = c.ID_;
    amount_of_money_ = c.amount_of_money_;
    return (*this);
}

string Account::get_id() const {
    return ID_;
}

double Account::get_money() const {
    return amount_of_money_;
}

void Account::set_money(double new_value) {
    amount_of_money_ = new_value;
}

// SAVINGS_ACCOUNT

Savings_account::Savings_account(const double rate) {
    interest_rate_ = rate;
}

std::ostream& operator << (std::ostream &out, Savings_account &c) {
    out << "{interest_rate: " << c.interest_rate_ << "}";
    return out;
}

std::istream& operator >> (std::istream &in, Savings_account &c) {
    in >> c.interest_rate_;
    return in;
}

void Savings_account::account_capitalization() {
    set_money(this->get_money() * (1 + interest_rate_ / 100));
}

double Savings_account::get_interest_rate() const {
    return interest_rate_;
}

// CARD

Card::Card(const string &cv, const string &card_numb) {
    cv_code_ = cv;
    card_number_ = card_numb;
}

std::ostream& operator << (std::ostream &out, Card &c) {
    out << "{cv_code: " << c.cv_code_ << ", card_number: " << c.card_number_ << "}";
    return out;
}

std::istream& operator >> (std::istream &in, Card &c) {
    in >> c.cv_code_ >> c.card_number_;
    return in;
}

string Card::get_cv_code() const {
    return cv_code_;
}

string Card::get_card_numb() const {
    return card_number_;
}

// CLIENT

Client::Client(const string &new_name, const int new_client_type,
                     const int new_ID, const int new_login, const int new_password) {
    full_name_ = new_name;
    client_type_ = new_client_type;
    ID_ = new_ID;
    login_ = new_login;
    password_ = new_password;
}

Client::Client(const Client &other) {
    full_name_ = other.full_name_;
    client_type_ = other.client_type_;
    ID_ = other.ID_;
    login_ = other.login_;
    password_ = other.password_;
    accounts_IDs = other.accounts_IDs;
}

Client& Client::operator = (const Client &other) {
    full_name_ = other.full_name_;
    client_type_ = other.client_type_;
    ID_ = other.ID_;
    accounts_IDs = other.accounts_IDs;
    return *this;
}

std::ostream& operator << (std::ostream &out, Client &cl) {
    out << "{full_name: " << cl.full_name_ << ", ID: " << cl.ID_ << ", type: " << cl.client_type_ << "}";
    return out;
}

std::istream& operator >> (std::istream &in, Client &cl) {
    in >> cl.full_name_ >> cl.ID_ >> cl.client_type_;
    return in;
}

void Client::add_new_savings_account(string ID_account, double interest_rate) {
}

void Client::add_new_card(string ID_card) {}

const string Client::get_name() {
    return full_name_;
}

const int Client::get_ID() {
    return ID_;
}

const int Client::get_client_type() {
    return client_type_;
}

const int Client::get_login() {
    return login_;
}

const int Client::get_password() {
    return password_;
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

void Client::print_accounts() {
    if (accounts_IDs.empty()) {
        std::cout << "This customer doesn't have any accounts yet" << std::endl;
    } else {
        for (int i = 0; i < accounts_IDs.size(); ++i) {
            std::cout << "IDs: " << accounts_IDs[i] << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<string> Client::get_accounts() {
    return accounts_IDs;
}

void Client::get_cards() {
    if (accounts_IDs.empty()) {
        std::cout << "This customer doesn't have any cards yet" << std::endl;
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

// TRANSACTION

bool Transaction::is_legal_transaction() {
    // TODO
    
    return true;
}

bool Transaction::enough_money() {
    // TODO
    
    return true;
}

bool Transaction::is_limited(int type_transaction) {
    if (type_transaction == INTERNAL_TRANSACTION) {
        if (amount_of_money_transfer_ > LIMIT_INTERNAL_TRANSACTION) {
            return true;
        }
    }
    
    if (type_transaction == EXTERNAL_TRANSACTION) {
        if (amount_of_money_transfer_ > LIMIT_EXTERNAL_TRANSACTION) {
            return true;
        }
    }
    return false;
}

void Transaction::set_addressee(string addressee) {
    addressee_ = addressee;
}

void Transaction::set_sender(string sender) {
    sender_ = sender;
}

// EXTERNAL_TRANSACTION

// INTERNAL TRANSACTION

void Internal_transaction::add_money(Account &S, double current_amount) {
    if (current_amount <= 0) {
        std::cout << "The entered amount of money must be positive" << std::endl;
        return;
    }
    S.set_money(S.get_money() + current_amount);
}

void Internal_transaction::c2c_transaction(Account &S, Account &A, double current_amount) {
    if (current_amount <= 0) {
        std::cout << "The entered amount of money must be positive" << std::endl;
        return;
    } else if (S.get_money() - current_amount < 0) {
        std::cout << "Insufficient funds" << std::endl;
    }
    
    S.set_money(S.get_money() - current_amount);
    A.set_money(A.get_money() + current_amount);
}

// BANK

void Bank::my_realloc() {
        Transaction **new_transactions = new Transaction* [++transactions_number];
        for (auto i = 0; i < transactions_number - 1; ++i) {
            new_transactions[i] = transactions_array[i];
        }
        
        delete [] transactions_array;
        transactions_array = new_transactions;
}

Bank::Bank() : transactions_array{nullptr}, transactions_number{0}, ID_(0) {};

void Bank::add_client(const string &name, const int type, const int login, const int password) {
    Client new_client(name, type, ID_, login, password);
    clients_.insert(std::pair<int, Client>(ID_, new_client));
    ++ID_;
};

Client &Bank::return_client(const int ID) {
    return clients_.at(ID);
}

bool Bank::is_authorization(const int login, const int password) {
    for (auto it = clients_.begin(); it != clients_.end(); ++it) {
        if ((it->second.get_login() == login) && (it->second.get_password() == password)) {
            return true;
        }
    }
    return false;
}
Client &Bank::client_authorization(const int login, const int password) {
    for (auto it = clients_.begin(); it != clients_.end(); ++it) {
        if ((it->second.get_login() == login) && (it->second.get_password() == password)) {
            return clients_.at(it->second.get_ID());
        }
    }
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

void Bank::internal_transaction(int ID1, int ID2, double current_amount) {
    std::cout << "choose your account number" << std::endl;
    clients_[ID1].print_accounts();
    std::string num1, num2;
    std::cin >> num1;
    
    Internal_transaction new_transaction;
    
    my_realloc();
    transactions_array[transactions_number - 1] = &new_transaction;
}
