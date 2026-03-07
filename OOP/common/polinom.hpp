#ifndef POLINOM_HPP
#define POLINOM_HPP

#include <iostream>
#include <algorithm>
#include "array.hpp"

template <typename T>
class Polinom {
private:
    Array<T> roots;
    T leadingCoeff;
    mutable Array<T> coefficients;
    mutable bool coeffValid;

    void calculateCoefficients() const {
        size_t n = roots.getSize();

        if (n == 0) {
            coefficients = Array<T>(1);
            coefficients[0] = leadingCoeff;
            coeffValid = true;
            return;
        }

        // Используем push_back для построения коэффициентов
        Array<T> coeff;

        // Начинаем с полинома (x - r1) = -r1 + x
        coeff.pushBack(-roots[0]);  // a0 = -r1
        coeff.pushBack(T(1));        // a1 = 1

        for (size_t i = 1; i < n; ++i) {
            Array<T> newCoeff;

            // Умножение на x: сдвиг коэффициентов вверх
            for (size_t j = 0; j < coeff.getSize(); ++j) {
                if (j + 1 >= newCoeff.getSize()) {
                    newCoeff.pushBack(coeff[j]);
                } else {
                    newCoeff[j + 1] = newCoeff[j + 1] + coeff[j];
                }
            }

            // Умножение на -roots[i]: добавление без сдвига
            for (size_t j = 0; j < coeff.getSize(); ++j) {
                if (j >= newCoeff.getSize()) {
                    newCoeff.pushBack((-roots[i]) * coeff[j]);
                } else {
                    newCoeff[j] = newCoeff[j] + (-roots[i]) * coeff[j];
                }
            }

            coeff = newCoeff;
        }

        // Умножаем на старший коэффициент
        coefficients = Array<T>(coeff.getSize());
        for (size_t i = 0; i < coeff.getSize(); ++i) {
            coefficients.pushBack(leadingCoeff * coeff[i]);
        }

        coeffValid = true;
    }

public:
    // Конструктор по умолчанию - полином 0-й степени (константа 0)
    Polinom() : roots(), leadingCoeff(T(0)), coefficients(), coeffValid(false) {
        coefficients = Array<T>();
        coeffValid = true;
    }

    // Конструктор с размером - полином степени n с корнями = 0
    explicit Polinom(size_t n) : roots(), leadingCoeff(T(1)), coefficients(), coeffValid(false) {
        for (size_t i = 0; i < n; ++i) {
            roots.pushBack(T(0));
        }
        if (n == 0) {
            coefficients = Array<T>(1);
            coefficients[0] = T(1);
            coeffValid = true;
        } else {
            calculateCoefficients();
        }
    }

    // Конструктор с корнями и старшим коэффициентом
    Polinom(size_t n, const T& an, const Array<T>& rootArray)
        : roots(), leadingCoeff(an), coefficients(), coeffValid(false) {

        size_t copySize = std::min(n, rootArray.getSize());
        for (size_t i = 0; i < copySize; ++i) {
            roots.pushBack(rootArray[i]);
        }
        for (size_t i = copySize; i < n; ++i) {
            roots.pushBack(T(0));
        }
        calculateCoefficients();
    }

    // Конструктор от массива корней
    Polinom(const T& an, const Array<T>& rootArray)
        : roots(), leadingCoeff(an), coefficients(), coeffValid(false) {

        for (size_t i = 0; i < rootArray.getSize(); ++i) {
            roots.pushBack(rootArray[i]);
        }
        calculateCoefficients();
    }

    // Конструктор копирования
    Polinom(const Polinom& other)
        : roots(other.roots), leadingCoeff(other.leadingCoeff),
        coefficients(other.coefficients), coeffValid(other.coeffValid) {}

    // Оператор присваивания
    Polinom& operator=(const Polinom& other) {
        if (this != &other) {
            roots = other.roots;
            leadingCoeff = other.leadingCoeff;
            coefficients = other.coefficients;
            coeffValid = other.coeffValid;
        }
        return *this;
    }

    // Геттеры
    size_t getDegree() const { return roots.getSize(); }

    T getLeadingCoeff() const { return leadingCoeff; }

    T getRoot(size_t index) const {
        if (index >= roots.getSize()) {
            throw std::out_of_range("Polinom::getRoot: index out of range");
        }
        return roots[index];
    }

    T getCoefficient(size_t power) const {
        if (!coeffValid) {
            calculateCoefficients();
        }
        if (power < coefficients.getSize()) {
            return coefficients[power];
        }
        return T(0);
    }

    const Array<T>& getRoots() const { return roots; }

    // Сеттеры
    void setLeadingCoeff(const T& an) {
        leadingCoeff = an;
        coeffValid = false;
    }

    void setRoot(size_t index, const T& value) {
        if (index >= roots.getSize()) {
            throw std::out_of_range("Polinom::setRoot: index out of range");
        }
        // Временно: для доступа по индексу нужно, чтобы массив уже имел этот размер
        // Но мы используем pushBack для добавления, поэтому нужно создать новый массив
        Array<T> newRoots;
        for (size_t i = 0; i < roots.getSize(); ++i) {
            if (i == index) {
                newRoots.pushBack(value);
            } else {
                newRoots.pushBack(roots[i]);
            }
        }
        roots = newRoots;
        coeffValid = false;
    }

    // Изменение размерности (степени полинома)
    void resize(size_t newDegree) {
        Array<T> newRoots;
        size_t oldDegree = roots.getSize();

        for (size_t i = 0; i < std::min(oldDegree, newDegree); ++i) {
            newRoots.pushBack(roots[i]);
        }
        for (size_t i = oldDegree; i < newDegree; ++i) {
            newRoots.pushBack(T(0));
        }

        roots = newRoots;
        coeffValid = false;
    }

    // Вычисление значения полинома в точке x (схема Горнера)
    T evaluate(const T& x) const {
        size_t n = roots.getSize();

        if (n == 0) {
            return leadingCoeff; // Константа
        }

        T result = leadingCoeff;
        for (size_t i = 0; i < n; ++i) {
            result = result * (x - roots[i]);
        }
        return result;
    }

    // Оператор вызова для вычисления значения
    T operator()(const T& x) const {
        return evaluate(x);
    }

    // Формы представления
    void printForm1(std::ostream& out) const {
        if (!coeffValid) {
            calculateCoefficients();
        }

        size_t n = coefficients.getSize();

        bool isZero = true;
        for (size_t i = 0; i < n; ++i) {
            if (!(coefficients[i] == T(0))) {
                isZero = false;
                break;
            }
        }

        if (isZero) {
            out << "0";
            return;
        }

        bool first = true;
        for (size_t i = n - 1; i > 0; --i) {
            T coeff = coefficients[i];
            if (coeff == T(0)) continue;

            if (!first) {
                if (coeff > T(0)) out << " + ";
                else out << " - ";
            } else {
                first = false;
                if (coeff < T(0)) out << "-";
            }

            T absCoeff = coeff;
            if (absCoeff < T(0)) absCoeff = -absCoeff;

            if (!(absCoeff == T(1)) || i == 0) {
                out << absCoeff;
            }

            if (i > 0) {
                out << "x";
                if (i > 1) out << "^" << i;
            }
        }

        T a0 = coefficients[0];
        if (!(a0 == T(0))) {
            if (!first) {
                if (a0 > T(0)) out << " + ";
                else out << " - ";
            } else {
                first = false;
                if (a0 < T(0)) out << "-";
            }

            T absA0 = a0;
            if (absA0 < T(0)) absA0 = -absA0;
            out << absA0;
        }
    }

    void printForm2(std::ostream& out) const {
        size_t n = roots.getSize();

        if (n == 0) {
            out << leadingCoeff;
            return;
        }

        out << leadingCoeff;

        for (size_t i = 0; i < n; ++i) {
            out << " * (x - " << roots[i] << ")";
        }
    }

    // Операторы ввода/вывода
    template <typename U>
    friend std::istream& operator>>(std::istream& in, Polinom<U>& p);

    template <typename U>
    friend std::ostream& operator<<(std::ostream& out, const Polinom<U>& p);
};

// Оператор ввода
template <typename T>
std::istream& operator>>(std::istream& in, Polinom<T>& p) {
    T an;
    size_t degree;

    in >> an >> degree;

    Array<T> roots;
    for (size_t i = 0; i < degree; ++i) {
        T root;
        in >> root;
        roots.pushBack(root);
    }

    p = Polinom<T>(an, roots);
    return in;
}

// Оператор вывода (по умолчанию форма 1)
template <typename T>
std::ostream& operator<<(std::ostream& out, const Polinom<T>& p) {
    p.printForm1(out);
    return out;
}

#endif // POLINOM_HPP
