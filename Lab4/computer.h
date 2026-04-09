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
	// getter
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

	void read_from_stdin();
	void read_from_file(ifstream &file);
	void write_to_file(ofstream &output);
	void print_computer();
	void print_computer_file(ofstream &output);
};

#endif /* COMPUTER_H */
