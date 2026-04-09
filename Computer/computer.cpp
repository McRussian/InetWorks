#include "computer.h"
#include <iomanip>

// Функция считывания информации о компьютере из файлового потока
Computer read_computer(ifstream &file) {
    Computer pc;

    getline(file, pc.cpu_brand);
    file >> pc.cpu_clock;
    file.get(); // очистка буфера
    getline(file, pc.motherboard);
    getline(file, pc.harddrive);
    getline(file, pc.videocard);
    file >> pc.wifi;
    file >> pc.price;
    file >> pc.store;
    file.get(); // очистка буфера для следующей записи

    return pc;
}

// Функция чтения данных из файла
void read_data_from_file(Computer *&arr, int &count, const char *filename) {
    ifstream infile(filename);

    if (!infile.is_open()) {
        cout << "Ошибка открытия файла " << filename << "!" << endl;
        count = 0;
        arr = nullptr;
        return;
    }

    // Считываем количество записей
    infile >> count;
    infile.get(); // очистка буфера

    // Динамическое выделение памяти под массив структур
    arr = new Computer[count];

    // Считываем данные о компьютерах из файла
    for (int i = 0; i < count; i++) {
        arr[i] = read_computer(infile);
    }

    // Закрываем файл
    infile.close();

    cout << "Успешно загружено " << count << " записей из файла " << filename << endl;
}

// Функция вывода информации об одном компьютере
void print_computer(Computer pc) {
    cout << "\n==========================================" << endl;
    cout << "Марка процессора: " << pc.cpu_brand << endl;
    cout << "Тактовая частота: " << pc.cpu_clock << " ГГц" << endl;
    cout << "Марка мат. платы: " << pc.motherboard << endl;
    cout << "Марка HDD: " << pc.harddrive << endl;
    cout << "Марка видеокарты: " << pc.videocard << endl;
    cout << "Наличие WiFi: " << (pc.wifi ? "Да" : "Нет") << endl;
    cout << "Цена: " << pc.price << " руб." << endl;
    cout << "Количество на складе: " << pc.store << " шт." << endl;
    cout << "==========================================" << endl;
}

// Функция вывода всех компьютеров
void print_all_computers(Computer *arr, int count) {
    if (count == 0 || arr == nullptr) {
        cout << "\nНет данных для отображения!" << endl;
        return;
    }

    cout << "\n========== ВСЕ КОМПЬЮТЕРЫ ==========" << endl;
    for (int i = 0; i < count; i++) {
        cout << "\nКомпьютер #" << (i + 1);
        print_computer(arr[i]);
    }
    cout << "\nВсего записей: " << count << endl;
}

// Функция расчета общей стоимости всех компьютеров на складе
double calculate_total_cost(Computer *arr, int count) {
    if (count == 0 || arr == nullptr) return 0;

    double total = 0;
    for (int i = 0; i < count; i++) {
        total += arr[i].price * arr[i].store;
    }
    return total;
}

// Функция поиска компьютера с самой высокой тактовой частотой
// в ценовом диапазоне от 20000 до 30000 руб.
void find_highest_clock_in_price_range(Computer *arr, int count) {
    if (count == 0 || arr == nullptr) {
        cout << "\nНет данных для поиска!" << endl;
        return;
    }

    bool found = false;
    int max_index = -1;
    double max_clock = -1;

    for (int i = 0; i < count; i++) {
        // Проверяем, попадает ли компьютер в ценовой диапазон
        if (arr[i].price >= 20000 && arr[i].price <= 30000) {
            found = true;
            // Если частота больше текущей максимальной
            if (arr[i].cpu_clock > max_clock) {
                max_clock = arr[i].cpu_clock;
                max_index = i;
            }
        }
    }

    if (found && max_index != -1) {
        cout << "\n========== РЕЗУЛЬТАТ ПОИСКА ==========" << endl;
        cout << "Компьютер с максимальной тактовой частотой" << endl;
        cout << "в ценовом диапазоне 20000-30000 руб.:" << endl;
        print_computer(arr[max_index]);
    } else {
        cout << "\nКомпьютеров в ценовом диапазоне 20000-30000 руб. не найдено!" << endl;
    }
}
