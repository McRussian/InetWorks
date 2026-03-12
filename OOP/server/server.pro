TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

TARGET = server

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

# Исходные файлы сервера
HEADERS += \
    communicator.h \
    serverapplication.h

SOURCES += \
    communicator.cpp \
    serverapplication.cpp \
    main_server.cpp

# Qt модули
QT += core network

# Для корректной работы с потоками
DEFINES += QT_USE_QSTRINGBUILDER
