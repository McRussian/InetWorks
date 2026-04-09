#include <algorithm> // sort()
#include <fstream>   // ifstream, ofstream
#include <iostream>  // cin, cout
#include <map>
#include <memory>    // std::allocator<T>
#include <limits>    // std::numeric_limits
#include <queue>
#include <set>       // std::set
#include <sstream>   // std::istringstream
#include <stack>
#include <string>
#include <vector>

#include "utils.h"
#include "computer.h"

using namespace std;

void Computer::read_from_stdin() {
	cout << "  CPU brand: ";
	getline(cin, this->cpu_brand);
	cout << "  Clock speed (MHz): ";
	cin >> this->cpu_clock;
	clear();
	cout << "  Motherboard: ";
	getline(cin, this->motherboard);
	cout << "  HDD: ";
	getline(cin, this->harddrive);
	cout << "  Video card: ";
	getline(cin, this->videocard);
	cout << "  Has WiFi (+/-): ";
	char wifi = ' ';
	cin >> wifi;
	this->set_wifi(wifi == '+');
	cout << "  Price: ";
	cin >> this->price;
	clear();
	cout << "  In stock (pcs): ";
	cin >> this->store;
	clear();
}

void Computer::read_from_file(ifstream &file) {
	if (!getline(file, this->cpu_brand)) {
		cout << "error read_from_file()" << endl;
	}
	file >> this->cpu_clock;
	clearf(file);

	getline(file, this->motherboard);
	getline(file, this->harddrive);
	getline(file, this->videocard);
	char wifi = ' ';
	file >> wifi;
	clearf(file);
	// while (wifi != '+' && wifi != '-') {
	// 	file >> wifi;
	// 	file.get();
	// 	cout << "read wifi" << endl;
	// }
	this->set_wifi(wifi == '+');
	// if (wifi == '+') {
	// 	this->set_wifi(true);
	// }
	// else {
	// 	this->set_wifi(false);
	// }

	file >> this->price;
	clearf(file);

	file >> this->store;
	clearf(file);
}

void Computer::write_to_file(ofstream &output) {
	output << this->cpu_brand << "\n";
	output << this->cpu_clock << "\n";
	output << this->motherboard << "\n";
	output << this->harddrive << "\n";
	output << this->videocard << "\n";
	output << (this->wifi ? "+" : "-") << "\n";
	output << this->price << "\n";
	output << this->store << "\n";
}

void Computer::print_computer() {
	cout << "Процессор: " << this->cpu_brand << endl;
	cout << "   Частота: " << this->cpu_clock << endl;
	cout << "   Мат. плата: " << this->motherboard << endl;
	cout << "   HDD: " << this->harddrive << endl;
	cout << "   Video: " << this->videocard << endl;
	cout << "   Wifi: " << this->wifi << endl;
	cout << "   Цена: " << this->price << endl;
	cout << "   На складе: " << this->store << endl;
	cout << endl;
}

void Computer::print_computer_file(ofstream &output) {
	output << "Процессор: " << this->cpu_brand << endl;
	output << "   Частота: " << this->cpu_clock << endl;
	output << "   Мат. плата: " << this->motherboard << endl;
	output << "   HDD: " << this->harddrive << endl;
	output << "   Video: " << this->videocard << endl;
	output << "   Wifi: " << this->wifi << endl;
	output << "   Цена: " << this->price << endl;
	output << "   На складе: " << this->store << endl;
	output << endl;
}
