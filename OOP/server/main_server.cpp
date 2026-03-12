#include <QCoreApplication>
#include <QTimer>
#include "serverapplication.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    cout << "=== Запуск серверного приложения ===" << endl;

    ServerApplication server;

    // Запускаем серверное приложение через таймер, чтобы event loop успел запуститься
    QTimer::singleShot(0, &server, [&server]() {
        server.run();
    });

    return app.exec();
}
