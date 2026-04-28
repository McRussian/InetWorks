#ifdef _WIN32
#include <windows.h>
#endif

#include <clocale>
#include <iostream>
#include <string>

#include "computer.h"
#include "computers.h"
#include "utils.h"

using namespace std;

void menu_read_computers(Computers &c, string &filename) {
	cout << "Filename: ";
	getline(cin, filename);
	c.read_from_file(filename);
	cout << "Records read: " << c.get_size() << endl;
}

void menu_read_laptops(Computers &c, string &filename) {
	cout << "Filename: ";
	getline(cin, filename);
	c.read_from_file_laptops(filename);
	cout << "Records read: " << c.get_size_laptops() << endl;
}

void menu_print_all(Computers &c) {
	int n = c.get_size(), n2 = c.get_size_laptops();
	if (n == 0 && n2 == 0) { cout << "No data." << endl; return; }
	if (n > 0) {
		cout << "Computers (" << n << "):" << endl;
		for (int i = 0; i < n; ++i) {
			cout << i + 1 << "." << endl;
			c.print_at_computer(i);
		}
	}
	if (n2 > 0) {
		cout << "Laptops (" << n2 << "):" << endl;
		for (int i = 0; i < n2; ++i) {
			cout << i + 1 << "." << endl;
			c.print_at_laptop(i);
		}
	}
}

void menu_add_computer(Computers &c) {
	cout << "Enter computer data:" << endl;
	Computer comp;
	comp.read_from_stdin();
	c += comp;
	cout << "Record added. Total computers: " << c.get_size() << endl;
}

void menu_add_laptop(Computers &c) {
	cout << "Enter laptop data:" << endl;
	Laptop lap;
	lap.read_from_stdin();
	c += lap;
	cout << "Record added. Total laptops: " << c.get_size_laptops() << endl;
}

void menu_edit_computer(Computers &c) {
	int n = c.get_size();
	if (n == 0) { cout << "No computers." << endl; return; }
	cout << "Computer number to edit (1-" << n << "): ";
	int idx;
	cin >> idx;
	clear_stdin();
	if (idx < 1 || idx > n) { cout << "Invalid number." << endl; return; }
	c.edit_computer(idx - 1);
	cout << "Record updated." << endl;
}

void menu_edit_laptop(Computers &c) {
	int n = c.get_size_laptops();
	if (n == 0) { cout << "No laptops." << endl; return; }
	cout << "Laptop number to edit (1-" << n << "): ";
	int idx;
	cin >> idx;
	clear_stdin();
	if (idx < 1 || idx > n) { cout << "Invalid number." << endl; return; }
	c.edit_laptop(idx - 1);
	cout << "Record updated." << endl;
}

void menu_delete_computer(Computers &c) {
	int n = c.get_size();
	if (n == 0) { cout << "No computers." << endl; return; }
	cout << "Computer number to delete (1-" << n << "): ";
	int idx;
	cin >> idx;
	clear_stdin();
	if (idx < 1 || idx > n) { cout << "Invalid number." << endl; return; }
	c.remove_computer(idx - 1);
	cout << "Record deleted. Total computers: " << c.get_size() << endl;
}

void menu_delete_laptop(Computers &c) {
	int n = c.get_size_laptops();
	if (n == 0) { cout << "No laptops." << endl; return; }
	cout << "Laptop number to delete (1-" << n << "): ";
	int idx;
	cin >> idx;
	clear_stdin();
	if (idx < 1 || idx > n) { cout << "Invalid number." << endl; return; }
	c.remove_laptop(idx - 1);
	cout << "Record deleted. Total laptops: " << c.get_size_laptops() << endl;
}

void menu_total_price(Computers &c) {
	if (c.get_size() == 0 && c.get_size_laptops() == 0) { cout << "No data." << endl; return; }
	c.task1();
}

void menu_max_clock(Computers &c) {
	if (c.get_size() == 0 && c.get_size_laptops() == 0) { cout << "No data." << endl; return; }
	int pmin, pmax;
	cout << "Min. price: ";
	cin >> pmin;
	cout << "Max. price: ";
	cin >> pmax;
	clear_stdin();
	c.task2(pmin, pmax);
}

void menu_find_cpu(Computers &c) {
	if (c.get_size() == 0 && c.get_size_laptops() == 0) { cout << "No data." << endl; return; }
	cout << "CPU brand: ";
	string cpu;
	getline(cin, cpu);
	c.find_by_cpu(cpu);
}

void menu_save_computers(Computers &c, const string &filename) {
	string fname = filename;
	if (fname.empty()) {
		cout << "Enter filename: ";
		getline(cin, fname);
	}
	c.save_to_file(fname);
	cout << "Saved to " << fname << endl;
}

void menu_save_laptops(Computers &c, const string &filename) {
	string fname = filename;
	if (fname.empty()) {
		cout << "Enter filename: ";
		getline(cin, fname);
	}
	c.save_to_file_laptops(fname);
	cout << "Saved to " << fname << endl;
}

int main() {
#ifdef _WIN32
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
#endif
	setlocale(LC_ALL, "");

	Computers c(100);
	string file_computers = "", file_laptops = "";

	int choice = -1;
	while (choice != 0) {
		cout << "\n=== Menu ===" << endl;
		cout << "1.  Read computers from file" << endl;
		cout << "2.  Read laptops from file" << endl;
		cout << "3.  Show all records" << endl;
		cout << "4.  Add computer" << endl;
		cout << "5.  Add laptop" << endl;
		cout << "6.  Edit computer" << endl;
		cout << "7.  Edit laptop" << endl;
		cout << "8.  Delete computer" << endl;
		cout << "9.  Delete laptop" << endl;
		cout << "10. Total cost" << endl;
		cout << "11. Max clock speed in price range" << endl;
		cout << "12. Find by CPU brand" << endl;
		cout << "13. Save computers to file" << endl;
		cout << "14. Save laptops to file" << endl;
		cout << "0.  Exit" << endl;
		cout << "Choice: ";
		cin >> choice;
		clear_stdin();

		switch (choice) {
			case 1:  menu_read_computers(c, file_computers); break;
			case 2:  menu_read_laptops(c, file_laptops); break;
			case 3:  menu_print_all(c); break;
			case 4:  menu_add_computer(c); break;
			case 5:  menu_add_laptop(c); break;
			case 6:  menu_edit_computer(c); break;
			case 7:  menu_edit_laptop(c); break;
			case 8:  menu_delete_computer(c); break;
			case 9:  menu_delete_laptop(c); break;
			case 10: menu_total_price(c); break;
			case 11: menu_max_clock(c); break;
			case 12: menu_find_cpu(c); break;
			case 13: menu_save_computers(c, file_computers); break;
			case 14: menu_save_laptops(c, file_laptops); break;
			case 0:  break;
			default: cout << "Invalid choice." << endl;
		}
	}

	return 0;
}
