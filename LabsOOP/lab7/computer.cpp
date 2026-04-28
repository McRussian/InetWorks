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

Computer::Computer(string cpu, int clock, string mb, string hdd,
				   string video, bool wifi, int price, int store) {
	cpu_brand = cpu;
	cpu_clock = clock;
	motherboard = mb;
	harddrive = hdd;
	videocard = video;
	this->wifi = wifi;
	this->price = price;
	this->store = store;
	cout << "Конструктор Computer()" << endl;
};

bool Computer::operator==(const Computer& c2) {
    if (cpu_brand != c2.cpu_brand) return false;
	if (harddrive != c2.harddrive) return false;
	if (videocard != c2.videocard) return false;
	if (wifi != c2.wifi) return false;
	if (motherboard != c2.motherboard) return false;
	return true;
}

void Computer::display() {
	cout << "Процессор: " << cpu_brand << endl;
	cout << "   Частота: " << cpu_clock << endl;
	cout << "   Мат. плата: " << motherboard << endl;
	cout << "   HDD: " << harddrive << endl;
	cout << "   Video: " << videocard << endl;
	cout << "   Wifi: " << wifi << endl;
	cout << "   Цена: " << price << endl;
	cout << "   На складе: " << store << endl;
}

void Computer::save_to_file(ofstream &stream) {
	stream << "computer" << endl;
	stream << *this;
}

void Computer::read_from_file(ifstream &stream) {
	getline(stream, cpu_brand);

	stream >> cpu_clock;
	clear(stream);

	getline(stream, motherboard);
	getline(stream, harddrive);
	getline(stream, videocard);

	char wifi = ' ';
	stream >> wifi;
	set_wifi(wifi == '+');
	clear(stream);

	stream >> price;
	clear(stream);

	stream >> store;
	clear(stream);
}

bool operator==(const Laptop& c1, const Laptop& c2) {
	bool ok1 = static_cast<Computer>(c1) == static_cast<Computer>(c2);
	bool ok2 = c1.weight == c2.weight;
	bool ok3 = c1.screen_size == c2.screen_size;
	if (ok1 && ok2 && ok3) {
		return true;
	}
	return false;
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

istream& operator>>(istream& stream, Laptop& c) {
	stream >> static_cast<Computer&>(c);
	stream >> c.weight;
	clear(stream);
	stream >> c.screen_size;
	clear(stream);
    return stream;
}

ifstream &operator>>(ifstream &stream, Laptop &laptop) {
	stream >> static_cast<Computer&>(laptop);
	stream >> laptop.weight;
	clear(stream);
	stream >> laptop.screen_size;
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

void Laptop::display() {
	Computer::display();
	cout << "   Вес: " << weight << endl;
	cout << "   Экран: " << screen_size << endl;
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

ofstream &operator<<(ofstream &stream, const Laptop& c) {
	stream << static_cast<const Computer&>(c);
	stream << "   Вес: " << c.weight << endl;
	stream << "   Экран: " << c.screen_size << endl;
    return stream;
}

ostream &operator<<(ostream &stream, const Laptop& c) {
	stream << static_cast<const Computer&>(c);
	stream << "   Вес: " << c.weight << endl;
	stream << "   Экран: " << c.screen_size << endl;
    return stream;
}

Laptop::Laptop() {
	weight = 0;
	screen_size = 0;
}

Laptop::Laptop(string cpu, int clock, string mb, string hdd,
			   string video, bool wifi, int price, int store,
			   double weight, double screen_size):
				   Computer(cpu, clock, mb, hdd, video, wifi, price, store),
				   weight(weight), screen_size(screen_size) {
	cout << "Конструктор Laptop()" << endl;
}

void Laptop::save_to_file(ofstream &stream) {
	stream << "laptop" << endl;
	stream << static_cast<Computer&>(*this);
	// static_cast<Computer&>(*this).save_to_file(stream);
	stream << "   Вес: " << this->weight << endl;
	stream << "   Экран: " << this->screen_size << endl;
}

void Laptop::read_from_file(ifstream &stream) {
	stream >> static_cast<Computer&>(*this);
	stream >> this->weight;
	clear(stream);
	stream >> this->screen_size;
	clear(stream);
}

void Computer::read_from_stdin() {
	cout << "  CPU brand: ";
	getline(cin, cpu_brand);
	cout << "  Clock speed (MHz): ";
	cin >> cpu_clock;
	clear(cin);
	cout << "  Motherboard: ";
	getline(cin, motherboard);
	cout << "  HDD: ";
	getline(cin, harddrive);
	cout << "  Video card: ";
	getline(cin, videocard);
	cout << "  Has WiFi (+/-): ";
	char w;
	cin >> w;
	set_wifi(w == '+');
	clear(cin);
	cout << "  Price: ";
	cin >> price;
	clear(cin);
	cout << "  In stock: ";
	cin >> store;
	clear(cin);
}

void Laptop::read_from_stdin() {
	Computer::read_from_stdin();
	cout << "  Weight (kg): ";
	cin >> weight;
	clear(cin);
	cout << "  Screen size (inches): ";
	cin >> screen_size;
	clear(cin);
}
