#include "polinom.h"
#include <algorithm>
#include <iostream>

// Конструктор по умолчанию - полином 0-й степени (константа 0)
Polinom::Polinom() : roots(), leadingCoeff(0.0), coefficients() {
    coefficients = Array();
}

// Полином n-й степени с корнями = 0 (все корни нулевые)
Polinom::Polinom(size_t n) : roots(n, number(0.0)), leadingCoeff(1.0), coefficients() {
    calculateCoefficients();
}

// Полином n-й степени с заданными корнями
Polinom::Polinom(size_t n, number an, const Array& rootArray)
    : roots(n), leadingCoeff(an), coefficients() {

    // Проверка на пустой массив корней
    if (n > 0) {
        // Копируем корни из переданного массива (сколько есть)
        size_t copySize = std::min(n, rootArray.getSize());
        for (size_t i = 0; i < copySize; ++i) {
            roots[i] = rootArray[i];
        }
        // Оставшиеся корни (если n > rootArray.getSize()) уже инициализированы 0 конструктором Array
    }

    calculateCoefficients();
}

// Полином степени rootArray.getSize() с заданными корнями
Polinom::Polinom(number an, const Array& rootArray)
    : roots(rootArray), leadingCoeff(an), coefficients() {
    calculateCoefficients();
}

// Конструктор копирования
Polinom::Polinom(const Polinom& other)
    : roots(other.roots), leadingCoeff(other.leadingCoeff), coefficients(other.coefficients) {}

// Оператор присваивания
Polinom& Polinom::operator=(const Polinom& other) {
    if (this != &other) {
        roots = other.roots;
        leadingCoeff = other.leadingCoeff;
        coefficients = other.coefficients;
    }
    return *this;
}

// Получить степень полинома
size_t Polinom::getDegree() const {
    return roots.getSize();
}

// Получить старший коэффициент
number Polinom::getLeadingCoeff() const {
    return leadingCoeff;
}

// Получить корень по индексу с проверкой
number Polinom::getRoot(size_t index) const {
    if (index >= roots.getSize()) {
        throw std::out_of_range("Polinom::getRoot: index out of range");
    }
    return roots[index];
}

// Получить массив корней
const Array& Polinom::getRoots() const {
    return roots;
}

// Получить коэффициент при x^power с проверкой
number Polinom::getCoefficient(size_t power) const {
    if (coefficients.getSize() == 0) {
        return number(0.0);
    }
    if (power < coefficients.getSize()) {
        return coefficients[power];
    }
    return number(0.0); // Для степеней выше текущей возвращаем 0
}

// Установить старший коэффициент
void Polinom::setLeadingCoeff(number an) {
    leadingCoeff = an;
    calculateCoefficients(); // Пересчитываем коэффициенты
}

// Установить корень по индексу с проверкой
void Polinom::setRoot(size_t index, number value) {
    if (index >= roots.getSize()) {
        throw std::out_of_range("Polinom::setRoot: index out of range");
    }
    roots[index] = value;
    calculateCoefficients(); // Пересчитываем коэффициенты
}

// Изменить степень полинома
void Polinom::resize(size_t newDegree) {
    roots.reserve(newDegree);
    // При изменении степени пересчитываем коэффициенты
    calculateCoefficients();
}

// Вычисление коэффициентов полинома из корней
void Polinom::calculateCoefficients() {
    size_t n = roots.getSize();

    if (n == 0) {
        // Полином 0-й степени (константа)
        coefficients = Array(1);
        coefficients[0] = leadingCoeff;
        return;
    }

    // Алгоритм: раскрываем скобки (x - r1)(x - r2)...(x - rn)
    // Коэффициенты хранятся от младшего к старшему: c0 + c1*x + ... + cn*x^n
    Array coeff(n + 1);

    // Инициализируем нулями
    for (size_t i = 0; i <= n; ++i) {
        coeff.pushBack(number(0.0));
    }

    // Начинаем с полинома (x - r1) = -r1 + x
    coeff[0] = -roots[0];
    coeff[1] = number(1.0);

    // Последовательно умножаем на (x - ri)
    for (size_t i = 1; i < n; ++i) {
        // Текущая степень: i+1
        // Умножаем на (x - roots[i])

        // Создаём новый массив коэффициентов
        Array newCoeff(i + 2);

        // Инициализируем нулями
        for (size_t j = 0; j <= i + 1; ++j) {
            newCoeff.pushBack(number(0.0));
        }

        // Умножение на x: сдвиг коэффициентов вверх
        for (size_t j = 0; j <= i; ++j) {
            newCoeff[j + 1] = newCoeff[j + 1] + coeff[j];
        }

        // Умножение на -roots[i]: добавление без сдвига
        for (size_t j = 0; j <= i; ++j) {
            newCoeff[j] = newCoeff[j] + (-roots[i]) * coeff[j];
        }

        coeff = newCoeff;
    }

    // Умножаем на старший коэффициент
    for (size_t i = 0; i <= n; ++i) {
        coeff[i] = leadingCoeff * coeff[i];
    }

    coefficients = coeff;
}

// Вычисление значения полинома в точке x (схема Горнера)
number Polinom::evaluate(number x) const {
    size_t n = roots.getSize();

    if (n == 0) {
        return leadingCoeff; // Константа
    }

    // Схема Горнера для разложения по корням: an * (x - r1) * (x - r2) * ... * (x - rn)
    number result = leadingCoeff;
    for (size_t i = 0; i < n; ++i) {
        result = result * (x - roots[i]);
    }
    return result;
}

// Оператор вызова для вычисления значения
number Polinom::operator()(number x) const {
    return evaluate(x);
}

// Вывод в форме an*x^n + ... + a0
void Polinom::printForm1(std::ostream& out) const {
    size_t n = coefficients.getSize();

    if (n == 0) {
        out << "0";
        return;
    }

    // Проверка на нулевой полином (все коэффициенты равны 0)
    bool isZero = true;
    for (size_t i = 0; i < n; ++i) {
        if (!(coefficients[i] == number(0.0))) {
            isZero = false;
            break;
        }
    }

    if (isZero) {
        out << "0";
        return;
    }

    // Вывод от старшей степени к младшей
    bool first = true;
    for (size_t i = n - 1; i > 0; --i) {
        number coeff = coefficients[i];

        // Пропускаем нулевые коэффициенты
        if (coeff == number(0.0)) continue;

        if (first) {
            // Первый ненулевой коэффициент
            out << coeff;
            first = false;
        } else {
            // Последующие коэффициенты со знаком
            if (coeff > number(0.0)) {
                out << " + " << coeff;
            } else {
                out << " - " << (-coeff);
            }
        }

        if (i > 0) {
            out << "x";
            if (i > 1) out << "^" << i;
        }
    }

    // Свободный член
    number a0 = coefficients[0];
    if (!(a0 == number(0.0))) {
        if (first) {
            // Только свободный член
            out << a0;
        } else {
            if (a0 > number(0.0)) {
                out << " + " << a0;
            } else {
                out << " - " << (-a0);
            }
        }
    }
}

// Вывод в форме an*(x - r1)*...*(x - rn)
void Polinom::printForm2(std::ostream& out) const {
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

// Оператор вывода (по умолчанию форма 1)
std::ostream& operator<<(std::ostream& out, const Polinom& p) {
    p.printForm1(out);
    return out;
}

// Оператор ввода
std::istream& operator>>(std::istream& in, Polinom& p) {
    number an;

    // Читаем старший коэффициент
    in >> an;

    // Создаем временный массив для корней, используя оператор >> для Array
    // Он прочитает размер (степень) и сами корни
    Array roots;
    in >> roots;

    p = Polinom(an, roots);
    return in;
}
