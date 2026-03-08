TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11

TARGET = common

# Подключаем Qt модули, необходимые для всех классов
QT += core

# Добавляем widgets для WindowApplication
QT += widgets

# Исходные файлы
HEADERS += \
    array.hpp \
    number.h \
    complex.h \
    application.h \
    polinom.hpp \
    windowapplication.h

SOURCES += \
    complex.cpp \
    application.cpp \
    windowapplication.cpp

# Пути для include
INCLUDEPATH += .

# Оптимизация для релиза
QMAKE_CXXFLAGS_RELEASE += -O2

# Для отладки
CONFIG(debug, debug|release) {
    TARGET = commond
} else {
    TARGET = common
}
