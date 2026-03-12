#include "serverapplication.h"
#include <iostream>
#include <sstream>
#include <QCoreApplication>
#include <QMetaMethod>

using namespace std;

// ==================== MenuWorker ====================

void MenuWorker::run()
{
    while (true) {
        int choice;
        cin >> choice;

        if (cin.fail()) {
            cout << "Ошибка ввода! Пожалуйста, введите число.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 1) {
            // Ввод полинома double
            cout << "\n=== Ввод нового полинома (double) ===\n";
            cout << "Формат: an degree r1 r2 ... rn\n";
            cout << "Пример: 2 3 1 2 3\n";
            cout << "Ваш ввод: ";

            double an;
            size_t degree;

            cin >> an >> degree;

            if (cin.fail()) {
                cout << "Ошибка ввода!\n";
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }

            DoublePolinomData data;
            data.an = an;
            data.degree = degree;

            bool ok = true;
            for (size_t i = 0; i < degree; ++i) {
                double root;
                cin >> root;
                if (cin.fail()) {
                    cout << "Ошибка ввода корня " << i << "!\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                    ok = false;
                    break;
                }
                data.roots.push_back(root);
            }

            if (ok) {
                emit doublePolinomDataReady(data);
            }
        }
        else if (choice == 2) {
            // Ввод полинома complex
            cout << "\n=== Ввод нового полинома (TComplex) ===\n";
            cout << "Формат: an degree r1 r2 ... rn\n";
            cout << "Пример: (1,0) 2 (1,0) (2,0)\n";
            cout << "Ваш ввод: ";

            // Очищаем буфер
            cin.clear();
            cin.ignore(10000, '\n');

            string line;
            getline(cin, line);

            while (line.empty()) {
                getline(cin, line);
            }

            stringstream ss(line);

            TComplex an;
            ss >> an;
            if (ss.fail()) {
                cout << "Ошибка ввода старшего коэффициента!\n";
                continue;
            }

            size_t degree;
            ss >> degree;
            if (ss.fail()) {
                cout << "Ошибка ввода степени!\n";
                continue;
            }

            ComplexPolinomData data;
            data.an = an;
            data.degree = degree;

            bool ok = true;
            for (size_t i = 0; i < degree; ++i) {
                TComplex root;
                ss >> root;
                if (ss.fail()) {
                    cout << "Ошибка ввода корня " << i << "!\n";
                    ok = false;
                    break;
                }
                data.roots.push_back(root);
            }

            if (ok) {
                emit complexPolinomDataReady(data);
            }
        }
        else {
            emit commandEntered(choice);
        }

        if (choice == 0) {
            emit finished();
            break;
        }
    }
}

// ==================== ServerApplication ====================

ServerApplication::ServerApplication(QObject* parent)
    : ConsoleApplication(parent), communicator(nullptr),
    serverThread(nullptr), menuThread(nullptr), menuWorker(nullptr)
{

    communicator = new Communicator(nullptr);

    // Подключаем сигналы коммуникатора
    connect(communicator, &Communicator::polinomRequested,
            this, &ServerApplication::onPolinomRequested, Qt::QueuedConnection);
    connect(communicator, &Communicator::clientConnected,
            this, &ServerApplication::onClientConnected, Qt::QueuedConnection);
    connect(communicator, &Communicator::clientDisconnected,
            this, &ServerApplication::onClientDisconnected, Qt::QueuedConnection);
    connect(communicator, &Communicator::dataTypeReceived,
            this, &ServerApplication::onDataTypeReceived, Qt::QueuedConnection);
    connect(communicator, &Communicator::errorOccurred,
            this, &ServerApplication::onError, Qt::QueuedConnection);

    const QMetaObject* meta = metaObject();
    int index = meta->indexOfSlot("onPolinomRequested()");
    if (index >= 0) {
        cout << "[ServerApplication]   Слот onPolinomRequested найден, индекс: " << index << endl;
    }
}

ServerApplication::~ServerApplication()
{
    stopServer();

    if (menuThread) {
        menuThread->quit();
        menuThread->wait();
        delete menuThread;
    }
}

bool ServerApplication::startServer(quint16 port)
{
    // Создаем поток для коммуникатора
    serverThread = new QThread(this);

    // Перемещаем коммуникатор в поток
    communicator->moveToThread(serverThread);

    // Запускаем сервер в потоке
    connect(serverThread, &QThread::started, [=]() {
        communicator->start(port);
    });

    // Очистка при завершении потока
    connect(serverThread, &QThread::finished, [=]() {
        communicator->deleteLater();
    });

    serverThread->start();
    return true;
}

void ServerApplication::stopServer()
{

    if (serverThread && serverThread->isRunning()) {
        // Останавливаем коммуникатор
        if (communicator) {
            QMetaObject::invokeMethod(communicator, "stop", Qt::QueuedConnection);
        }
        serverThread->quit();

        // Ждем завершения потока
        if (!serverThread->wait(3000)) {
            serverThread->terminate();
            serverThread->wait();
        }

        delete serverThread;
        serverThread = nullptr;
    }
}

void ServerApplication::onClientConnected(const QHostAddress& address, quint16 port)
{
}

void ServerApplication::onClientDisconnected()
{

}

void ServerApplication::onDataTypeReceived(int type)
{
}

void ServerApplication::onError(const QString& error)
{
}

void ServerApplication::onPolinomRequested()
{

    if (!polinomExists) {
        if (currentType == 1) {
            Polinom<double> empty;
            communicator->sendPolinomToClient(empty, false);
        } else {
            Polinom<TComplex> empty;
            communicator->sendPolinomToClient(empty, false);
        }
        return;
    }

    // Отправляем полином соответствующего типа
    if (currentType == 1 && realPolinom) {
        const Array<double>& roots = realPolinom->getRoots();
        for (size_t i = 0; i < roots.getSize(); ++i) {
            cout << roots[i] << " ";
        }
        cout << endl;

        if (communicator) {
            communicator->sendPolinomToClient(*realPolinom, true);
        } else {
        }
    } else if (currentType == 2 && complexPolinom) {
        const Array<TComplex>& roots = complexPolinom->getRoots();
        for (size_t i = 0; i < roots.getSize(); ++i) {
            cout << roots[i] << " ";
        }
        cout << endl;

        if (communicator) {
            communicator->sendPolinomToClient(*complexPolinom, true);
        } else {
            cout << "[ServerApplication]   → ОШИБКА: communicator = nullptr" << endl;
        }
    } else {
        cout << "[ServerApplication]   → Ошибка: несоответствие типа" << endl;
    }
}

void ServerApplication::onCommandEntered(int cmd)
{

    switch (cmd) {
    case 1: handleInput(); break;
    case 2: handleChangeCoeffOrRoot(); break;
    case 3: handleEvaluate(); break;
    case 4: handleResize(); break;
    case 5: handleDisplayForm1(); break;
    case 6: handleDisplayForm2(); break;
    case 7: handlePrintRoots(); break;
    case 8: handlePrintCoeffs(); break;
    case 9: selectDataType(); break;
    case 0:
        cout << "Завершение работы...\n";
        running = false;
        stopServer();
        QCoreApplication::quit();
        break;
    default:
        cout << "Неверная команда!\n";
    }
}

void ServerApplication::onDoublePolinomDataReceived(const DoublePolinomData& data)
{
    for (double r : data.roots) {
        cout << r << " ";
    }
    cout << endl;

    Array<double> roots;
    for (double r : data.roots) {
        roots.pushBack(r);
    }

    if (realPolinom) delete realPolinom;
    realPolinom = new Polinom<double>(data.an, roots);
    polinomExists = true;
}

void ServerApplication::onComplexPolinomDataReceived(const ComplexPolinomData& data)
{
    Array<TComplex> roots;
    for (const TComplex& r : data.roots) {
        roots.pushBack(r);
    }

    if (complexPolinom) delete complexPolinom;
    complexPolinom = new Polinom<TComplex>(data.an, roots);
    polinomExists = true;
}

// ==================== Переопределенные методы ====================

void ServerApplication::handleInput()
{
    ConsoleApplication::handleInput();

    // Уведомляем коммуникатор о новом типе данных
    if (communicator && communicator->isClientConnected()) {
        communicator->setDataType(currentType);
    }
}

void ServerApplication::handleChangeCoeffOrRoot()
{
    ConsoleApplication::handleChangeCoeffOrRoot();

    // Уведомляем о изменении полинома
    if (polinomExists && communicator && communicator->isClientConnected()) {
        if (currentType == 1) {
            communicator->sendPolinomToClient(*realPolinom, true);
        } else {
            communicator->sendPolinomToClient(*complexPolinom, true);
        }
    }
}

void ServerApplication::handleResize()
{
    ConsoleApplication::handleResize();

    // Уведомляем о изменении полинома
    if (polinomExists && communicator && communicator->isClientConnected()) {
        if (currentType == 1) {
            communicator->sendPolinomToClient(*realPolinom, true);
        } else {
            communicator->sendPolinomToClient(*complexPolinom, true);
        }
    }
}

void ServerApplication::handleEvaluate()
{
    ConsoleApplication::handleEvaluate();
}

void ServerApplication::handleDisplayForm1()
{
    ConsoleApplication::handleDisplayForm1();
}

void ServerApplication::handleDisplayForm2()
{
    ConsoleApplication::handleDisplayForm2();
}

void ServerApplication::handlePrintRoots()
{
    ConsoleApplication::handlePrintRoots();
}

void ServerApplication::handlePrintCoeffs()
{
    ConsoleApplication::handlePrintCoeffs();
}

void ServerApplication::run()
{
    // Запускаем сервер
    startServer(12345);

    // Создаем поток для меню
    menuThread = new QThread(this);
    menuWorker = new MenuWorker();
    menuWorker->moveToThread(menuThread);

    // Подключаем сигналы от menuWorker
    connect(menuThread, &QThread::started, menuWorker, &MenuWorker::run);
    connect(menuWorker, &MenuWorker::commandEntered,
            this, &ServerApplication::onCommandEntered, Qt::QueuedConnection);
    connect(menuWorker, &MenuWorker::doublePolinomDataReady,
            this, &ServerApplication::onDoublePolinomDataReceived, Qt::QueuedConnection);
    connect(menuWorker, &MenuWorker::complexPolinomDataReady,
            this, &ServerApplication::onComplexPolinomDataReceived, Qt::QueuedConnection);
    connect(menuWorker, &MenuWorker::finished, menuThread, &QThread::quit);
    connect(menuThread, &QThread::finished, menuWorker, &QObject::deleteLater);

    // Запускаем выбор типа данных в главном потоке
    selectDataType();

    // Запускаем поток меню
    menuThread->start();
}
