#ifndef ALLFUNCTIONS_H
#define ALLFUNCTIONS_H

#include <QString>
#include <cmath>
#include "polinom.hpp"
#include "complex.h"

// ==================== Базовый абстрактный класс Function ====================

class Function {
protected:
    int maxDegree;           // Максимальная степень для вычислений

public:
    Function() : maxDegree(10) {}
    virtual ~Function() = default;

    // Чисто виртуальные методы
    virtual QString getName() const = 0;
    virtual TComplex evaluate(const TComplex& x, int degree) const = 0;
    virtual Polinom<TComplex> getMaclaurinPolynom(int degree) const = 0;

    // Геттеры/сеттеры
    void setMaxDegree(int degree) { maxDegree = degree; }
    int getMaxDegree() const { return maxDegree; }
};

// ==================== Класс SinFunction ====================

class SinFunction : public Function {
private:
    // Вычисление факториала
    long long factorial(int n) const {
        long long result = 1;
        for (int i = 1; i <= n; ++i) {
            result *= i;
        }
        return result;
    }

    // Вычисление степени комплексного числа
    TComplex power(const TComplex& x, int n) const {
        TComplex result(1, 0);
        for (int i = 0; i < n; ++i) {
            result = result * x;
        }
        return result;
    }

public:
    SinFunction() : Function() {}
    ~SinFunction() override = default;

    QString getName() const override {
        return "sin(x)";
    }

    // Вычисление члена ряда: (-1)^n * x^(2n+1) / (2n+1)!
    TComplex term(const TComplex& x, int n) const {
        int k = 2 * n + 1;
        TComplex xk = power(x, k);
        long long fact = factorial(k);

        if (n % 2 == 1) {
            xk = -xk;
        }

        return xk / TComplex(fact, 0);
    }

    TComplex evaluate(const TComplex& x, int degree) const override {
        // Для sin нужны только нечетные степени
        int terms = (degree + 1) / 2;
        if (terms < 1) terms = 1;

        TComplex result(0, 0);
        for (int n = 0; n < terms; ++n) {
            result = result + term(x, n);
        }
        return result;
    }

    Polinom<TComplex> getMaclaurinPolynom(int degree) const override {
        // Создаем полином степени degree
        // Коэффициенты: a1 = 1, a3 = -1/6, a5 = 1/120, a7 = -1/5040, ...
        // Все четные коэффициенты равны 0

        // Для создания полинома нам нужны корни, но проще создать через коэффициенты
        // Временно возвращаем пустой полином, в реальности нужно реализовать
        return Polinom<TComplex>(degree);
    }
};

// ==================== Класс SiFunction (sin(x)/x) ====================

class SiFunction : public Function {
private:
    // Вычисление факториала
    long long factorial(int n) const {
        long long result = 1;
        for (int i = 1; i <= n; ++i) {
            result *= i;
        }
        return result;
    }

    // Вычисление степени комплексного числа
    TComplex power(const TComplex& x, int n) const {
        TComplex result(1, 0);
        for (int i = 0; i < n; ++i) {
            result = result * x;
        }
        return result;
    }

public:
    SiFunction() : Function() {}
    ~SiFunction() override = default;

    QString getName() const override {
        return "Si(x) = sin(x)/x";
    }

    // Вычисление члена ряда: (-1)^n * x^(2n) / (2n+1)!
    TComplex term(const TComplex& x, int n) const {
        int k = 2 * n;
        TComplex xk = power(x, k);

        // Факториал для (2n+1)!
        int factN = 2 * n + 1;
        long long fact = factorial(factN);

        if (n % 2 == 1) {
            xk = -xk;
        }

        return xk / TComplex(fact, 0);
    }

    TComplex evaluate(const TComplex& x, int degree) const override {
        // Особый случай: x = 0
        if (x == TComplex(0, 0)) {
            return TComplex(1, 0); // lim x->0 sin(x)/x = 1
        }

        // Для Si нужны четные степени
        int terms = degree / 2 + 1;
        if (terms < 1) terms = 1;

        TComplex result(0, 0);
        for (int n = 0; n < terms; ++n) {
            result = result + term(x, n);
        }
        return result;
    }

    Polinom<TComplex> getMaclaurinPolynom(int degree) const override {
        // Создаем полином для Si(x)
        // Коэффициенты: a0 = 1, a2 = -1/6, a4 = 1/120, a6 = -1/5040, ...
        // Нечетные коэффициенты равны 0

        // Временно возвращаем пустой полином
        return Polinom<TComplex>(degree);
    }
};

#endif // ALLFUNCTIONS_H
