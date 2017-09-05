#include "Gandra.h"

#include<iostream>
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::streamsize;

#include<limits>
using std::numeric_limits;

#include "Errors.h"

int main()
try {
	char choice {'?'};

	display_glbl_menu();

	while (cin >> choice) {

		char next = cin.peek();

		if (next != '\n') {
			choice = '?';
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');
		}

		switch (choice) {
		case '1':
			cout << endl;
			gandrac_same_screen();
			break;

		case '2':
			cout << endl;
			gandrac_network();
			break;

		case 'x':
			cout << "\nGoodbye !" << endl;
			return 0;

		default:
			cout << "\nPlease enter a valid choice." << endl;
			break;
		}

		cout << endl;
		display_glbl_menu();
	}
	return 0;
}
catch (runtime_error &e) {
	cerr << "Error: " << e.what() << endl;
	return 1;
}