// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <cmath>
#include <iostream>  // cin, cout
#include <fstream>
#include <sstream>
#include <string>
#include <limits>    // std::numeric_limits

#define MAX_STR_LEN 50

using namespace std;

class Computer {
private:
    string cpu_brand;
    double cpu_clock;
    string motherboard;
    string harddrive;
    string videocard;
    bool has_wifi;
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

    double get_clock() {
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

    void set_cpu_clock(double new_clock) {
        this->cpu_clock = new_clock;
    }

    void set_hdd(string new_hdd) {
        this->harddrive = new_hdd;
    }

    void set_video(string new_video) {
        this->videocard = new_video;
    }

    void set_wifi(bool wifi) {
        this->has_wifi = wifi;
    }

    void set_price(int price) {
        this->price = price;
    }

    void set_store(int store) {
        this->store = store;
    }

    void read_from_stdin();
    void read_from_stream(istream &in);
    void write_to_stream(ostream &out);
    void print_computer();
    void print_to_stream(ostream &out);
};

void clear(istream &stream) {
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Computer::read_from_stdin() {
    cout << "  CPU brand: ";
    getline(cin, this->cpu_brand);
    cout << "  Clock speed: ";
    cin >> this->cpu_clock;
    clear(cin);
    cout << "  Motherboard: ";
    getline(cin, this->motherboard);
    cout << "  HDD: ";
    getline(cin, this->harddrive);
    cout << "  Video card: ";
    getline(cin, this->videocard);
    cout << "  Has WiFi (+/-): ";
    char wifi = ' ';
    cin >> wifi;
    this->set_wifi(wifi == '+');
    cout << "  Price: ";
    cin >> this->price;
    cout << "  In stock (pcs): ";
    cin >> this->store;
    clear(cin);
}

void Computer::read_from_stream(istream &in) {
    if (!getline(in, this->cpu_brand)) {
        cout << "Stream read error" << endl;
        exit(1);
    }
    in >> this->cpu_clock;
    clear(in);

    getline(in, this->motherboard);
    getline(in, this->harddrive);
    getline(in, this->videocard);
    char wifi = ' ';
    in >> wifi;
    clear(in);
    this->set_wifi(wifi == '+');

    in >> this->price;
    clear(in);

    in >> this->store;
    clear(in);
}

void Computer::write_to_stream(ostream &out) {
    out << this->cpu_brand << "\n";
    out << this->cpu_clock << "\n";
    out << this->motherboard << "\n";
    out << this->harddrive << "\n";
    out << this->videocard << "\n";
    out << (this->has_wifi ? "+" : "-") << "\n";
    out << this->price << "\n";
    out << this->store << "\n";
}

void Computer::print_to_stream(ostream &out) {
    out << "CPU: " << this->cpu_brand << "\n";
    out << "   Clock: " << this->cpu_clock << "\n";
    out << "   Motherboard: " << this->motherboard << "\n";
    out << "   HDD: " << this->harddrive << "\n";
    out << "   Video: " << this->videocard << "\n";
    out << "   Has WiFi: " << (this->has_wifi ? "yes" : "no") << "\n";
    out << "   Price: " << this->price << "\n";
    out << "   In stock: " << this->store << "\n";
    out << "\n";
}

void Computer::print_computer() {
    this->print_to_stream(cout);
}


// --- Вычисления ---

int total_price(Computer *a, int n) {
    int total = 0;
    for (int i = 0; i < n; ++i) {
        total += a[i].get_price() * a[i].get_store();
    }
    return total;
}

double max_freq(Computer* a, int n, int price_min, int price_max) {
    double max_clock = -10000000;
    for (int i = 0; i < n; ++i) {
        if (a[i].get_price() >= price_min && a[i].get_price() <= price_max) {
            if (a[i].get_clock() > max_clock) {
                max_clock = a[i].get_clock();
            }
        }
    }
    return max_clock;
}

void print_all_needed_pc(Computer* a, int n, double max_clock, int price_min, int price_max) {
    int k = 0;
    for (int i = 0; i < n; ++i) {
        if (a[i].get_price() >= price_min && a[i].get_price() <= price_max &&
            fabs(a[i].get_clock() - max_clock) < 0.0001) {
            k++;
            cout << k << "." << endl;
            a[i].print_computer();
        }
    }
}

// --- Пункты меню ---

void menu_read_file(Computer* &a, int &n, string &filename) {
    cout << "File name: ";
    getline(cin, filename);

    ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        cout << "Failed to open file!" << endl;
        filename = "";
        return;
    }

    // читаем весь файл в stringstream, затем разбираем
    ostringstream buf;
    buf << file.rdbuf();
    file.close();
    istringstream ss(buf.str());

    if (a != nullptr) {
        delete[] a;
        a = nullptr;
        n = 0;
    }

    ss >> n;
    clear(ss);

    a = new Computer[n];
    for (int i = 0; i < n; ++i) {
        a[i].read_from_stream(ss);
    }

    cout << "Records read: " << n << endl;
}

void menu_print_all(Computer* a, int n) {
    if (n == 0) {
        cout << "No data." << endl;
        return;
    }
    for (int i = 0; i < n; ++i) {
        cout << i + 1 << "." << endl;
        a[i].print_computer();
    }
}

void menu_add(Computer* &a, int &n) {
    Computer* new_a = new Computer[n + 1];
    for (int i = 0; i < n; ++i) {
        new_a[i] = a[i];
    }
    cout << "Enter new record data:" << endl;
    new_a[n].read_from_stdin();
    delete[] a;
    a = new_a;
    n++;
    cout << "Record added. Total records: " << n << endl;
}

void menu_edit(Computer* a, int n) {
    if (n == 0) {
        cout << "No data." << endl;
        return;
    }
    cout << "Record number to edit (1-" << n << "): ";
    int idx;
    cin >> idx;
    clear(cin);
    if (idx < 1 || idx > n) {
        cout << "Invalid number." << endl;
        return;
    }
    cout << "Current data:" << endl;
    a[idx - 1].print_computer();
    cout << "Enter new data:" << endl;
    a[idx - 1].read_from_stdin();
    cout << "Record updated." << endl;
}

void menu_delete(Computer* &a, int &n) {
    if (n == 0) {
        cout << "No data." << endl;
        return;
    }
    cout << "Record number to delete (1-" << n << "): ";
    int idx;
    cin >> idx;
    clear(cin);
    if (idx < 1 || idx > n) {
        cout << "Invalid number." << endl;
        return;
    }
    Computer* new_a = new Computer[n - 1];
    for (int i = 0, j = 0; i < n; ++i) {
        if (i != idx - 1) {
            new_a[j++] = a[i];
        }
    }
    delete[] a;
    a = new_a;
    n--;
    cout << "Record deleted. Total records: " << n << endl;
}

void menu_total_price(Computer* a, int n) {
    if (n == 0) {
        cout << "No data." << endl;
        return;
    }
    int total = total_price(a, n);
    cout << "Total cost of all computers: " << total << endl;
}

void menu_filter_price(Computer* a, int n) {
    if (n == 0) {
        cout << "No data." << endl;
        return;
    }
    int price_min, price_max;
    cout << "Min price: ";
    cin >> price_min;
    cout << "Max price: ";
    cin >> price_max;
    clear(cin);

    double mc = max_freq(a, n, price_min, price_max);
    if (mc == -10000000) {
        cout << "No computers in price range " << price_min << "-" << price_max << endl;
        return;
    }
    cout << "PCs with highest clock speed in price range "
         << price_min << "-" << price_max << ":" << endl;
    print_all_needed_pc(a, n, mc, price_min, price_max);
}

void menu_save_file(Computer* a, int n, const string &filename) {
    string fname = filename;
    if (fname.empty()) {
        cout << "Enter file name: ";
        getline(cin, fname);
    }

    // собираем данные в stringstream, затем пишем в файл
    ostringstream ss;
    ss << n << "\n";
    for (int i = 0; i < n; ++i) {
        a[i].write_to_stream(ss);
    }

    ofstream file;
    file.open(fname);
    if (!file.is_open()) {
        cout << "Failed to open file!" << endl;
        return;
    }
    file << ss.str();
    file.close();
    cout << "Saved to " << fname << endl;
}

int main()
{
    Computer *a = nullptr;
    int n = 0;
    string filename = "";

    int choice = -1;
    while (choice != 0) {
        cout << "\n=== Menu ===" << endl;
        cout << "1. Read from file" << endl;
        cout << "2. Print all records" << endl;
        cout << "3. Add record" << endl;
        cout << "4. Edit record" << endl;
        cout << "5. Delete record" << endl;
        cout << "6. Total cost" << endl;
        cout << "7. Find PC with max clock speed by price range" << endl;
        cout << "8. Save to file" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;
        clear(cin);

        switch (choice) {
            case 1: menu_read_file(a, n, filename); break;
            case 2: menu_print_all(a, n); break;
            case 3: menu_add(a, n); break;
            case 4: menu_edit(a, n); break;
            case 5: menu_delete(a, n); break;
            case 6: menu_total_price(a, n); break;
            case 7: menu_filter_price(a, n); break;
            case 8: menu_save_file(a, n, filename); break;
            case 0: break;
            default: cout << "Invalid choice." << endl;
        }
    }

    if (a != nullptr) delete[] a;
    return 0;
}
