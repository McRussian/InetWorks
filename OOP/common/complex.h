#ifndef TCOMPLEX_H
#define TCOMPLEX_H

#include <iostream>
#include <cmath>

class TComplex {
private:
    double re;  // Действительная часть
    double im;  // Мнимая часть

public:
    // Конструкторы
    TComplex();                          // По умолчанию (0+0i)
    TComplex(double real, double imag = 0); // С параметрами

    // Геттеры
    double getRe() const;
    double getIm() const;

    // Сеттеры
    void setRe(double real);
    void setIm(double imag);

    // Модуль (норма) комплексного числа - для сравнения
    double norm() const;

    // Арифметические операторы
    TComplex operator+(const TComplex& other) const;
    TComplex operator-(const TComplex& other) const;
    TComplex operator*(const TComplex& other) const;
    TComplex operator/(const TComplex& other) const;

    // Операторы присваивания с арифметикой
    TComplex& operator+=(const TComplex& other);
    TComplex& operator-=(const TComplex& other);
    TComplex& operator*=(const TComplex& other);
    TComplex& operator/=(const TComplex& other);

    // Унарный минус
    TComplex operator-() const;

    // Операторы сравнения (по норме)
    bool operator==(const TComplex& other) const;
    bool operator!=(const TComplex& other) const;
    bool operator<(const TComplex& other) const;
    bool operator<=(const TComplex& other) const;
    bool operator>(const TComplex& other) const;
    bool operator>=(const TComplex& other) const;
    // Извлечение квадратного корня
    TComplex sqrt() const;

    // Квадрат модуля (норма в квадрате)
    double normSquared() const;

    // Модуль (вещественное число)
    double modulus() const;

    // Дружественные функции для ввода/вывода
    friend std::istream& operator>>(std::istream& in, TComplex& c);
    friend std::ostream& operator<<(std::ostream& out, const TComplex& c);
};

// Глобальная функция sqrt для комплексных чисел
TComplex sqrt(const TComplex& c);
#endif // TCOMPLEX_H
