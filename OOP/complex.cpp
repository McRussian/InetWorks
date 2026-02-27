#include "complex.h"
#include <cmath>
#include <sstream>
#include <string>

// Конструктор по умолчанию
TComplex::TComplex() : re(0.0), im(0.0) {}

// Конструктор с параметрами
TComplex::TComplex(double real, double imag) : re(real), im(imag) {}

// Геттеры
double TComplex::getRe() const {
    return re;
}

double TComplex::getIm() const {
    return im;
}

// Сеттеры
void TComplex::setRe(double real) {
    re = real;
}

void TComplex::setIm(double imag) {
    im = imag;
}

// Модуль (норма) комплексного числа
double TComplex::norm() const {
    return std::sqrt(re * re + im * im);
}

// Оператор сложения
TComplex TComplex::operator+(const TComplex& other) const {
    return TComplex(re + other.re, im + other.im);
}

// Оператор вычитания
TComplex TComplex::operator-(const TComplex& other) const {
    return TComplex(re - other.re, im - other.im);
}

// Оператор умножения
TComplex TComplex::operator*(const TComplex& other) const {
    // (a+bi)(c+di) = (ac - bd) + (ad + bc)i
    return TComplex(
        re * other.re - im * other.im,
        re * other.im + im * other.re
        );
}

// Оператор деления
TComplex TComplex::operator/(const TComplex& other) const {
    // (a+bi)/(c+di) = ((ac+bd)/(c^2+d^2)) + ((bc-ad)/(c^2+d^2))i
    double denominator = other.re * other.re + other.im * other.im;

    // Проверка деления на ноль
    if (std::abs(denominator) < 1e-10) {
        throw std::runtime_error("Division by zero complex number");
    }

    return TComplex(
        (re * other.re + im * other.im) / denominator,
        (im * other.re - re * other.im) / denominator
        );
}

// Оператор +=
TComplex& TComplex::operator+=(const TComplex& other) {
    re += other.re;
    im += other.im;
    return *this;
}

// Оператор -=
TComplex& TComplex::operator-=(const TComplex& other) {
    re -= other.re;
    im -= other.im;
    return *this;
}

// Оператор *=
TComplex& TComplex::operator*=(const TComplex& other) {
    double newRe = re * other.re - im * other.im;
    double newIm = re * other.im + im * other.re;
    re = newRe;
    im = newIm;
    return *this;
}

// Оператор /=
TComplex& TComplex::operator/=(const TComplex& other) {
    double denominator = other.re * other.re + other.im * other.im;

    if (std::abs(denominator) < 1e-10) {
        throw std::runtime_error("Division by zero complex number");
    }

    double newRe = (re * other.re + im * other.im) / denominator;
    double newIm = (im * other.re - re * other.im) / denominator;
    re = newRe;
    im = newIm;
    return *this;
}

// Унарный минус
TComplex TComplex::operator-() const {
    return TComplex(-re, -im);
}

// Оператор равенства (по норме с точностью)
bool TComplex::operator==(const TComplex& other) const {
    return std::abs(norm() - other.norm()) < 1e-10;
}

// Оператор неравенства
bool TComplex::operator!=(const TComplex& other) const {
    return !(*this == other);
}

// Оператор меньше
bool TComplex::operator<(const TComplex& other) const {
    return norm() < other.norm();
}

// Оператор меньше или равно
bool TComplex::operator<=(const TComplex& other) const {
    return norm() <= other.norm() + 1e-10;
}

// Оператор больше
bool TComplex::operator>(const TComplex& other) const {
    return norm() > other.norm();
}

// Оператор больше или равно
bool TComplex::operator>=(const TComplex& other) const {
    return norm() >= other.norm() - 1e-10;
}

// Оператор ввода
std::istream& operator>>(std::istream& in, TComplex& c) {
    // Поддерживаем форматы: "a+bi", "a-bi", "a", "i"
    std::string token;
    in >> token;

    // Простой парсинг комплексного числа
    double real = 0.0, imag = 0.0;
    char sign = '+';
    size_t i_pos = token.find('i');

    if (i_pos == std::string::npos) {
        // Нет мнимой части - это действительное число
        std::istringstream iss(token);
        iss >> real;
        if (iss.fail()) {
            in.setstate(std::ios::failbit);
            return in;
        }
    } else {
        // Есть мнимая часть
        std::string real_part = token.substr(0, i_pos);
        std::string imag_part;

        // Находим знак перед мнимой частью
        size_t sign_pos = real_part.find_last_of("+-");
        if (sign_pos != std::string::npos && sign_pos > 0) {
            sign = real_part[sign_pos];
            imag_part = real_part.substr(sign_pos + 1);
            real_part = real_part.substr(0, sign_pos);
        } else {
            imag_part = real_part;
            real_part = "0";
            sign = '+';
        }

        // Если после знака ничего нет или только 'i', значит мнимая часть = 1
        if (imag_part.empty() || imag_part == "i") {
            imag = 1.0;
        } else {
            // Убираем возможный 'i' в конце
            size_t i_in_imag = imag_part.find('i');
            if (i_in_imag != std::string::npos) {
                imag_part = imag_part.substr(0, i_in_imag);
            }
            std::istringstream iss_imag(imag_part);
            iss_imag >> imag;
            if (iss_imag.fail()) {
                in.setstate(std::ios::failbit);
                return in;
            }
        }

        // Парсим действительную часть
        if (!real_part.empty()) {
            std::istringstream iss_real(real_part);
            iss_real >> real;
            if (iss_real.fail()) {
                in.setstate(std::ios::failbit);
                return in;
            }
        }

        // Применяем знак
        if (sign == '-') {
            imag = -imag;
        }
    }

    c.re = real;
    c.im = imag;
    return in;
}

// Оператор вывода
std::ostream& operator<<(std::ostream& out, const TComplex& c) {
    if (std::abs(c.im) < 1e-10) {
        // Только действительная часть
        out << c.re;
    } else if (std::abs(c.re) < 1e-10) {
        // Только мнимая часть
        if (std::abs(c.im - 1.0) < 1e-10)
            out << "i";
        else if (std::abs(c.im + 1.0) < 1e-10)
            out << "-i";
        else
            out << c.im << "i";
    } else {
        // Обе части
        out << c.re;
        if (c.im > 0)
            out << "+" << c.im << "i";
        else if (c.im < 0)
            out << c.im << "i";
    }
    return out;
}

// Квадрат модуля
double TComplex::normSquared() const {
    return re * re + im * im;
}

// Модуль (вещественное число)
double TComplex::modulus() const {
    return std::sqrt(normSquared());
}

// Извлечение квадратного корня из комплексного числа
TComplex TComplex::sqrt() const {
    double r = modulus();
    if (r < 1e-10) return TComplex(0, 0);

    double theta = std::atan2(im, re);
    double sqrt_r = std::sqrt(r);

    return TComplex(
        sqrt_r * std::cos(theta / 2),
        sqrt_r * std::sin(theta / 2)
        );
}

// Глобальная функция sqrt
TComplex sqrt(const TComplex& c) {
    return c.sqrt();
}
