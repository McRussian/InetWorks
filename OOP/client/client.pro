TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle

TARGET = client

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

# Исходные файлы клиента - ТОЛЬКО свои файлы
HEADERS += \
    clientapplication.h \
    clientcommunicator.h

SOURCES += \
    clientapplication.cpp \
    clientcommunicator.cpp \
    main_client.cpp

# Qt модули
QT += core widgets network
