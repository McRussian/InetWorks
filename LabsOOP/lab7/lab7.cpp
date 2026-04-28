#ifdef _WIN32
#include <windows.h>
#endif

#include <clocale>
#include <iostream>
#include <string>

#include "computer.h"
#include "tablet.h"
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
	for (int i = 0; i < c.get_size(); ++i) {
		cout << i + 1 << "." << endl;
		c.print_at(i);
	}
}

void menu_add_computer(Computers &c) {
	cout << "Enter computer data:" << endl;
	Computer *comp = new Computer();
	comp->read_from_stdin();
	c += comp;
	cout << "Record added. Total: " << c.get_size() << endl;
}

void menu_add_laptop(Computers &c) {
	cout << "Enter laptop data:" << endl;
	Laptop *lap = new Laptop();
	lap->read_from_stdin();
	c += lap;
	cout << "Record added. Total: " << c.get_size() << endl;
}

void menu_add_tablet(Computers &c) {
	cout << "Enter tablet data:" << endl;
	Tablet *tab = new Tablet();
	tab->read_from_stdin();
	c += tab;
	cout << "Record added. Total: " << c.get_size() << endl;
}

void menu_edit(Computers &c) {
	int n = c.get_size();
	if (n == 0) { cout << "No data." << endl; return; }
	cout << "Record number to edit (1-" << n << "): ";
	int idx;
	cin >> idx;
	clear_stdin();
	if (idx < 1 || idx > n) { cout << "Invalid number." << endl; return; }
	c.edit_at(idx - 1);
	cout << "Record updated." << endl;
}

void menu_delete(Computers &c) {
	int n = c.get_size();
	if (n == 0) { cout << "No data." << endl; return; }
	cout << "Record number to delete (1-" << n << "): ";
	int idx;
	cin >> idx;
	clear_stdin();
	if (idx < 1 || idx > n) { cout << "Invalid number." << endl; return; }
	c.remove_at(idx - 1);
	cout << "Record deleted. Total: " << c.get_size() << endl;
}

void menu_total_price(Computers &c) {
	if (c.get_size() == 0) { cout << "No data." << endl; return; }
	c.task1();
}

void menu_max_clock(Computers &c) {
	if (c.get_size() == 0) { cout << "No data." << endl; return; }
	int pmin, pmax;
	cout << "Min. price: ";
	cin >> pmin;
	cout << "Max. price: ";
	cin >> pmax;
	clear_stdin();
	c.task2(pmin, pmax);
}

void menu_find_cpu(Computers &c) {
	if (c.get_size() == 0) { cout << "No data." << endl; return; }
	cout << "CPU brand: ";
	string cpu;
	getline(cin, cpu);
	c.find_by_cpu(cpu);
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
		cout << "1.  Read from file" << endl;
		cout << "2.  Show all records" << endl;
		cout << "3.  Add computer" << endl;
		cout << "4.  Add laptop" << endl;
		cout << "5.  Add tablet" << endl;
		cout << "6.  Edit record" << endl;
		cout << "7.  Delete record" << endl;
		cout << "8.  Total cost" << endl;
		cout << "9.  Max clock speed in price range" << endl;
		cout << "10. Find by CPU brand" << endl;
		cout << "11. Save to file" << endl;
		cout << "0.  Exit" << endl;
		cout << "Choice: ";
		cin >> choice;
		clear_stdin();

		switch (choice) {
			case 1:  menu_read_file(c, filename); break;
			case 2:  menu_print_all(c); break;
			case 3:  menu_add_computer(c); break;
			case 4:  menu_add_laptop(c); break;
			case 5:  menu_add_tablet(c); break;
			case 6:  menu_edit(c); break;
			case 7:  menu_delete(c); break;
			case 8:  menu_total_price(c); break;
			case 9:  menu_max_clock(c); break;
			case 10: menu_find_cpu(c); break;
			case 11: menu_save_file(c, filename); break;
			case 0:  break;
			default: cout << "Invalid choice." << endl;
		}
	}

	return 0;
}
