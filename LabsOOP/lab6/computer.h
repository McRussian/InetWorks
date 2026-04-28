#ifndef COMPUTER_H
#define COMPUTER_H

#include <vector>
#include <fstream>
#include <string>

using namespace std;

class Computer {
private:
	string cpu_brand;
	int cpu_clock;
	string motherboard;
	string harddrive;
	string videocard;
	bool wifi;
	int price;
	int store;
public:
	Computer(): cpu_brand(""), cpu_clock(3000), motherboard(""), harddrive(""),
				videocard(""), wifi(false), price(0), store(0) {};
	Computer(string cpu, int clock, string mb, string hdd,
			 string video, bool wifi, int price, int store);

	// getter
	string get_cpu_brand() {
		return this->cpu_brand;
	}

	string get_motherboard() {
		return this->motherboard;
	}

	int get_price() {
		return this->price;
	}

	int get_clock() {
		return this->cpu_clock;
	}

	int get_store() {
		return this->store;
	}

	// setter
	void set_motherboard(string new_mb) {
		this->motherboard = new_mb;
	}

	void set_cpu_brand(string new_cpu) {
		this->cpu_brand = new_cpu;
	}

	void set_cpu_clock(int new_clock) {
		this->cpu_clock = new_clock;
	}

	void set_hdd(string new_hdd) {
		this->harddrive = new_hdd;
	}

	void set_video(string new_video) {
		this->videocard = new_video;
	}

	void set_wifi(bool wifi) {
		this->wifi = wifi;
	}

	void set_price(int price) {
		this->price = price;
	}

	void set_store(int store) {
		this->store = store;
	}

	bool operator==(const Computer& c2);
	friend istream &operator>>(istream &stream, Computer& c);
	friend ifstream &operator>>(ifstream &stream, Computer& c);
	friend ostream& operator<<(ostream& stream, const Computer& c);
	friend ofstream& operator<<(ofstream& stream, const Computer& c);
	void read_from_stdin();
};

class Laptop: public Computer {
private:
	double weight;  // вес в кг
	double screen_size;  // размер экрана в дюймах
public:
	Laptop();  // Конструктор по умолчанию
	Laptop(string cpu, int clock, string mb, string hdd,
		   string video, bool wifi, int price, int store,
		   double weight, double screen_size);
	friend bool operator==(const Laptop& c1, const Laptop& c2);
	friend istream &operator>>(istream &stream, Laptop& c);
	friend ifstream &operator>>(ifstream &stream, Laptop& c);
	friend ostream& operator<<(ostream& stream, const Laptop& c);
	friend ofstream& operator<<(ofstream& stream, const Laptop& c);
	void read_from_stdin();
};

#endif /* COMPUTER_H */
