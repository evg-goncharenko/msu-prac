#include "interface.h"

using std::cin;
using std::cout;
using std::endl;

void authorization(Bank b) {
    int order_number = 0;
    cout << "Hello, you need to log in" << endl;
    do {
        int username;
        int password;
        cout << "Please enter your username" << endl;
        cin >> username;
        cout << "Please enter your password" << endl;
        cin >> password;

        if (b.is_authorization(username, password)) {
            Client cl = b.client_authorization(username, password);
            cout << "   You have successfully logged in the system" << endl;
            order_number = 1;
        } else {
            cout << "   Invalid username or password" << endl;
        }
    } while (!order_number);
}

void start_menu() {
    cout << "*******************************************" << endl;
    cout << "*              Start program              *" << endl;
    cout << "*******************************************" << endl;
    int order_number = 0;

    Bank Sber;
    int username = 5;
    int password = 100;
    Sber.add_client("Evg", INDIVIDUAL, username, password);

    authorization(Sber);
    Client cl = Sber.client_authorization(username, password);
    cl.get_info();

    do {
        cout << "Select an action:" << endl;
        cout << "[0] -> exit the program" << endl;
        cout << "[1] -> example of how the program works" << endl;
        cout << " :: ";
        cin >> order_number;

        switch (order_number) {
            case 0: {
                break;
            }
            case 1: {
                Bank Sberbank;
                Sberbank.add_client("Evg", INDIVIDUAL, 1, 1);
                Sberbank.add_client("Evg", LEGAL_ENTITY, 5, 10);
                Sberbank.return_client(0).set_name("Maksim");
                Sberbank.all_bank_information();

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
