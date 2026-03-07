#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <iostream>
#include <stdexcept>
#include <algorithm>

template <typename T>
class Array {
private:
    T* data;
    size_t capacity;
    size_t size;

public:
    // Конструкторы
    Array() : data(nullptr), capacity(0), size(0) {}

    explicit Array(size_t n) : capacity(n), size(0) {
        if (n > 0) {
            data = new T[capacity];
        } else {
            data = nullptr;
        }
    }

    Array(size_t n, const T& value) : capacity(n), size(n) {
        if (n > 0) {
            data = new T[capacity];
            for (size_t i = 0; i < size; ++i) {
                data[i] = value;
            }
        } else {
            data = nullptr;
        }
    }

    // Конструктор копирования
    Array(const Array& other) : capacity(other.capacity), size(other.size) {
        if (capacity > 0) {
            data = new T[capacity];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        } else {
            data = nullptr;
        }
    }

    // Деструктор
    ~Array() {
        delete[] data;
    }

    // Оператор присваивания
    Array& operator=(const Array& other) {
        if (this != &other) {
            delete[] data;

            capacity = other.capacity;
            size = other.size;

            if (capacity > 0) {
                data = new T[capacity];
                for (size_t i = 0; i < size; ++i) {
                    data[i] = other.data[i];
                }
            } else {
                data = nullptr;
            }
        }
        return *this;
    }

    // Изменение capacity
    void reserve(size_t newCapacity) {
        if (newCapacity < size) {
            throw std::runtime_error("Cannot reduce capacity below current size");
        }

        if (newCapacity == capacity) {
            return;
        }

        T* newData = (newCapacity > 0) ? new T[newCapacity] : nullptr;

        for (size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    // Работа с размером
    size_t getSize() const { return size; }
    size_t getCapacity() const { return capacity; }
    bool isEmpty() const { return size == 0; }

    // Добавление/удаление в конец
    void pushBack(const T& value) {
        if (size == capacity) {
            // Автоматически увеличиваем capacity при необходимости
            size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
            reserve(newCapacity);
        }
        data[size++] = value;
    }

    void popBack() {
        if (size == 0) {
            throw std::runtime_error("Cannot pop from empty array");
        }
        --size;
    }

    // Доступ к элементам
    T& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Array::operator[]: index out of range");
        }
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Array::operator[]: index out of range");
        }
        return data[index];
    }

    // Статистические вычисления
    T mean() const {
        if (size == 0) return T(0);

        T sum = T(0);
        for (size_t i = 0; i < size; ++i) {
            sum = sum + data[i];
        }
        return sum / static_cast<T>(size);
    }

    T stdDeviation() const {
        if (size <= 1) return T(0);

        T avg = mean();
        T sumSq = T(0);

        for (size_t i = 0; i < size; ++i) {
            T diff = data[i] - avg;
            sumSq = sumSq + diff * diff;
        }

        T meanSq = sumSq / static_cast<T>(size - 1);
        return sqrt(meanSq);
    }

    // Сортировка
    void sortAscending() {
        if (size > 0) {
            std::sort(data, data + size, [](const T& a, const T& b) { return a < b; });
        }
    }

    void sortDescending() {
        if (size > 0) {
            std::sort(data, data + size, [](const T& a, const T& b) { return a > b; });
        }
    }

    // Ввод/вывод
    template <typename U>
    friend std::istream& operator>>(std::istream& in, Array<U>& arr);

    template <typename U>
    friend std::ostream& operator<<(std::ostream& out, const Array<U>& arr);
};

// Оператор ввода
template <typename T>
std::istream& operator>>(std::istream& in, Array<T>& arr) {
    size_t n;
    in >> n;

    // Очищаем текущий массив
    delete[] arr.data;

    arr.capacity = n;
    arr.size = n;
    arr.data = (n > 0) ? new T[n] : nullptr;

    for (size_t i = 0; i < n; ++i) {
        in >> arr.data[i];
    }

    return in;
}

// Оператор вывода
template <typename T>
std::ostream& operator<<(std::ostream& out, const Array<T>& arr) {
    out << "[";
    for (size_t i = 0; i < arr.size; ++i) {
        if (i > 0) out << ", ";
        out << arr.data[i];
    }
    out << "]";
    return out;
}

#endif // ARRAY_HPP
