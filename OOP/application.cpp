#include "application.h"
#include <iostream>
#include <limits>
#include <string>

using namespace std;

// Конструктор - создаёт пустой массив
ConsoleApplication::ConsoleApplication() : array(), running(true) {}

// Показать меню
void ConsoleApplication::showMenu() {
    cout << "\n========== МЕНЮ ПРИЛОЖЕНИЯ ==========\n";
    cout << "1. Ввести новый массив\n";
    cout << "2. Вычислить среднее и СКО\n";
    cout << "3. Сортировать массив\n";
    cout << "4. Изменить вместимость (capacity) массива\n";
    cout << "5. Добавить элемент в конец\n";
    cout << "6. Удалить элемент с конца\n";
    cout << "7. Изменить значение элемента\n";
    cout << "8. Вывести информацию о массиве\n";
    cout << "0. Выход\n";
    cout << "====================================\n";
    cout << "Выберите команду: ";
}

// Ввод нового массива
void ConsoleApplication::handleInput() {
    cout << "Введите размер массива и элементы (например: 3 1.5 2.7 3.14): ";

    // Очищаем поток ввода
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Используем перегруженный оператор >>
    cin >> array;

    if (cin.fail()) {
        cout << "Ошибка ввода! Создан пустой массив.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        array = Array();  // Пустой массив
    } else {
        cout << "Массив успешно создан: " << array << endl;
        cout << "Вместимость (capacity): " << array.getCapacity() << endl;
        cout << "Количество элементов (size): " << array.getSize() << endl;
    }
}

// Вычисление статистики
void ConsoleApplication::handleCalculate() {
    if (array.isEmpty()) {
        cout << "Массив пуст! Невозможно вычислить статистику.\n";
        return;
    }

    number avg = array.mean();
    number stddev = array.stdDeviation();

    cout << "Текущий массив: " << array << endl;
    cout << "Среднее арифметическое: " << avg << endl;
    cout << "Среднеквадратичное отклонение: " << stddev << endl;
}

// Сортировка
void ConsoleApplication::handleSort() {
    if (array.isEmpty()) {
        cout << "Массив пуст! Нечего сортировать.\n";
        return;
    }

    cout << "Текущий массив: " << array << endl;
    cout << "Выберите направление сортировки:\n";
    cout << "1. По возрастанию\n";
    cout << "2. По убыванию\n";
    cout << "Ваш выбор: ";

    int choice;
    cin >> choice;

    switch (choice) {
    case 1:
        array.sortAscending();
        cout << "Массив отсортирован по возрастанию: " << array << endl;
        break;
    case 2:
        array.sortDescending();
        cout << "Массив отсортирован по убыванию: " << array << endl;
        break;
    default:
        cout << "Неверный выбор! Сортировка отменена.\n";
    }
}

// Изменение capacity (резервирование памяти)
void ConsoleApplication::handleReserve() {
    cout << "Текущая вместимость (capacity): " << array.getCapacity() << endl;
    cout << "Текущее количество элементов (size): " << array.getSize() << endl;
    cout << "Текущий массив: " << array << endl;

    cout << "Введите новую вместимость (не меньше " << array.getSize() << "): ";
    size_t newCapacity;
    cin >> newCapacity;

    if (cin.fail()) {
        cout << "Ошибка ввода!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    try {
        array.reserve(newCapacity);
        cout << "Вместимость изменена. Новая capacity: " << array.getCapacity() << endl;
        cout << "Массив: " << array << endl;
    } catch (const runtime_error& e) {
        cout << "Ошибка: " << e.what() << endl;
    }
}

// Добавление элемента в конец
void ConsoleApplication::handlePushBack() {
    cout << "Текущая вместимость (capacity): " << array.getCapacity() << endl;
    cout << "Текущее количество элементов (size): " << array.getSize() << endl;
    cout << "Текущий массив: " << array << endl;

    if (array.getSize() == array.getCapacity()) {
        cout << "Внимание! Массив заполнен. Сначала увеличьте capacity (пункт 4 меню).\n";
        return;
    }

    cout << "Введите значение для добавления: ";
    number value;
    cin >> value;

    if (cin.fail()) {
        cout << "Ошибка ввода!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    try {
        array.pushBack(value);
        cout << "Элемент добавлен. Новый массив: " << array << endl;
        cout << "Использовано " << array.getSize() << " из " << array.getCapacity() << endl;
    } catch (const runtime_error& e) {
        cout << "Ошибка: " << e.what() << endl;
    }
}

// Удаление элемента с конца
void ConsoleApplication::handlePopBack() {
    if (array.isEmpty()) {
        cout << "Массив пуст! Нечего удалять.\n";
        return;
    }

    cout << "Текущий массив: " << array << endl;
    cout << "Текущий size: " << array.getSize() << ", capacity: " << array.getCapacity() << endl;

    try {
        array.popBack();
        cout << "Последний элемент удален. Новый массив: " << array << endl;
        cout << "Теперь size: " << array.getSize() << ", capacity: " << array.getCapacity() << endl;
    } catch (const runtime_error& e) {
        cout << "Ошибка: " << e.what() << endl;
    }
}

// Изменение значения элемента
void ConsoleApplication::handleChangeElement() {
    if (array.isEmpty()) {
        cout << "Массив пуст! Нельзя изменить элемент.\n";
        return;
    }

    cout << "Текущий массив: " << array << endl;
    cout << "Введите индекс элемента (от 0 до " << array.getSize() - 1 << "): ";

    size_t index;
    cin >> index;

    if (cin.fail()) {
        cout << "Ошибка ввода!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    try {
        cout << "Текущее значение array[" << index << "] = " << array[index] << endl;
        cout << "Введите новое значение: ";

        number value;
        cin >> value;

        if (cin.fail()) {
            cout << "Ошибка ввода! Значение не изменено.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        array[index] = value;
        cout << "Элемент изменен. Новый массив: " << array << endl;
    } catch (const out_of_range& e) {
        cout << "Ошибка: " << e.what() << endl;
    }
}

// Вывод информации о массиве
void ConsoleApplication::handleDisplay() {
    cout << "\n=== Информация о массиве ===\n";
    if (array.isEmpty()) {
        cout << "Массив пуст.\n";
    } else {
        cout << "Массив: " << array << endl;
    }
    cout << "Размер (size): " << array.getSize() << endl;
    cout << "Вместимость (capacity): " << array.getCapacity() << endl;
    cout << "Свободно места: " << array.getCapacity() - array.getSize() << endl;
}

// Запуск приложения
void ConsoleApplication::run() {
    cout << "=== Консольное приложение для работы с массивом ===\n";
    cout << "Новая логика: size показывает реальные элементы, capacity - выделенную память\n";

    while (running) {
        showMenu();

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cout << "Ошибка ввода! Пожалуйста, введите число.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            handleInput();
            break;
        case 2:
            handleCalculate();
            break;
        case 3:
            handleSort();
            break;
        case 4:
            handleReserve();
            break;
        case 5:
            handlePushBack();
            break;
        case 6:
            handlePopBack();
            break;
        case 7:
            handleChangeElement();
            break;
        case 8:
            handleDisplay();
            break;
        case 0:
            cout << "Завершение работы приложения...\n";
            running = false;
            break;
        default:
            cout << "Неверная команда! Пожалуйста, выберите 0-8.\n";
        }
    }
}
