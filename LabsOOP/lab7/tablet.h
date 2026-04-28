#ifndef TABLET_H
#define TABLET_H

#include "computer.h"

class Tablet: public Computer {
private:
	int sim_cards;
public:
	Tablet();  // Конструктор по умолчанию
	// Laptop(string cpu, int clock, string mb, string hdd,
	// 	   string video, bool wifi, int price, int store,
	// 	   double weight, double screen_size);
	// friend bool operator==(const Laptop& c1, const Laptop& c2);
	// friend istream &operator>>(istream &stream, Laptop& c);
	// friend ifstream &operator>>(ifstream &stream, Laptop& c);
	virtual void display();
	virtual void save_to_file(ofstream &stream);
	virtual void read_from_file(ifstream &stream);
	virtual void read_from_stdin();
};

#endif /* TABLET_H */
