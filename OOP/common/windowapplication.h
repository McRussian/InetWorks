#ifndef WINDOWAPPLICATION_H
#define WINDOWAPPLICATION_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include "polinom.hpp"
#include "complex.h"

class WindowApplication : public QWidget
{
    Q_OBJECT

protected:
    // Полиномы для разных типов
    Polinom<double>* realPolinom;
    Polinom<TComplex>* complexPolinom;

    int currentType;           // 1 - double, 2 - TComplex
    bool polinomExists;        // Флаг наличия полинома

    // Элементы интерфейса
    QComboBox* typeDisplayComboBox;  // Для отображения типа (только чтение)
    QTextEdit* displayArea;          // Область отображения информации

    virtual void setupUI();
    virtual void updateDisplay();

public:
    explicit WindowApplication(QWidget *parent = nullptr);
    virtual ~WindowApplication();

    // Методы для установки полиномов
    void setRealPolinom(const Polinom<double>& polinom);
    void setComplexPolinom(const Polinom<TComplex>& polinom);
    void setCurrentType(int type);
    void clearPolinom();

    // Геттеры
    Polinom<double>* getRealPolinom() const { return realPolinom; }
    Polinom<TComplex>* getComplexPolinom() const { return complexPolinom; }
    int getCurrentType() const { return currentType; }
    bool hasPolinom() const { return polinomExists; }
};

#endif // WINDOWAPPLICATION_H
