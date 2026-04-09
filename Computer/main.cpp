#include <iostream>
#include <iomanip>

#include "computer.h"

using namespace std;

int main() {
    int N = 0;                      // Количество компьютеров
    Computer *collection = nullptr;  // Указатель для массива структур

    // Чтение данных из файла с помощью отдельной функции
    read_data_from_file(collection, N, "computers.txt");

    // Проверка, успешно ли загружены данные
    if (N == 0 || collection == nullptr) {
        cout << "Не удалось загрузить данные. Программа завершена." << endl;
        system("pause");
        return 1;
    }

    // Выводим информацию обо всех компьютерах
    print_all_computers(collection, N);

    // Задание 1: Рассчитать общую стоимость всех компьютеров на складе
    double total_cost = calculate_total_cost(collection, N);
    cout << "\n========== ЗАДАНИЕ 1 ==========" << endl;
    cout << fixed << setprecision(2);
    cout << "Общая стоимость всех компьютеров на складе: "
         << total_cost << " руб." << endl;

    // Задание 2: Поиск компьютера с максимальной частотой в ценовом диапазоне
    cout << "\n========== ЗАДАНИЕ 2 ==========" << endl;
    find_highest_clock_in_price_range(collection, N);

    // Освобождаем выделенную память
    delete[] collection;

    cout << "\n\n";
    return 0;
}
