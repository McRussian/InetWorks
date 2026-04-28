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

#include "tablet.h"
#include "utils.h"

using namespace std;

Tablet::Tablet() {
	sim_cards = 0;
}

void Tablet::display() {
	Computer::display();
	cout << "   Sim cards: " << sim_cards << endl;
}

void Tablet::save_to_file(ofstream &stream) {
	stream << "tablet" << endl;
	stream << static_cast<Computer&>(*this);
	stream << "   Sim cards: " << this->sim_cards << endl;
}

void Tablet::read_from_file(ifstream &stream) {
	stream >> static_cast<Computer&>(*this);
	stream >> this->sim_cards;
	clear(stream);
}

void Tablet::read_from_stdin() {
	Computer::read_from_stdin();
	cout << "  Sim cards: ";
	cin >> sim_cards;
	clear(cin);
}
