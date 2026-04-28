#ifndef COMPUTERS_H
#define COMPUTERS_H

#include "computer.h"

class Computers {
private:
	// std::vector<Computer> computers;
	Computer *data;
	int size;
	int capacity;

	Laptop *laptops;
	int size2;
	int capacity2;

public:
    Computers(int capacity);  // Конструктор
    ~Computers(); // Деструктор

    void add_computer(const Computer& comp);
    void remove_computer(int i);
    void remove_laptop(int i);
    void read_from_file(const std::string& filename);
	void read_from_file_laptops(const std::string& filename);
    void save_to_file(const std::string& filename);
	void save_to_file_laptops(const std::string& filename);
	void find_by_cpu(string cpu);
	int get_size() const;
	int get_size_laptops() const;
	void print_at_computer(int index);
	void print_at_laptop(int index);
	void edit_computer(int index);
	void edit_laptop(int index);
	int total_price();
	int max_freq();
	void task1();
	void task2();
	void task2(int pmin, int pmax);
	void operator+=(const Computer &c);
	void operator+=(const Laptop& c);
	friend ostream& operator<<(ostream& stream, const Computers& c);
};


#endif /* COMPUTERS_H */
