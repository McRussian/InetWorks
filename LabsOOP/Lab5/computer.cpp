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

bool Computer::operator==(const Computer& c2) {
    if (cpu_brand != c2.cpu_brand) return false;
	if (harddrive != c2.harddrive) return false;
	if (videocard != c2.videocard) return false;
	if (wifi != c2.wifi) return false;
	if (motherboard != c2.motherboard) return false;
	return true;
}

// Class header:
// friend ostream& operator<<(ostream& stream, const Computer& c);
ostream &operator<<(ostream &stream, const Computer& c) {
	stream << "Процессор: " << c.cpu_brand << endl;
	stream << "   Частота: " << c.cpu_clock << endl;
	stream << "   Мат. плата: " << c.motherboard << endl;
	stream << "   HDD: " << c.harddrive << endl;
	stream << "   Video: " << c.videocard << endl;
	stream << "   Wifi: " << c.wifi << endl;
	stream << "   Цена: " << c.price << endl;
	stream << "   На складе: " << c.store << endl;
    return stream;
}

istream &operator>>(istream &stream, Computer& c) {
	getline(stream, c.cpu_brand);

	stream >> c.cpu_clock;
	clear(stream);

	getline(stream, c.motherboard);
	getline(stream, c.harddrive);
	getline(stream, c.videocard);

	char wifi = ' ';
	stream >> wifi;
	c.set_wifi(wifi == '+');
	clear(stream);

	stream >> c.price;
	clear(stream);

	stream >> c.store;
	clear(stream);
    return stream;
}

ifstream &operator>>(ifstream &stream, Computer& c) {
	getline(stream, c.cpu_brand);

	stream >> c.cpu_clock;
	clear(stream);

	getline(stream, c.motherboard);
	getline(stream, c.harddrive);
	getline(stream, c.videocard);

	char wifi = ' ';
	stream >> wifi;
	c.set_wifi(wifi == '+');
	clear(stream);

	stream >> c.price;
	clear(stream);

	stream >> c.store;
	clear(stream);
    return stream;
}

// Class header:
// friend ofstream& operator<<(ofstream& stream, const Computer& c);
ofstream &operator<<(ofstream &stream, const Computer& c) {
	stream << "Процессор: " << c.cpu_brand << endl;
	stream << "   Частота: " << c.cpu_clock << endl;
	stream << "   Мат. плата: " << c.motherboard << endl;
	stream << "   HDD: " << c.harddrive << endl;
	stream << "   Video: " << c.videocard << endl;
	stream << "   Wifi: " << c.wifi << endl;
	stream << "   Цена: " << c.price << endl;
	stream << "   На складе: " << c.store << endl;
    return stream;
}

// void Computer::read_from_file(ifstream &file) {
// 	if (!getline(file, this->cpu_brand)) {
// 		cout << "error read_from_file()" << endl;
// 	}
// 	file >> this->cpu_clock;
// 	clear(file);

// 	getline(file, this->motherboard);
// 	getline(file, this->harddrive);
// 	getline(file, this->videocard);
// 	char wifi = ' ';
// 	file >> wifi;
// 	clear(file);
// 	this->set_wifi(wifi == '+');

// 	file >> this->price;
// 	clear(file);

// 	file >> this->store;
// 	clear(file);
// }

// void Computer::print_computer() {
// 	cout << *this << endl;
// }

// void Computer::print_computer_file(ofstream &output) {
// 	output << *this << endl;
// }
