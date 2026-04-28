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
#include "tablet.h"
#include "utils.h"
#include "computers.h"

using namespace std;

Computers::Computers(int capacity) {
	this->capacity = capacity;
	this->size = 0;
	this->data = new Computer*[capacity];
}

Computers::~Computers() {
	for (int i = 0; i < size; ++i)
		delete data[i];
	delete [] this->data;
}

void Computers::operator+=(Computer* c) {
	if (size < capacity) {
		data[size++] = c;
	}
}

void Computers::read_from_file(const std::string& filename) {
	ifstream stream;
	stream.open(filename);
	if (!stream.is_open()) {
		cout << "Error reading file" << endl;
		exit(1);
	}
	int n;
	stream >> n;
	clearf(stream);
	cout << n << endl;
	for (int i = 0; i < n; ++i) {
		string t; // computer type (computer, laptop, tablet)
		getline(stream, t);
		if (t == "computer") {
			Computer *c = new Computer;
			c->read_from_file(stream);
			*this += c;
		} else if (t == "laptop") {
			Laptop *c = new Laptop;
			c->read_from_file(stream);
			*this += c;
		} else if (t == "tablet") {
			Tablet *c = new Tablet;
			c->read_from_file(stream);
			*this += c;
		} else {
			cout <<" Error: unknown PC type '" << t << "'" << endl;
			exit(1);
		}
	}
	stream.close();
	return;
}

void Computers::save_to_file(const std::string& filename) {
	ofstream output;
	output.open(filename);
	if (!output.is_open()) {
		cout << "Output file error" << endl;
		exit(1);
	}
	output << size << "\n";
	for (int i = 0; i < size; ++i) {
		data[i]->save_to_file(output);
	}
	output.close();
	return;
}

int Computers::total_price() {
	int total = 0;
	for (int i = 0; i < size; ++i) {
		total += data[i]->get_price() * data[i]->get_store();
	}
	return total;
}

int Computers::max_freq() {
	int max_clock = -10000000;
	for (int i = 0; i < this->size; ++i) {
		if (data[i]->get_price() >= 20000 && data[i]->get_price() <= 30000) {
			if (data[i]->get_clock() > max_clock) {
				max_clock = data[i]->get_clock();
			}
		}
	}
	return max_clock;
}

void Computers::display() {
	cout << "Total: " << size << endl;
	for (int i = 0; i < size; ++i) {
		data[i]->display();
	}
}

void Computers::find_by_cpu(string cpu) {
	cout << "Find CPU: " << cpu << " in " << size << " computers" << endl;
	for (int i = 0; i < size; ++i) {
		if (data[i]->get_cpu_brand() == cpu) {
			data[i]->display();
		}
	}
	cout << "---Find finished---" << endl;
}

void Computers::task1() {
	// 1. total price
	int total = this->total_price();
	cout << "Общая стоимость всех компьютеров: " << total << "р" << endl;
}

int Computers::get_size() const { return size; }

void Computers::remove_at(int index) {
	if (index < 0 || index >= size) {
		cout << "Invalid index." << endl;
		return;
	}
	delete data[index];
	for (int i = index; i < size - 1; ++i)
		data[i] = data[i + 1];
	size--;
}

void Computers::print_at(int index) {
	if (index >= 0 && index < size)
		data[index]->display();
}

void Computers::edit_at(int index) {
	if (index < 0 || index >= size) {
		cout << "Invalid index." << endl;
		return;
	}
	cout << "Current data:" << endl;
	data[index]->display();
	cout << "Enter new data:" << endl;
	data[index]->read_from_stdin();
}

void Computers::task2(int pmin, int pmax) {
	int max_clock = -10000000;
	for (int i = 0; i < size; ++i)
		if (data[i]->get_price() >= pmin && data[i]->get_price() <= pmax)
			if (data[i]->get_clock() > max_clock)
				max_clock = data[i]->get_clock();
	if (max_clock == -10000000) {
		cout << "No records in price range " << pmin << "-" << pmax << " rub!" << endl;
		return;
	}
	cout << "All PCs with max clock speed in range " << pmin << "-" << pmax << ":" << endl;
	int k = 0;
	for (int i = 0; i < size; ++i)
		if (data[i]->get_price() >= pmin && data[i]->get_price() <= pmax && data[i]->get_clock() == max_clock) {
			cout << ++k << "." << endl;
			data[i]->display();
		}
}

void Computers::task2() {
	// 2. ПК с самой высокой тактовой частотой с ценой 20000-30000
	int max_clock = this->max_freq();
	if (max_clock == -10000000) {
		cout << "Нет компьютеров в диапазоне цен 20000-30000р!" << endl;
	}
	else {
		cout << "Все ПК с самой высокой тактовой частотой с ценой 20000-30000:" << endl;
		int k = 0;
		for (int i = 0; i < this->size; ++i) {
			if (data[i]->get_price() >= 20000 && data[i]->get_price() <= 30000 && data[i]->get_clock() == max_clock) {
				k++;
				cout << k << endl;
				data[i]->display();
			}
		}
	}
}
