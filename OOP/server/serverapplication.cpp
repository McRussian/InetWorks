#include "serverapplication.h"
#include <QCoreApplication>
#include <iostream>

using namespace std;

// ==================== ServerWorker ====================

ServerWorker::ServerWorker(QObject* parent)
    : QObject(parent), communicator(nullptr), polinomExists(false)
{
    // Communicator создается здесь - уже в правильном потоке
    communicator = new Communicator(this);

    // Подключаем сигналы коммуникатора к слотам ServerWorker
    connect(communicator, &Communicator::polinomRequested,
            this, &ServerWorker::onPolinomRequested);

    connect(communicator, &Communicator::clientConnected,
            this, &ServerWorker::onClientConnected);

    connect(communicator, &Communicator::clientDisconnected,
            this, &ServerWorker::onClientDisconnected);

    connect(communicator, &Communicator::errorOccurred,
            this, [this](const QString& err) {
                emit logMessage("Ошибка: " + err);
            });
}

ServerWorker::~ServerWorker()
{
    // communicator удалится автоматически как дочерний объект
}

void ServerWorker::startServer(quint16 port)
{
    emit logMessage("Запуск сервера на порту " + QString::number(port));

    if (!communicator->start(port)) {
        emit serverError("Не удалось запустить коммуникатор");
        return;
    }

    emit serverStarted(port);
}

void ServerWorker::stopServer()
{
    if (communicator) {
        communicator->stop();
    }
}

void ServerWorker::updatePolinom(const Polinom& newPolinom, bool exists)
{
    polinom = newPolinom;
    polinomExists = exists;
}

void ServerWorker::onPolinomRequested()
{
    emit logMessage("Запрос полинома от клиента");
    communicator->sendPolinomToClient(polinom, polinomExists);
}

void ServerWorker::onClientConnected(const QHostAddress& address, quint16 port)
{
    emit logMessage("✓ Клиент подключен: " + address.toString() + ":" + QString::number(port));
    emit clientConnected(address, port);
}

void ServerWorker::onClientDisconnected()
{
    emit logMessage("✗ Клиент отключен");
    emit clientDisconnected();
}

// ==================== ServerApplication ====================

ServerApplication::ServerApplication(QObject* parent)
    : ConsoleApplication(parent), serverThread(nullptr), serverWorker(nullptr)
{
}

ServerApplication::~ServerApplication()
{
    if (serverThread) {
        serverThread->quit();
        serverThread->wait();
        delete serverThread;
    }
}

bool ServerApplication::start(quint16 port)
{
    cout << "===========================================================================\n";
    cout << "    СЕРВЕР ДЛЯ РАБОТЫ С ПОЛИНОМОМ\n";
    cout << "===========================================================================\n";

    // Создаем поток
    serverThread = new QThread(this);

    // Создаем worker (пока еще в главном потоке)
    serverWorker = new ServerWorker();

    // Подключаем сигналы от worker к главному потоку
    connect(serverWorker, &ServerWorker::serverStarted,
            this, &ServerApplication::onServerStarted, Qt::QueuedConnection);
    connect(serverWorker, &ServerWorker::serverError,
            this, &ServerApplication::onServerError, Qt::QueuedConnection);
    connect(serverWorker, &ServerWorker::clientConnected,
            this, &ServerApplication::onClientConnected, Qt::QueuedConnection);
    connect(serverWorker, &ServerWorker::clientDisconnected,
            this, &ServerApplication::onClientDisconnected, Qt::QueuedConnection);
    connect(serverWorker, &ServerWorker::logMessage,
            this, &ServerApplication::onLogMessage, Qt::QueuedConnection);

    // Перемещаем worker в отдельный поток
    serverWorker->moveToThread(serverThread);

    // Запускаем сервер в отдельном потоке
    connect(serverThread, &QThread::started,
            [=]() { serverWorker->startServer(port); });

    // Очистка при завершении потока
    connect(serverThread, &QThread::finished,
            serverWorker, &QObject::deleteLater);

    // Запускаем поток
    serverThread->start();

    return true;
}

void ServerApplication::onServerStarted(quint16 port)
{
    cout << "✓ Сервер запущен на порту " << port << endl;
    cout << "Серверное приложение готово к работе\n\n";
}

void ServerApplication::onServerError(const QString& error)
{
    cerr << "✗ Ошибка сервера: " << error.toStdString() << endl;
}

void ServerApplication::onClientConnected(const QHostAddress& address, quint16 port)
{
    cout << "\n✓ Клиент подключен: "
         << address.toString().toStdString()
         << ":" << port << endl;
}

void ServerApplication::onClientDisconnected()
{
    cout << "\n✗ Клиент отключен\n";
}

void ServerApplication::onLogMessage(const QString& message)
{
    cout << "[SERVER] " << message.toStdString() << endl;
}

void ServerApplication::run()
{
    // Запускаем меню в основном потоке
    while (running) {
        showMenu();
        cout << "> ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cout << "Ошибка ввода! Пожалуйста, введите число.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
        case 1:
            handleInput();
            // Отправляем обновленный полином в серверный поток
            if (serverWorker && polinomExists) {
                QMetaObject::invokeMethod(serverWorker, "updatePolinom",
                                          Qt::QueuedConnection,
                                          Q_ARG(Polinom, polinom),
                                          Q_ARG(bool, polinomExists));
            }
            break;
        case 2:
            handleChangeCoeffOrRoot();
            if (serverWorker && polinomExists) {
                QMetaObject::invokeMethod(serverWorker, "updatePolinom",
                                          Qt::QueuedConnection,
                                          Q_ARG(Polinom, polinom),
                                          Q_ARG(bool, polinomExists));
            }
            break;
        case 3:
            handleEvaluate();
            break;
        case 4:
            handleResize();
            if (serverWorker && polinomExists) {
                QMetaObject::invokeMethod(serverWorker, "updatePolinom",
                                          Qt::QueuedConnection,
                                          Q_ARG(Polinom, polinom),
                                          Q_ARG(bool, polinomExists));
            }
            break;
        case 5:
            handleDisplayForm1();
            break;
        case 6:
            handleDisplayForm2();
            break;
        case 7:
            handlePrintRoots();
            break;
        case 8:
            handlePrintCoeffs();
            break;
        case 0:
            cout << "Завершение работы сервера...\n";
            running = false;
            if (serverThread) {
                serverThread->quit();
                serverThread->wait();
            }
            QCoreApplication::quit();
            break;
        default:
            cout << "Неверная команда! Пожалуйста, выберите 0-8.\n";
        }
    }
}
