#include "application.h"
#include <iostream>
#include <limits>
#include <sstream>

using namespace std;

ConsoleApplication::ConsoleApplication(QObject* parent)
    : QObject(parent), realPolinom(nullptr), complexPolinom(nullptr),
    currentType(0), polinomExists(false), running(true)
{
}

ConsoleApplication::~ConsoleApplication()
{
    if (realPolinom) delete realPolinom;
    if (complexPolinom) delete complexPolinom;
}

void ConsoleApplication::selectDataType()
{
    cout << "\n=== Выбор типа данных ===\n";
    cout << "1. Вещественные числа (double)\n";
    cout << "2. Комплексные числа (TComplex)\n";
    cout << "Выберите тип (1-2): ";

    int type;
    cin >> type;

    if (cin.fail()) {
        cout << "Ошибка ввода! Используется double по умолчанию.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        type = 1;
    }

    if (type == 1) {
        currentType = 1;
        if (realPolinom) delete realPolinom;
        if (complexPolinom) {
            delete complexPolinom;
            complexPolinom = nullptr;
        }
        realPolinom = new Polinom<double>();
        cout << "✓ Выбран тип: double\n";
    } else if (type == 2) {
        currentType = 2;
        if (complexPolinom) delete complexPolinom;
        if (realPolinom) {
            delete realPolinom;
            realPolinom = nullptr;
        }
        complexPolinom = new Polinom<TComplex>();
        cout << "✓ Выбран тип: TComplex\n";
    } else {
        cout << "Неверный выбор. Используется double по умолчанию.\n";
        currentType = 1;
        if (!realPolinom) realPolinom = new Polinom<double>();
    }

    polinomExists = false;
}

void ConsoleApplication::showMenu()
{
    cout << "\n========== МЕНЮ ПРИЛОЖЕНИЯ ==========\n";
    cout << "1. Ввести новый полином\n";
    cout << "2. Изменить старший коэффициент или корень\n";
    cout << "3. Вычислить значение полинома в точке\n";
    cout << "4. Изменить степень полинома\n";
    cout << "5. Вывести полином (форма 1)\n";
    cout << "6. Вывести полином (форма 2)\n";
    cout << "7. Вывести корни полинома\n";
    cout << "8. Вывести коэффициенты полинома\n";
    cout << "9. Сменить тип данных\n";
    cout << "0. Выход\n";
    cout << "=====================================\n";
}

// ==================== Обертки ====================

void ConsoleApplication::handleInput()
{
    if (currentType == 1)
        handleInputDouble();
    else if (currentType == 2)
        handleInputComplex();
    else {
        cout << "Тип данных не выбран. Сначала выберите тип (команда 9).\n";
    }
}

void ConsoleApplication::handleChangeCoeffOrRoot()
{
    if (!polinomExists) {
        cout << "Полином не создан. Сначала введите полином.\n";
        return;
    }

    if (currentType == 1)
        handleChangeCoeffOrRootDouble();
    else if (currentType == 2)
        handleChangeCoeffOrRootComplex();
}

void ConsoleApplication::handleEvaluate()
{
    if (!polinomExists) {
        cout << "Полином не создан.\n";
        return;
    }

    if (currentType == 1)
        handleEvaluateDouble();
    else if (currentType == 2)
        handleEvaluateComplex();
}

void ConsoleApplication::handleResize()
{
    if (!polinomExists) {
        cout << "Полином не создан.\n";
        return;
    }

    if (currentType == 1)
        handleResizeDouble();
    else if (currentType == 2)
        handleResizeComplex();
}

void ConsoleApplication::handleDisplayForm1()
{
    if (!polinomExists) {
        cout << "Полином не создан.\n";
        return;
    }

    if (currentType == 1)
        handleDisplayForm1Double();
    else if (currentType == 2)
        handleDisplayForm1Complex();
}

void ConsoleApplication::handleDisplayForm2()
{
    if (!polinomExists) {
        cout << "Полином не создан.\n";
        return;
    }

    if (currentType == 1)
        handleDisplayForm2Double();
    else if (currentType == 2)
        handleDisplayForm2Complex();
}

void ConsoleApplication::handlePrintRoots()
{
    if (!polinomExists) {
        cout << "Полином не создан.\n";
        return;
    }

    if (currentType == 1)
        handlePrintRootsDouble();
    else if (currentType == 2)
        handlePrintRootsComplex();
}

void ConsoleApplication::handlePrintCoeffs()
{
    if (!polinomExists) {
        cout << "Полином не создан.\n";
        return;
    }

    if (currentType == 1)
        handlePrintCoeffsDouble();
    else if (currentType == 2)
        handlePrintCoeffsComplex();
}

// ==================== Реализация для double (базовая) ====================

void ConsoleApplication::handleInputDouble()
{
    cout << "\n=== Ввод нового полинома (double) ===\n";
    cout << "Формат: an degree r1 r2 ... rn\n";
    cout << "Пример: 2 3 1 2 3\n";
    cout << "Ваш ввод: ";

    double an;
    size_t degree;

    cin >> an >> degree;

    if (cin.fail()) {
        cout << "Ошибка ввода! Полином не создан.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        polinomExists = false;
        return;
    }

    Array<double> roots;
    for (size_t i = 0; i < degree; ++i) {
        double root;
        cin >> root;
        if (cin.fail()) {
            cout << "Ошибка ввода корня " << i << "!\n";
            cin.clear();
            cin.ignore(10000, '\n');
            polinomExists = false;
            return;
        }
        roots.pushBack(root);
    }

    if (realPolinom) delete realPolinom;
    realPolinom = new Polinom<double>(an, roots);
    polinomExists = true;
    cout << "✓ Полином (double) успешно создан.\n";
}

void ConsoleApplication::handleChangeCoeffOrRootDouble()
{
    cout << "\n--- Изменение параметров полинома (double) ---\n";
    cout << "Текущий полином: ";
    realPolinom->printForm2(cout);
    cout << endl;

    cout << "Что изменить?\n";
    cout << "1. Старший коэффициент an\n";
    cout << "2. Корень по индексу\n";
    cout << "Ваш выбор: ";

    int subChoice;
    cin >> subChoice;

    if (subChoice == 1) {
        cout << "Текущий an = " << realPolinom->getLeadingCoeff() << endl;
        cout << "Введите новое значение an: ";
        double an;
        cin >> an;
        realPolinom->setLeadingCoeff(an);
        cout << "✓ Старший коэффициент изменен.\n";
    } else if (subChoice == 2) {
        size_t degree = realPolinom->getDegree();
        if (degree == 0) {
            cout << "Полином нулевой степени, корней нет.\n";
            return;
        }

        cout << "Текущие корни:\n";
        const Array<double>& roots = realPolinom->getRoots();
        for (size_t i = 0; i < roots.getSize(); ++i) {
            cout << "  r" << i << " = " << roots[i] << endl;
        }

        cout << "Введите индекс корня (0.." << degree - 1 << "): ";
        size_t index;
        cin >> index;

        if (index < degree) {
            cout << "Текущее значение r" << index << " = " << realPolinom->getRoot(index) << endl;
            cout << "Введите новое значение: ";
            double value;
            cin >> value;
            realPolinom->setRoot(index, value);
            cout << "✓ Корень изменен.\n";
        } else {
            cout << "Неверный индекс!\n";
        }
    }
}

void ConsoleApplication::handleEvaluateDouble()
{
    cout << "\n--- Вычисление значения полинома (double) ---\n";
    cout << "Полином: ";
    realPolinom->printForm1(cout);
    cout << endl;

    cout << "Введите точку x: ";
    double x;
    cin >> x;

    double result = realPolinom->evaluate(x);
    cout << "P(" << x << ") = " << result << endl;
}

void ConsoleApplication::handleResizeDouble()
{
    cout << "\n--- Изменение степени полинома (double) ---\n";
    cout << "Текущая степень: " << realPolinom->getDegree() << endl;
    cout << "Текущий полином: ";
    realPolinom->printForm2(cout);
    cout << endl;

    cout << "Введите новую степень: ";
    size_t newDegree;
    cin >> newDegree;

    realPolinom->resize(newDegree);
    cout << "✓ Степень изменена.\n";
}

void ConsoleApplication::handleDisplayForm1Double()
{
    cout << "\n--- Полином (форма 1, double) ---\n";
    realPolinom->printForm1(cout);
    cout << endl;
}

void ConsoleApplication::handleDisplayForm2Double()
{
    cout << "\n--- Полином (форма 2, double) ---\n";
    realPolinom->printForm2(cout);
    cout << endl;
}

void ConsoleApplication::handlePrintRootsDouble()
{
    cout << "\n--- Корни полинома (double) ---\n";
    const Array<double>& roots = realPolinom->getRoots();
    for (size_t i = 0; i < roots.getSize(); ++i) {
        cout << "  r" << i << " = " << roots[i] << endl;
    }
}

void ConsoleApplication::handlePrintCoeffsDouble()
{
    cout << "\n--- Коэффициенты полинома (double) ---\n";
    cout << "Коэффициенты (от старшей степени к младшей):\n";
    size_t degree = realPolinom->getDegree();
    for (size_t i = degree; i > 0; --i) {
        cout << "  a" << i << " = " << realPolinom->getCoefficient(i) << endl;
    }
    cout << "  a0 = " << realPolinom->getCoefficient(0) << endl;
}

// ==================== Реализация для TComplex (базовая) ====================

void ConsoleApplication::handleInputComplex()
{
    cout << "\n=== Ввод нового полинома (TComplex) ===\n";
    cout << "Формат: an degree r1 r2 ... rn\n";
    cout << "Формат числа: (re,im) или re+imi\n";
    cout << "Пример: (1,0) 2 (1,0) (2,0)\n";
    cout << "Ваш ввод: ";

    // Очищаем буфер
    cin.clear();
    cin.ignore(10000, '\n');

    string line;
    getline(cin, line);

    while (line.empty()) {
        getline(cin, line);
    }

    stringstream ss(line);

    TComplex an;
    ss >> an;
    if (ss.fail()) {
        cout << "Ошибка ввода старшего коэффициента!\n";
        polinomExists = false;
        return;
    }

    size_t degree;
    ss >> degree;
    if (ss.fail()) {
        cout << "Ошибка ввода степени!\n";
        polinomExists = false;
        return;
    }

    Array<TComplex> roots;
    bool success = true;

    for (size_t i = 0; i < degree; ++i) {
        TComplex root;
        ss >> root;
        if (ss.fail()) {
            cout << "Ошибка ввода корня " << i << "!\n";
            success = false;
            break;
        }
        roots.pushBack(root);
    }

    if (success) {
        if (complexPolinom) delete complexPolinom;
        complexPolinom = new Polinom<TComplex>(an, roots);
        polinomExists = true;
        cout << "✓ Полином (TComplex) успешно создан.\n";
    } else {
        cout << "Ошибка ввода! Полином не создан.\n";
        polinomExists = false;
    }
}

void ConsoleApplication::handleChangeCoeffOrRootComplex()
{
    cout << "\n--- Изменение параметров полинома (TComplex) ---\n";
    cout << "Текущий полином: ";
    complexPolinom->printForm2(cout);
    cout << endl;

    cout << "Что изменить?\n";
    cout << "1. Старший коэффициент an\n";
    cout << "2. Корень по индексу\n";
    cout << "Ваш выбор: ";

    int subChoice;
    cin >> subChoice;

    if (subChoice == 1) {
        cout << "Текущий an = " << complexPolinom->getLeadingCoeff() << endl;
        cout << "Введите новое значение an: ";
        TComplex an;
        cin >> an;
        complexPolinom->setLeadingCoeff(an);
        cout << "✓ Старший коэффициент изменен.\n";
    } else if (subChoice == 2) {
        size_t degree = complexPolinom->getDegree();
        if (degree == 0) {
            cout << "Полином нулевой степени, корней нет.\n";
            return;
        }

        cout << "Текущие корни:\n";
        const Array<TComplex>& roots = complexPolinom->getRoots();
        for (size_t i = 0; i < roots.getSize(); ++i) {
            cout << "  r" << i << " = " << roots[i] << endl;
        }

        cout << "Введите индекс корня (0.." << degree - 1 << "): ";
        size_t index;
        cin >> index;

        if (index < degree) {
            cout << "Текущее значение r" << index << " = " << complexPolinom->getRoot(index) << endl;
            cout << "Введите новое значение: ";
            TComplex value;
            cin >> value;
            complexPolinom->setRoot(index, value);
            cout << "✓ Корень изменен.\n";
        } else {
            cout << "Неверный индекс!\n";
        }
    }
}

void ConsoleApplication::handleEvaluateComplex()
{
    cout << "\n--- Вычисление значения полинома (TComplex) ---\n";
    cout << "Полином: ";
    complexPolinom->printForm1(cout);
    cout << endl;

    cout << "Введите точку x: ";
    TComplex x;
    cin >> x;

    TComplex result = complexPolinom->evaluate(x);
    cout << "P(" << x << ") = " << result << endl;
}

void ConsoleApplication::handleResizeComplex()
{
    cout << "\n--- Изменение степени полинома (TComplex) ---\n";
    cout << "Текущая степень: " << complexPolinom->getDegree() << endl;
    cout << "Текущий полином: ";
    complexPolinom->printForm2(cout);
    cout << endl;

    cout << "Введите новую степень: ";
    size_t newDegree;
    cin >> newDegree;

    complexPolinom->resize(newDegree);
    cout << "✓ Степень изменена.\n";
}

void ConsoleApplication::handleDisplayForm1Complex()
{
    cout << "\n--- Полином (форма 1, TComplex) ---\n";
    complexPolinom->printForm1(cout);
    cout << endl;
}

void ConsoleApplication::handleDisplayForm2Complex()
{
    cout << "\n--- Полином (форма 2, TComplex) ---\n";
    complexPolinom->printForm2(cout);
    cout << endl;
}

void ConsoleApplication::handlePrintRootsComplex()
{
    cout << "\n--- Корни полинома (TComplex) ---\n";
    const Array<TComplex>& roots = complexPolinom->getRoots();
    for (size_t i = 0; i < roots.getSize(); ++i) {
        cout << "  r" << i << " = " << roots[i] << endl;
    }
}

void ConsoleApplication::handlePrintCoeffsComplex()
{
    cout << "\n--- Коэффициенты полинома (TComplex) ---\n";
    cout << "Коэффициенты (от старшей степени к младшей):\n";
    size_t degree = complexPolinom->getDegree();
    for (size_t i = degree; i > 0; --i) {
        cout << "  a" << i << " = " << complexPolinom->getCoefficient(i) << endl;
    }
    cout << "  a0 = " << complexPolinom->getCoefficient(0) << endl;
}

// ==================== Главный цикл ====================

void ConsoleApplication::run()
{
    cout << "========================================\n";
    cout << "   КОНСОЛЬНОЕ ПРИЛОЖЕНИЕ\n";
    cout << "========================================\n";

    selectDataType();

    while (running) {
        showMenu();
        cout << "> ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cout << "Ошибка ввода! Пожалуйста, введите число.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
        case 1: handleInput(); break;
        case 2: handleChangeCoeffOrRoot(); break;
        case 3: handleEvaluate(); break;
        case 4: handleResize(); break;
        case 5: handleDisplayForm1(); break;
        case 6: handleDisplayForm2(); break;
        case 7: handlePrintRoots(); break;
        case 8: handlePrintCoeffs(); break;
        case 9: selectDataType(); break;
        case 0:
            cout << "Завершение работы...\n";
            running = false;
            break;
        default:
            cout << "Неверная команда! Пожалуйста, выберите 0-9.\n";
        }
    }
}
