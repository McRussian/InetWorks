#ifdef _WIN32
#include <windows.h>
#endif

#include <clocale>
#include <fstream>
#include <iostream>
#include <string>

#include "computer.h"
#include "computers.h"
#include "utils.h"

using namespace std;

void menu_read_file(Computers &c, string &filename) {
	cout << "Filename: ";
	getline(cin, filename);
	c.read_from_file(filename);
	cout << "Records read: " << c.get_size() << endl;
}

void menu_print_all(Computers &c) {
	if (c.get_size() == 0) {
		cout << "No data." << endl;
		return;
	}
	c.print();
}

void menu_add(Computers &c) {
	cout << "Enter new record data:" << endl;
	Computer comp;
	cin >> comp;
	c.add_computer(comp);
	cout << "Record added. Total records: " << c.get_size() << endl;
}

void menu_edit(Computers &c) {
	int n = c.get_size();
	if (n == 0) {
		cout << "No data." << endl;
		return;
	}
	cout << "Record number to edit (1-" << n << "): ";
	int idx;
	cin >> idx;
	clear_stdin();
	if (idx < 1 || idx > n) {
		cout << "Invalid number." << endl;
		return;
	}
	c.edit(idx - 1);
	cout << "Record updated." << endl;
}

void menu_delete(Computers &c) {
	int n = c.get_size();
	if (n == 0) {
		cout << "No data." << endl;
		return;
	}
	cout << "Record number to delete (1-" << n << "): ";
	int idx;
	cin >> idx;
	clear();
	if (idx < 1 || idx > n) {
		cout << "Invalid number." << endl;
		return;
	}
	c.remove_computer(idx - 1);
	cout << "Record deleted. Total records: " << c.get_size() << endl;
}

void menu_total_price(Computers &c) {
	if (c.get_size() == 0) {
		cout << "No data." << endl;
		return;
	}
	c.task1();
}

void menu_filter_price(Computers &c) {
	if (c.get_size() == 0) {
		cout << "No data." << endl;
		return;
	}
	int price_min, price_max;
	cout << "Min. price: ";
	cin >> price_min;
	cout << "Max. price: ";
	cin >> price_max;
	clear();
	c.task2(price_min, price_max);
}

void menu_save_file(Computers &c, const string &filename) {
	string fname = filename;
	if (fname.empty()) {
		cout << "Enter filename: ";
		getline(cin, fname);
	}
	c.save_to_file(fname);
	cout << "Saved to " << fname << endl;
}

int main() {
#ifdef _WIN32
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
#endif
	setlocale(LC_ALL, "");

	Computers c(100);
	string filename = "";

	int choice = -1;
	while (choice != 0) {
		cout << "\n=== Menu ===" << endl;
		cout << "1. Read from file" << endl;
		cout << "2. Show all records" << endl;
		cout << "3. Add record" << endl;
		cout << "4. Edit record" << endl;
		cout << "5. Delete record" << endl;
		cout << "6. Total cost" << endl;
		cout << "7. PC with max clock speed in price range" << endl;
		cout << "8. Save to file" << endl;
		cout << "0. Exit" << endl;
		cout << "Choice: ";
		cin >> choice;
		clear_stdin();

		switch (choice) {
			case 1: menu_read_file(c, filename); break;
			case 2: menu_print_all(c); break;
			case 3: menu_add(c); break;
			case 4: menu_edit(c); break;
			case 5: menu_delete(c); break;
			case 6: menu_total_price(c); break;
			case 7: menu_filter_price(c); break;
			case 8: menu_save_file(c, filename); break;
			case 0: break;
			default: cout << "Invalid choice." << endl;
		}
	}

	return 0;
}
