#include "interface.h"

using std::cin;
using std::cout;
using std::endl;

void start_menu() {
    
    cout << "*******************************************" << endl;
    cout << "*              Start program              *" << endl;
    cout << "*******************************************" << endl;
    int order_number;
    
    do {
        cout << "Select an action:" << endl;
        cout << "[0] -> exit the program" << endl;
        cout << "[1] -> example of how the program works" << endl;
        cout << "[2] -> opening an account and a bank card" << endl;
        cout << "[3] -> account capitalization" << endl;
        cout << "[4] -> making transactions with money" << endl;
        cout << "[5] -> getting various information" << endl;
        cout << "[6] -> working with individuals and legal entities" << endl;
        cout << "[7] -> interactive customer search" << endl;
        cout << " :: ";
        cin >> order_number;
        
        switch (order_number) {
            case 0: {
                break;
            }
            case 1: {
                Bank Sberbank;
                Sberbank.add_client("Evg", INDIVIDUAL);
                Sberbank.add_client("Evg", LEGAL_ENTITY);
                Sberbank.return_client(0).set_name("Maksim");
                
                Sberbank.return_client(0).add_new_account("N12346575", 2);
                Sberbank.return_client(0).add_new_account("N12346666", 5);
                Sberbank.return_client(0).add_new_card("N12886575");
                
                Sberbank.return_client(0).withdraw_money("N12886575", 500, CARD);
                Sberbank.return_client(0).add_money("N12886575", 3000, CARD);
                Sberbank.return_client(0).add_money("N12346666", 3000, SAVINGS_ACCOUNT);
                Sberbank.return_client(0).return_account("N12346666").account_capitalization();
                Sberbank.return_client(0).withdraw_money("N12886575", 1000, CARD);
                Sberbank.all_bank_information();
                break;
            }
            case 2: {
                Bank Tinkoff;
                Tinkoff.add_client("Ivan", INDIVIDUAL);
                Tinkoff.return_client(0).add_new_account("N12346666", 5);
                Tinkoff.return_client(0).add_new_card("N12886575");
                Tinkoff.all_bank_information();
                break;
            }
            case 3: {
                Bank Tinkoff;
                Tinkoff.add_client("Ivan", INDIVIDUAL);
                Tinkoff.return_client(0).add_new_account("N12346666", 5);
                Tinkoff.return_client(0).add_money("N12346666", 3000, SAVINGS_ACCOUNT);
                Tinkoff.return_client(0).return_account("N12346666").account_capitalization();
                Tinkoff.all_bank_information();
                break;
            }
            case 4: {
                Bank VTB;
                VTB.add_client("Evg", LEGAL_ENTITY);
                
                VTB.return_client(0).add_new_card("N12346575");
                
                VTB.return_client(0).withdraw_money("N12346575", 500, CARD);
                VTB.return_client(0).add_money("N12346575", 3000, CARD);
                VTB.return_client(0).withdraw_money("N12346575", 1000, CARD);
                VTB.all_bank_information();
                break;
            }
            case 5: {
                Bank Sberbank;
                Sberbank.add_client("Evg", LEGAL_ENTITY);
                Sberbank.add_client("Michael", INDIVIDUAL);
                Sberbank.return_client(0).add_new_account("N12346575", 2);
                Sberbank.return_client(0).add_new_account("N12346666", 5);
                Sberbank.return_client(0).add_new_card("N12886575");
                Sberbank.return_client(0).add_money("N12886575", 3000, CARD);
                cout << "Getting information about savings accounts:" << std::endl;
                Sberbank.return_client(0).get_accounts();
                cout << "Getting information about cards:" << std::endl;
                Sberbank.return_client(0).get_cards();
                cout << "Getting all the information about the first client:" << endl;
                Sberbank.return_client(0).get_info();
                cout << "Getting all the information about all the clients:" << endl;
                Sberbank.all_bank_information();
                break;
            }
            case 6: {
                Bank ATNT;
                ATNT.add_client("Bob", INDIVIDUAL);
                ATNT.add_client("Andrey", LEGAL_ENTITY);
                ATNT.return_client(0).add_new_account("N12346575", 5);
                ATNT.return_client(1).add_new_account("N12346666", 5);
                
                ATNT.return_client(0).add_money("N12346575", 8500000, SAVINGS_ACCOUNT);
                ATNT.return_client(1).add_money("N12346666", 8500000, SAVINGS_ACCOUNT);
                
                ATNT.all_bank_information();
                break;
            }
            case 7: {
                cout << "Enter full name for a search:" << endl;
                string tmp; cin >> tmp;
                Bank Sberbank;
                Sberbank.add_client("Bob", INDIVIDUAL);
                Sberbank.add_client("Michael", INDIVIDUAL);
                Sberbank.add_client("Bob", LEGAL_ENTITY);
                Sberbank.print_of_lost_people(tmp);
                
                break;
            }
            default: {
                cout << "\tInvalid value entered, enter a number from 0 to 7" << endl;
                break;
            }
        }
        
    } while (order_number != 0);
    
    cout << "*******************************************" << endl;
    cout << "*            End of the program           *" << endl;
    cout << "*******************************************" << endl;
}
