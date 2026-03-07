#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include "number.h"

class Array {
private:
    number* data;      // Указатель на динамический массив
    size_t capacity;   // Выделенная память
    size_t size;       // Текущее количество элементов

public:
    // Конструкторы
    Array();                                   // Пустой массив (capacity = 0, size = 0)
    explicit Array(size_t n);                   // Выделить память на n элементов (size = 0)
    Array(size_t n, number value);              // Создать массив с n элементами = value (size = n, capacity = n)

    // Правило трёх
    Array(const Array& other);
    ~Array();
    Array& operator=(const Array& other);

    // Изменение capacity
    void reserve(size_t newCapacity);            // Изменить capacity (нельзя меньше size)

    // Работа с размером (size)
    size_t getSize() const;
    size_t getCapacity() const;
    bool isEmpty() const;

    // Добавление/удаление в конец
    void pushBack(number value);                 // Добавить элемент в конец
    void popBack();                              // Удалить последний элемент

    // Доступ к элементам (только в пределах size)
    number& operator[](size_t index);
    const number& operator[](size_t index) const;

    // Статистические вычисления
    number mean() const;
    number stdDeviation() const;

    // Сортировка
    void sortAscending();
    void sortDescending();

    // Ввод/вывод
    friend std::istream& operator>>(std::istream& in, Array& arr);
    friend std::ostream& operator<<(std::ostream& out, const Array& arr);
};

#endif // ARRAY_H
