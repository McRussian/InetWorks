#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include "polinom.hpp"
#include "complex.h"

class ConsoleApplication : public QObject
{
    Q_OBJECT

protected:
    // Полиномы для разных типов
    Polinom<double>* realPolinom;
    Polinom<TComplex>* complexPolinom;

    int currentType;           // 1 - double, 2 - TComplex
    bool polinomExists;        // Флаг наличия полинома
    bool running;              // Флаг работы приложения

    // Методы для работы с double
    virtual void handleInputDouble();
    virtual void handleChangeCoeffOrRootDouble();
    virtual void handleEvaluateDouble();
    virtual void handleResizeDouble();
    virtual void handleDisplayForm1Double();
    virtual void handleDisplayForm2Double();
    virtual void handlePrintRootsDouble();
    virtual void handlePrintCoeffsDouble();

    // Методы для работы с TComplex
    virtual void handleInputComplex();
    virtual void handleChangeCoeffOrRootComplex();
    virtual void handleEvaluateComplex();
    virtual void handleResizeComplex();
    virtual void handleDisplayForm1Complex();
    virtual void handleDisplayForm2Complex();
    virtual void handlePrintRootsComplex();
    virtual void handlePrintCoeffsComplex();

public:
    explicit ConsoleApplication(QObject* parent = nullptr);
    virtual ~ConsoleApplication();

    // Методы меню (обертки)
    virtual void showMenu();
    virtual void selectDataType();
    virtual void handleInput();
    virtual void handleChangeCoeffOrRoot();
    virtual void handleEvaluate();
    virtual void handleResize();
    virtual void handleDisplayForm1();
    virtual void handleDisplayForm2();
    virtual void handlePrintRoots();
    virtual void handlePrintCoeffs();

    virtual void run();

    // Геттеры для доступа из наследников
    Polinom<double>* getRealPolinom() const { return realPolinom; }
    Polinom<TComplex>* getComplexPolinom() const { return complexPolinom; }
    int getCurrentType() const { return currentType; }
    bool hasPolinom() const { return polinomExists; }
};

#endif // APPLICATION_H
