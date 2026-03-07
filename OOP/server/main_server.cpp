#include <QCoreApplication>
#include "serverapplication.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    quint16 port = 12345; // Порт по умолчанию

    if (argc > 1) {
        port = QString(argv[1]).toUShort();
    }

    ServerApplication serverApp;

    if (!serverApp.start(port)) {
        std::cerr << "Не удалось запустить серверное приложение" << std::endl;
        return 1;
    }

    // Запускаем меню в основном потоке
    serverApp.run();

    return app.exec();
}
