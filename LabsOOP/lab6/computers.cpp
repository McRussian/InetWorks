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

	this->capacity2 = capacity;
	this->size2 = 0;
	this->laptops = new Laptop[capacity];
}

Computers::~Computers() {
	// this->capacity = 0;
	// this->size = 0;
	delete [] this->data;
	delete [] this->laptops;
}

void Computers::operator+=(const Computer& c) {
	if (size < capacity) {
		data[size++] = c;
	}
}

void Computers::operator+=(const Laptop& c) {
	if (size2 < capacity2) {
		laptops[size2++] = c;
	}
}

void Computers::remove_computer(int index) {
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
		exit(1);
	}
	int n;
	file >> n;
	clearf(file);
	for (int i = 0; i < n; ++i) {
		Computer c;
		file >> c;
		*this += c;
	}
	file.close();
	return;
}

void Computers::read_from_file_laptops(const std::string& filename) {
	ifstream file;
	file.open(filename);
	if (!file.is_open()) {
		cout << "Error reading file" << endl;
		exit(1);
	}
	int n;
	file >> n;
	clearf(file);
	for (int i = 0; i < n; ++i) {
		Laptop c;
		file >> c;
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
		exit(1);
	}
	for (int i = 0; i < this->size; ++i) {
		output << this->data[i] << endl;
	}
	output.close();
	return;
}

void Computers::find_by_cpu(string cpu) {
	cout << "Find CPU: " << cpu << " in " << size + size2 << " computers" << endl;
	for (int i = 0; i < size; ++i) {
		// cout << data[i].get_cpu_brand() << endl;
		if (data[i].get_cpu_brand() == cpu) {
			cout << this->data[i] << endl;
		}
	}
	for (int i = 0; i < size2; ++i) {
		// cout << laptops[i].get_cpu_brand() << endl;
		if (laptops[i].get_cpu_brand() == cpu) {
			cout << this->laptops[i] << endl;
		}
	}
	cout << "---Find finished---" << endl;
}

void Computers::save_to_file_laptops(const std::string& filename) {
	ofstream output;
	output.open(filename);
	if (!output.is_open()) {
		cout << "Output file error" << endl;
		exit(1);
	}
	for (int i = 0; i < this->size2; ++i) {
		output << this->laptops[i] << endl;
	}
	output.close();
	return;
}


int Computers::total_price() {
	int total = 0;
	for (int i = 0; i < this->size; ++i) {
		total += this->data[i].get_price() * this->data[i].get_store();
	}
	for (int i = 0; i < this->size2; ++i) {
		total += this->laptops[i].get_price() * this->laptops[i].get_store();
	}
	return total;
}

int Computers::max_freq() {
	int max_clock = -10000000;
	for (int i = 0; i < this->size; ++i) {
		if (this->data[i].get_price() >= 20000 && this->data[i].get_price() <= 30000) {
			if (this->data[i].get_clock() > max_clock) {
				max_clock = this->data[i].get_clock();
			}
		}
	}
	for (int i = 0; i < this->size2; ++i) {
		if (this->laptops[i].get_price() >= 20000 && this->laptops[i].get_price() <= 30000) {
			if (this->laptops[i].get_clock() > max_clock) {
				max_clock = this->laptops[i].get_clock();
			}
		}
	}
	return max_clock;
}

ostream &operator<<(ostream &stream, const Computers& c) {
	cout << "Regular computers " << c.size << endl;
	for (int i = 0; i < c.size; ++i) {
		cout << c.data[i] << endl;
	}
	cout << "Laptops: " << c.size2 << endl;
	for (int i = 0; i < c.size2; ++i) {
		cout << c.laptops[i] << endl;
	}
    return stream;
}

// void Computers::print() {
// 	for (int i = 0; i < this->size; ++i) {
// 		// this->data[i].print_computer();
// 		cout << this->data[i] << endl;
// 	}
// }

void Computers::task1() {
	// 1. total price
	int total = this->total_price();
	cout << "Общая стоимость всех компьютеров: " << total << "р" << endl;
	// output << "Общая стоимость всех компьютеров: " << total << "р" << endl;
}

int Computers::get_size() const { return size; }
int Computers::get_size_laptops() const { return size2; }

void Computers::print_at_computer(int index) {
	if (index >= 0 && index < size)
		cout << data[index] << endl;
}

void Computers::print_at_laptop(int index) {
	if (index >= 0 && index < size2)
		cout << laptops[index] << endl;
}

void Computers::remove_laptop(int index) {
	if (index < 0 || index >= size2) {
		cout << "Error removing" << endl;
		return;
	}
	for (int i = index; i < size2 - 1; ++i)
		laptops[i] = laptops[i + 1];
	size2--;
}

void Computers::edit_computer(int index) {
	if (index < 0 || index >= size) {
		cout << "Invalid index." << endl;
		return;
	}
	cout << "Current data:" << endl;
	cout << data[index] << endl;
	cout << "Enter new data:" << endl;
	data[index].read_from_stdin();
}

void Computers::edit_laptop(int index) {
	if (index < 0 || index >= size2) {
		cout << "Invalid index." << endl;
		return;
	}
	cout << "Current data:" << endl;
	cout << laptops[index] << endl;
	cout << "Enter new data:" << endl;
	laptops[index].read_from_stdin();
}

void Computers::task2(int pmin, int pmax) {
	int max_clock = -10000000;
	for (int i = 0; i < size; ++i)
		if (data[i].get_price() >= pmin && data[i].get_price() <= pmax)
			if (data[i].get_clock() > max_clock)
				max_clock = data[i].get_clock();
	for (int i = 0; i < size2; ++i)
		if (laptops[i].get_price() >= pmin && laptops[i].get_price() <= pmax)
			if (laptops[i].get_clock() > max_clock)
				max_clock = laptops[i].get_clock();
	if (max_clock == -10000000) {
		cout << "No records in price range " << pmin << "-" << pmax << " rub!" << endl;
		return;
	}
	cout << "All PCs with max clock speed in range " << pmin << "-" << pmax << ":" << endl;
	int k = 0;
	for (int i = 0; i < size; ++i)
		if (data[i].get_price() >= pmin && data[i].get_price() <= pmax && data[i].get_clock() == max_clock) {
			cout << ++k << "." << endl;
			cout << data[i] << endl;
		}
	for (int i = 0; i < size2; ++i)
		if (laptops[i].get_price() >= pmin && laptops[i].get_price() <= pmax && laptops[i].get_clock() == max_clock) {
			cout << ++k << "." << endl;
			cout << laptops[i] << endl;
		}
}

void Computers::task2() {
	// 2. ПК с самой высокой тактовой частотой с ценой 20000-30000
	int max_clock = this->max_freq();
	if (max_clock == -10000000) {
		cout << "Нет компьютеров в диапазоне цен 20000-30000р!" << endl;
		// output << "Нет компьютеров в диапазоне цен 20000-30000р!" << endl;
	}
	else {
		cout << "Все ПК с самой высокой тактовой частотой с ценой 20000-30000:" << endl;
		// output << "Все ПК с самой высокой тактовой частотой с ценой 20000-30000:" << endl;
		// print_all_needed_pc(a, n, max_clock, output);
		int k = 0;
		for (int i = 0; i < this->size; ++i) {
			if (data[i].get_price() >= 20000 && data[i].get_price() <= 30000 && data[i].get_clock() == max_clock) {
				k++;
				cout << k << endl;
				// this->data[i].print_computer();
				cout << data[i] << endl;
			}
		}
		for (int i = 0; i < this->size2; ++i) {
			if (laptops[i].get_price() >= 20000 && laptops[i].get_price() <= 30000 && laptops[i].get_clock() == max_clock) {
				k++;
				cout << k << endl;
				cout << laptops[i] << endl;
			}
		}
	}
}
