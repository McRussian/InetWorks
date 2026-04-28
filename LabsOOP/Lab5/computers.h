#ifndef COMPUTERS_H
#define COMPUTERS_H

#include "computer.h"

class Computers {
private:
	// std::vector<Computer> computers;
	Computer *data;
	int size;
	int capacity;

public:
    Computers(int capacity);  // Конструктор
    ~Computers(); // Деструктор

    void add_computer(const Computer& comp);
    void remove_computer(int i);
    void read_from_file(const std::string& filename);
    void save_to_file(const std::string& filename);
	void print();
	int total_price();
	int max_freq(int price_min, int price_max);
	void task1();
	void task2(int price_min, int price_max);
	int get_size();
	void edit(int i);
    // void printAll() const;
    // std::vector<Computer> searchByPriceRange(int minPrice, int maxPrice) const; // Поиск по цене
	void operator+=(const Computer &c);
};


#endif /* COMPUTERS_H */
