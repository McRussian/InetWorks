#ifndef COMPUTERS_H
#define COMPUTERS_H

#include "computer.h"

class Computers {
private:
	// std::vector<Computer> computers;
	Computer **data;
	int size;
	int capacity;

public:
    Computers(int capacity);  // Конструктор
    ~Computers(); // Деструктор

    void read_from_file(const std::string& filename);
    void save_to_file(const std::string& filename);

	int get_size() const;
	void remove_at(int index);
	void print_at(int index);
	void edit_at(int index);
	int total_price();
	int max_freq();
	void task1();
	void task2();
	void task2(int pmin, int pmax);
	void operator+=(Computer *c);
	void display();
	void find_by_cpu(string cpu);
};


#endif /* COMPUTERS_H */
