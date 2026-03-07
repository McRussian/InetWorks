TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle

TARGET = client

# Подключаем общую библиотеку
INCLUDEPATH += ../common
LIBS += -L../common -lcommon

# Для отладочной сборки
CONFIG(debug, debug|release) {
    LIBS += -L../common -lcommond
} else {
    LIBS += -L../common -lcommon
}

# Исходные файлы клиента
HEADERS += \
    clientcommunicator.h \
    clientapplication.h

SOURCES += \
    clientcommunicator.cpp \
    clientapplication.cpp \
    main_client.cpp

# Qt модули
QT += core network widgets

# Зависимость от сборки общей библиотеки
PRE_TARGETDEPS += ../common/libcommon.a

# Для отладочной сборки
CONFIG(debug, debug|release) {
    PRE_TARGETDEPS += ../common/libcommond.a
}
