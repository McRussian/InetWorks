#ifndef COMPUTER_H
#define COMPUTER_H

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

// Определение структуры "Персональный компьютер"
struct Computer {
    string cpu_brand;        // Марка процессора
    double cpu_clock;         // Тактовая частота процессора (ГГц)
    string motherboard;       // Марка материнской платы
    string harddrive;         // Марка жесткого диска
    string videocard;         // Марка видеокарты
    bool wifi;                // Наличие модуля wifi (1 - есть, 0 - нет)
    int price;                // Цена в рублях
    int store;                // Количество на складе
};

// Прототипы функций
void read_data_from_file(Computer *&arr, int &count, const char *filename);
Computer read_computer(ifstream &file);
void print_computer(Computer pc);
void print_all_computers(Computer *arr, int count);
double calculate_total_cost(Computer *arr, int count);
void find_highest_clock_in_price_range(Computer *arr, int count);

#endif
