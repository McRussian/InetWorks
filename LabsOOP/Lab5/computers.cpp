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

#include "computer.h"
#include "utils.h"
#include "computers.h"

using namespace std;

Computers::Computers(int capacity) {
	this->capacity = capacity;
	this->size = 0;
	this->data = new Computer[capacity];
}

Computers::~Computers() {
	this->capacity = 0;
	this->size = 0;
	delete [] this->data;
}

void Computers::operator+=(const Computer& c) {
	if (size < capacity) {
		data[size++] = c;
	}
}


void Computers::add_computer(const Computer& comp) {
	// if (size + 1 > capacity) {
	// 	capacity = capacity + 10;
	// 	auto newdata = new Computer[capacity];
	// 	std::copy(data, data + size, newdata);
	// 	if (data) {
	// 	    delete[] data;
	// 	}
	// 	data = newdata;
	// }
	if (size < capacity) {
		data[size++] = comp;
	}
}

void Computers::remove_computer(int index)
{
    if (index > size - 1 || index < 0) {
		cout << "Error removing" << endl;
	}
	for (int i = index; i < size - 1; ++i) {
	    data[i] = data[i + 1];
	}
	size--;
}

void Computers::read_from_file(const std::string& filename) {
	ifstream file;
	file.open(filename);
	if (!file.is_open()) {
		cout << "Error reading file" << endl;
		return;
	}
	int n;
	file >> n;
	clearf(file);
	for (int i = 0; i < n; ++i) {
		Computer c;
		// c.read_from_file(file);
		file >> c;
		// this->add_computer(c);
		*this += c;
	}
	file.close();
	return;
}

void Computers::save_to_file(const std::string& filename) {
	ofstream output;
	output.open(filename);
	if (!output.is_open()) {
		cout << "Output file error" << endl;
	}
	for (int i = 0; i < this->size; ++i) {
		output << this->data[i] << endl;
	}
	output.close();
	return;
}


int Computers::total_price() {
	int total = 0;
	for (int i = 0; i < this->size; ++i) {
		total += this->data[i].get_price() * this->data[i].get_store();
	}
	return total;
}

int Computers::max_freq(int price_min, int price_max) {
	int max_clock = -10000000;
	for (int i = 0; i < this->size; ++i) {
		if (this->data[i].get_price() >= price_min && this->data[i].get_price() <= price_max) {
			if (this->data[i].get_clock() > max_clock) {
				max_clock = this->data[i].get_clock();
			}
		}
	}
	return max_clock;
}

void Computers::print() {
	for (int i = 0; i < this->size; ++i) {
		// this->data[i].print_computer();
		cout << this->data[i] << endl;
	}
}

void Computers::task1() {
	// 1. total price
	int total = this->total_price();
	cout << "Общая стоимость всех компьютеров: " << total << "р" << endl;
	// output << "Общая стоимость всех компьютеров: " << total << "р" << endl;
}

void Computers::task2(int price_min, int price_max) {
	int max_clock = this->max_freq(price_min, price_max);
	if (max_clock == -10000000) {
		cout << "No computers in price range " << price_min << "-" << price_max << " rub!" << endl;
	} else {
		cout << "All PCs with the highest clock speed in price range " << price_min << "-" << price_max << ":" << endl;
		int k = 0;
		for (int i = 0; i < this->size; ++i) {
			if (this->data[i].get_price() >= price_min && this->data[i].get_price() <= price_max &&
			    this->data[i].get_clock() == max_clock) {
				k++;
				cout << k << "." << endl;
				cout << this->data[i];
			}
		}
	}
}

int Computers::get_size() {
	return this->size;
}

void Computers::edit(int index) {
	if (index < 0 || index >= this->size) {
		cout << "Invalid index." << endl;
		return;
	}
	cout << "Current data:" << endl;
	cout << data[index];
	cout << "Enter new data:" << endl;
	cin >> data[index];
}