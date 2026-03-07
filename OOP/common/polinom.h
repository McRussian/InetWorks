#ifndef POLINOM_H
#define POLINOM_H

#include <iostream>
#include "array.h"
#include "number.h"

class Polinom {
private:
    Array roots;        // Массив корней полинома (size = степень полинома)
    number leadingCoeff; // Старший коэффициент an
    Array coefficients;  // Массив коэффициентов при степенях x

    void calculateCoefficients(); // Вычисляет коэффициенты из корней

public:
    // Конструкторы
    Polinom();                              // Полином 0-й степени (константа 0)
    explicit Polinom(size_t n);              // Полином n-й степени с корнями = 0
    Polinom(size_t n, number an, const Array& rootArray); // Полином n-й степени с заданными корнями
    Polinom(number an, const Array& rootArray); // Полином степени rootArray.getSize() с корнями из массива

    // Правило трёх
    Polinom(const Polinom& other);
    ~Polinom() = default;
    Polinom& operator=(const Polinom& other);

    // Геттеры
    size_t getDegree() const;                 // Степень полинома
    number getLeadingCoeff() const;           // Старший коэффициент
    number getRoot(size_t index) const;       // Корень по индексу
    number getCoefficient(size_t power) const; // Коэффициент при x^power
    const Array& getRoots() const;            // Получить массив корней

    // Сеттеры
    void setLeadingCoeff(number an);           // Изменить старший коэффициент
    void setRoot(size_t index, number value);  // Изменить корень по индексу

    // Изменение размерности (степени полинома)
    void resize(size_t newDegree);              // Изменить степень

    // Вычисление значения полинома в точке x
    number evaluate(number x) const;

    // Оператор вызова для вычисления значения
    number operator()(number x) const;

    // Формы представления
    void printForm1(std::ostream& out) const;  // Вывод в форме an*x^n + ... + a0
    void printForm2(std::ostream& out) const;  // Вывод в форме an*(x - r1)*...*(x - rn)

    // Дружественные операторы ввода/вывода
    friend std::istream& operator>>(std::istream& in, Polinom& p);
    friend std::ostream& operator<<(std::ostream& out, const Polinom& p);
};

#endif // POLINOM_H
