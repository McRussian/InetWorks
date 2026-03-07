#include "application.h"
#include <iostream>
#include <limits>

using namespace std;

// Конструктор с инициализацией QObject
ConsoleApplication::ConsoleApplication(QObject* parent)
    : QObject(parent), polinom(), polinomExists(false), running(true)
{
}

ConsoleApplication::~ConsoleApplication()
{
    // Виртуальный деструктор
}

// Показать меню
void ConsoleApplication::showMenu() {
    cout << "\n========== МЕНЮ ПРИЛОЖЕНИЯ ==========\n";
    cout << "1. Ввести новый полином\n";
    cout << "2. Изменить старший коэффициент или корень\n";
    cout << "3. Вычислить значение полинома в точке\n";
    cout << "4. Изменить степень полинома\n";
    cout << "5. Вывести полином (форма 1: an*x^n + ... + a0)\n";
    cout << "6. Вывести полином (форма 2: an*(x - r1)*...*(x - rn))\n";
    cout << "7. Вывести корни полинома\n";
    cout << "8. Вывести коэффициенты полинома\n";
    cout << "0. Выход\n";
    cout << "=====================================\n";
    cout << "Выберите команду: ";
}

// Ввод нового полинома
void ConsoleApplication::handleInput() {
    cout << "=== Ввод нового полинома ===\n";
    cout << "Формат: an n r1 r2 ... rn\n";
    cout << "где n - степень полинома (количество корней)\n";
    cout << "Пример: 1 3 1+2i 3-4i 5i\n";
    cout << "(an=1, степень 3, корни: 1+2i, 3-4i, 5i)\n";
    cout << "Ваш ввод: ";

    // Очищаем поток ввода
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Используем оператор >> для Polinom
    cin >> polinom;

    if (cin.fail()) {
        cout << "Ошибка ввода! Полином не создан.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        polinomExists = false;
    } else {
        cout << "Полином успешно создан.\n";
        cout << "Степень: " << polinom.getDegree() << endl;
        cout << "Старший коэффициент: " << polinom.getLeadingCoeff() << endl;
        cout << "Корни: ";
        const Array& roots = polinom.getRoots();
        for (size_t i = 0; i < roots.getSize(); ++i) {
            cout << roots[i] << " ";
        }
        cout << endl;
        polinomExists = true;
    }
}

// Изменение старшего коэффициента или корня
void ConsoleApplication::handleChangeCoeffOrRoot() {
    if (!polinomExists) {
        cout << "Ошибка: полином не создан. Сначала введите полином (команда 1).\n";
        return;
    }

    if (polinom.getDegree() == 0 && polinom.getLeadingCoeff() == number(0.0)) {
        cout << "Полином нулевой. Сначала введите полином (команда 1).\n";
        return;
    }

    cout << "=== Изменение параметров полинома ===\n";
    cout << "Текущий полином: ";
    polinom.printForm2(cout);
    cout << endl;

    cout << "Что изменить?\n";
    cout << "1. Старший коэффициент an\n";
    cout << "2. Корень по индексу\n";
    cout << "Ваш выбор: ";

    int choice;
    cin >> choice;

    if (cin.fail()) {
        cout << "Ошибка ввода!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    switch (choice) {
    case 1: {
        cout << "Текущий старший коэффициент: " << polinom.getLeadingCoeff() << endl;
        cout << "Введите новое значение an: ";
        number an;
        cin >> an;

        if (cin.fail()) {
            cout << "Ошибка ввода!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        polinom.setLeadingCoeff(an);
        cout << "Старший коэффициент изменен.\n";
        break;
    }
    case 2: {
        size_t degree = polinom.getDegree();
        if (degree == 0) {
            cout << "Полином нулевой степени, корней нет.\n";
            return;
        }

        cout << "Текущие корни:\n";
        const Array& roots = polinom.getRoots();
        for (size_t i = 0; i < roots.getSize(); ++i) {
            cout << "  r" << i << " = " << roots[i] << endl;
        }

        cout << "Введите индекс корня (0.." << degree - 1 << "): ";
        size_t index;
        cin >> index;

        if (cin.fail() || index >= degree) {
            cout << "Неверный индекс!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        cout << "Текущее значение r" << index << " = " << polinom.getRoot(index) << endl;
        cout << "Введите новое значение: ";
        number value;
        cin >> value;

        if (cin.fail()) {
            cout << "Ошибка ввода!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        polinom.setRoot(index, value);
        cout << "Корень изменен.\n";
        break;
    }
    default:
        cout << "Неверный выбор!\n";
    }
}

// Вычисление значения полинома в точке
void ConsoleApplication::handleEvaluate() {
    if (!polinomExists) {
        cout << "Ошибка: полином не создан. Сначала введите полином (команда 1).\n";
        return;
    }

    if (polinom.getDegree() == 0 && polinom.getLeadingCoeff() == number(0.0)) {
        cout << "Полином нулевой. Сначала введите полином (команда 1).\n";
        return;
    }

    cout << "=== Вычисление значения полинома ===\n";
    cout << "Полином: ";
    polinom.printForm1(cout);
    cout << endl;

    cout << "Введите точку x: ";
    number x;
    cin >> x;

    if (cin.fail()) {
        cout << "Ошибка ввода!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    number result = polinom.evaluate(x);
    cout << "Значение полинома в точке x = " << x << " равно: " << result << endl;
}

// Изменение степени полинома
void ConsoleApplication::handleResize() {
    if (!polinomExists) {
        cout << "Ошибка: полином не создан. Сначала введите полином (команда 1).\n";
        return;
    }

    cout << "=== Изменение степени полинома ===\n";
    cout << "Текущая степень: " << polinom.getDegree() << endl;
    cout << "Текущий полином: ";
    polinom.printForm2(cout);
    cout << endl;

    cout << "Введите новую степень: ";
    size_t newDegree;
    cin >> newDegree;

    if (cin.fail()) {
        cout << "Ошибка ввода!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    polinom.resize(newDegree);
    cout << "Степень изменена. Новый полином:\n";
    polinom.printForm2(cout);
    cout << endl;
}

// Вывод полинома в форме 1
void ConsoleApplication::handleDisplayForm1() {
    if (!polinomExists) {
        cout << "Ошибка: полином не создан. Сначала введите полином (команда 1).\n";
        return;
    }

    cout << "=== Полином (форма 1: an*x^n + ... + a0) ===\n";
    if (polinom.getDegree() == 0 && polinom.getLeadingCoeff() == number(0.0)) {
        cout << "Полином нулевой.\n";
    } else {
        polinom.printForm1(cout);
        cout << endl;
    }
}

// Вывод полинома в форме 2
void ConsoleApplication::handleDisplayForm2() {
    if (!polinomExists) {
        cout << "Ошибка: полином не создан. Сначала введите полином (команда 1).\n";
        return;
    }

    cout << "=== Полином (форма 2: an*(x - r1)*...*(x - rn)) ===\n";
    if (polinom.getDegree() == 0 && polinom.getLeadingCoeff() == number(0.0)) {
        cout << "Полином нулевой.\n";
    } else {
        polinom.printForm2(cout);
        cout << endl;
    }
}

// Вывод корней полинома
void ConsoleApplication::handlePrintRoots() {
    if (!polinomExists) {
        cout << "Ошибка: полином не создан. Сначала введите полином (команда 1).\n";
        return;
    }

    cout << "=== Корни полинома ===\n";
    size_t degree = polinom.getDegree();

    if (degree == 0) {
        cout << "Полином нулевой степени, корней нет.\n";
        return;
    }

    const Array& roots = polinom.getRoots();
    for (size_t i = 0; i < degree; ++i) {
        cout << "r" << i << " = " << roots[i] << endl;
    }
}

// Вывод коэффициентов полинома
void ConsoleApplication::handlePrintCoeffs() {
    if (!polinomExists) {
        cout << "Ошибка: полином не создан. Сначала введите полином (команда 1).\n";
        return;
    }

    cout << "=== Коэффициенты полинома ===\n";
    size_t degree = polinom.getDegree();

    cout << "Коэффициенты полинома (от старшей степени к младшей):\n";
    for (size_t i = degree; i > 0; --i) {
        cout << "a" << i << " = " << polinom.getCoefficient(i) << endl;
    }
    cout << "a0 = " << polinom.getCoefficient(0) << endl;
}

// Запуск приложения
void ConsoleApplication::run() {
    cout << "========================================\n";
    cout << "   КОНСОЛЬНОЕ ПРИЛОЖЕНИЕ ДЛЯ РАБОТЫ\n";
    cout << "         С ПОЛИНОМОМ (COMPLEX)\n";
    cout << "========================================\n";
    cout << "Полином не создан. Используйте команду 1 для ввода.\n";

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
            handleChangeCoeffOrRoot();
            break;
        case 3:
            handleEvaluate();
            break;
        case 4:
            handleResize();
            break;
        case 5:
            handleDisplayForm1();
            break;
        case 6:
            handleDisplayForm2();
            break;
        case 7:
            handlePrintRoots();
            break;
        case 8:
            handlePrintCoeffs();
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
