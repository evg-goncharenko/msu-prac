#include <iostream>
#include "../complex/complex.h"
using namespace std;

void start_menu() {
    
    cout << "*******************************************" << endl;
    cout << "*              Start program              *" << endl;
    cout << "*******************************************" << endl;
    int order_number;
    
    do {
        cout << "Select an action:" << endl;
        cout << "[0] -> exit the program" << endl;
        cout << "[1] -> example of how the program works" << endl;
        cout << "[2] -> adding two numbers" << endl;
        cout << "[3] -> subtracting two numbers" << endl;
        cout << "[4] -> multiplying two numbers" << endl;
        cout << "[5] -> dividing two numbers" << endl;
        cout << "[6] -> equality of two numbers" << endl;
        cout << "[7] -> inequality of two numbers" << endl;
        cout << "[8] -> modulus of a complex number" << endl;
        cout << " :: ";
        cin >> order_number;
        
        switch (order_number) {
            case 0: {
                break;
            }
            case 1: {
                Complex a (7, 1);
                const Complex b (4, -3);
                cout << "\ta = " << a << "; b = " << b << endl;
                cout << "\tre.a = " << a.real() << "; im.b = " << b.imag() << endl;
                cout << "\ta += b" << endl;
                a += b;
                cout << "\tResult:" << endl;
                cout << "\ta = " << a << "; b = " << b << endl;
                
                Complex c = a + b;
                
                cout << "\tc = a + b, c = " << c << endl;
                cout << "\ta * b = " << a * b << endl;
                break;
            }
            case 2: {
                Complex a (5, 1);
                Complex b (9, -3);
                cout << "\ta = " << a << "; b = " << b << endl;
                cout << "\ta + b = " << a + b << endl;
                break;
            }
            case 3: {
                Complex a (0, 1);
                Complex b (1, -3);
                cout << "\ta = " << a << "; b = " << b << endl;
                cout << "\ta - b = " << a - b << endl;
                break;
            }
            case 4: {
                Complex a (8, 9);
                Complex b (11, -2);
                cout << "\ta = " << a << "; b = " << b << endl;
                cout << "\ta * b = " << a * b << endl;
                break;
            }
            case 5: {
                Complex a (1, 2);
                Complex b (5, -3);
                cout << "\ta = " << a << "; b = " << b << endl;
                cout << "\ta / b = " << a / b << endl;
                break;
            }
            case 6: {
                Complex a (1, 2);
                Complex b (1, 2);
                cout << "\ta = " << a << "; b = " << b << endl;
                if (a == b) {
                    cout << "\tequal" << endl;
                } else {
                    cout << "\tnot equal" << endl;
                }
                break;
            }
            case 7: {
                Complex a (2, 2);
                Complex b (1, 2);
                cout << "\ta = " << a << "; b = " << b << endl;
                if (a != b) {
                    cout << "\tnot equal" << endl;
                } else {
                    cout << "\tequal" << endl;
                }
                break;
            }
            case 8: {
                Complex a (9, 2);
                cout << "\ta = " << a << endl;
                cout << "\tmodulus of a complex number: " << a.abs() << endl;
                break;
            }
            default: {
                cout << "\tInvalid value entered, enter a number from 0 to 9" << endl;
                break;
            }
        }
        
    } while (order_number != 0);
    
    cout << "*******************************************" << endl;
    cout << "*            End of the program           *" << endl;
    cout << "*******************************************" << endl;
}
