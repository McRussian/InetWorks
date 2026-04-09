#ifdef _WIN32
#include <windows.h>
#endif

#include <clocale>
#include <fstream>
#include <iostream>
#include <string>

#include "computer.h"
#include "computers.h"
#include "utils.h"

using namespace std;

void menu_read_file(Computers &c, string &filename) {
	cout << "Имя файла: ";
	getline(cin, filename);
	c.read_from_file(filename);
	cout << "Записей прочитано: " << c.get_size() << endl;
}

void menu_print_all(Computers &c) {
	if (c.get_size() == 0) {
		cout << "Нет данных." << endl;
		return;
	}
	for (int i = 0; i < c.get_size(); ++i) {
		cout << i + 1 << "." << endl;
		c.print_at(i);
	}
}

void menu_add(Computers &c) {
	cout << "Введите данные новой записи:" << endl;
	Computer comp;
	comp.read_from_stdin();
	c.add_computer(comp);
	cout << "Запись добавлена. Всего записей: " << c.get_size() << endl;
}

void menu_edit(Computers &c) {
	int n = c.get_size();
	if (n == 0) {
		cout << "Нет данных." << endl;
		return;
	}
	cout << "Номер записи для редактирования (1-" << n << "): ";
	int idx;
	cin >> idx;
	clear();
	if (idx < 1 || idx > n) {
		cout << "Неверный номер." << endl;
		return;
	}
	c.edit_computer(idx - 1);
	cout << "Запись обновлена." << endl;
}

void menu_delete(Computers &c) {
	int n = c.get_size();
	if (n == 0) {
		cout << "Нет данных." << endl;
		return;
	}
	cout << "Номер записи для удаления (1-" << n << "): ";
	int idx;
	cin >> idx;
	clear();
	if (idx < 1 || idx > n) {
		cout << "Неверный номер." << endl;
		return;
	}
	c.remove_computer(idx - 1);
	cout << "Запись удалена. Всего записей: " << c.get_size() << endl;
}

void menu_total_price(Computers &c) {
	if (c.get_size() == 0) {
		cout << "Нет данных." << endl;
		return;
	}
	c.task1();
}

void menu_filter_price(Computers &c) {
	if (c.get_size() == 0) {
		cout << "Нет данных." << endl;
		return;
	}
	int price_min, price_max;
	cout << "Мин. цена: ";
	cin >> price_min;
	cout << "Макс. цена: ";
	cin >> price_max;
	clear();
	c.task2(price_min, price_max);
}

void menu_save_file(Computers &c, const string &filename) {
	string fname = filename;
	if (fname.empty()) {
		cout << "Введите имя файла: ";
		getline(cin, fname);
	}
	c.save_to_file(fname);
	cout << "Сохранено в " << fname << endl;
}

int main() {
#ifdef _WIN32
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
#endif
	setlocale(LC_ALL, "");

	Computers c(100);
	string filename = "";

	int choice = -1;
	while (choice != 0) {
		cout << "\n=== Меню ===" << endl;
		cout << "1. Прочитать из файла" << endl;
		cout << "2. Показать все записи" << endl;
		cout << "3. Добавить запись" << endl;
		cout << "4. Редактировать запись" << endl;
		cout << "5. Удалить запись" << endl;
		cout << "6. Общая стоимость" << endl;
		cout << "7. ПК с макс. частотой в диапазоне цен" << endl;
		cout << "8. Сохранить в файл" << endl;
		cout << "0. Выход" << endl;
		cout << "Выбор: ";
		cin >> choice;
		clear();

		switch (choice) {
			case 1: menu_read_file(c, filename); break;
			case 2: menu_print_all(c); break;
			case 3: menu_add(c); break;
			case 4: menu_edit(c); break;
			case 5: menu_delete(c); break;
			case 6: menu_total_price(c); break;
			case 7: menu_filter_price(c); break;
			case 8: menu_save_file(c, filename); break;
			case 0: break;
			default: cout << "Неверный выбор." << endl;
		}
	}

	return 0;
}
