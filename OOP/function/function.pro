TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle

TARGET = function

# Подключаем общую библиотеку
INCLUDEPATH += ../common

# В зависимости от конфигурации подключаем нужную библиотеку
CONFIG(debug, debug|release) {
    # Отладочная сборка
    LIBS += -L../common -lcommond
    PRE_TARGETDEPS += ../common/libcommond.a
    message("Using debug library: libcommond.a")
} else {
    # Релизная сборка
    LIBS += -L../common -lcommon
    PRE_TARGETDEPS += ../common/libcommon.a
    message("Using release library: libcommon.a")
}

# Исходные файлы
HEADERS += \
    function.hpp \
    functioncalculator.h \
    functionplotter.h

SOURCES += \
    functioncalculator.cpp \
    functionplotter.cpp \
    main_function.cpp

# Qt модули
QT += core widgets charts
