#ifndef APPLICATION_H
#define APPLICATION_H

#include "array.h"

class ConsoleApplication {
private:
    Array array;           // Текущий массив
    bool running;          // Флаг работы приложения

    void showMenu();                    // Показать меню
    void handleInput();                  // Ввод нового массива
    void handleCalculate();               // Рассчитать статистику
    void handleSort();                    // Сортировка
    void handleReserve();                 // Изменение capacity (резервирование памяти)
    void handlePushBack();                // Добавление элемента в конец
    void handlePopBack();                 // Удаление элемента с конца
    void handleChangeElement();            // Изменение элемента
    void handleDisplay();                  // Вывод информации о массиве

public:
    ConsoleApplication();                  // Конструктор
    void run();                            // Запуск приложения
};

#endif // APPLICATION_H
