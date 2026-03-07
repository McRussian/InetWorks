TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

TARGET = server

# Подключаем общую библиотеку
INCLUDEPATH += ../common
LIBS += -L../common -lcommon

# Для отладочной сборки
CONFIG(debug, debug|release) {
    LIBS += -L../common -lcommond
} else {
    LIBS += -L../common -lcommon
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

# Зависимость от сборки общей библиотеки
PRE_TARGETDEPS += ../common/libcommon.a

# Для отладочной сборки
CONFIG(debug, debug|release) {
    PRE_TARGETDEPS += ../common/libcommond.a
}
