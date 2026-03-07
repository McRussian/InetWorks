#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>  // Добавлено
#include "polinom.h"

class ConsoleApplication : public QObject  // Добавлено наследование
{
    Q_OBJECT  // Добавлено

protected:
    Polinom polinom;           // Текущий полином
    bool polinomExists;        // Флаг наличия полинома
    bool running;              // Флаг работы приложения

    // Методы меню
    void showMenu();
    void handleInput();
    void handleChangeCoeffOrRoot();
    void handleEvaluate();
    void handleResize();
    void handleDisplayForm1();
    void handleDisplayForm2();
    void handlePrintRoots();
    void handlePrintCoeffs();

public:
    explicit ConsoleApplication(QObject* parent = nullptr);  // Явный конструктор с parent
    virtual ~ConsoleApplication();  // Виртуальный деструктор

    virtual void run();  // Виртуальный метод для возможности переопределения
};
#endif // APPLICATION_H
