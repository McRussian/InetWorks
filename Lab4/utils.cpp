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

using namespace std;

void clear() {
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	//getchar();
	// char ch;
	// int i = 0;
	// while ((ch = getchar()) != '\n' && ch != EOF) i++;
}

void clearf(ifstream &f) {
	f.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	//getchar();
	// char ch;
	// int i = 0;
	// while ((ch = getchar()) != '\n' && ch != EOF) i++;
}
