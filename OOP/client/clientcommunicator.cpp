#include "clientcommunicator.h"
#include <iostream>
#include <sstream>
#include <QHostInfo>

using namespace std;

ClientCommunicator::ClientCommunicator(QObject* parent)
    : QObject(parent), udpSocket(nullptr), serverPort(0), isConnected(false), currentType(1)
{
    cout << "[ClientCommunicator] Создан" << endl;

    udpSocket = new QUdpSocket(this);
    timeoutTimer = new QTimer(this);
    timeoutTimer->setSingleShot(true);

    connect(udpSocket, &QUdpSocket::readyRead, this, &ClientCommunicator::onReadyRead);
    connect(timeoutTimer, &QTimer::timeout, this, &ClientCommunicator::onTimeout);
}

ClientCommunicator::~ClientCommunicator()
{
    cout << "[ClientCommunicator] Уничтожен" << endl;
    disconnectFromServer();
}

bool ClientCommunicator::connectToServer(const QHostAddress& address, quint16 port, int type)
{
    cout << "\n[ClientCommunicator] connectToServer()" << endl;

    // Нормализуем адрес (IPv4)
    QHostAddress normalizedAddress;
    if (address.protocol() == QAbstractSocket::IPv6Protocol) {
        QString addrStr = address.toString();
        if (addrStr.startsWith("::ffff:")) {
            QString ipv4Str = addrStr.mid(7);
            normalizedAddress = QHostAddress(ipv4Str);
        } else {
            normalizedAddress = address;
        }
    } else {
        normalizedAddress = address;
    }

    serverAddress = normalizedAddress;
    serverPort = port;
    currentType = type;

    cout << "[ClientCommunicator] Подключение к серверу "
         << serverAddress.toString().toStdString() << ":" << port
         << " (тип: " << (type == 1 ? "double" : "complex") << ")" << endl;

    // Отправляем тип данных
    QString typeMsg = (type == 1) ? "TYPE:DOUBLE" : "TYPE:COMPLEX";
    cout << "[ClientCommunicator] Отправка: " << typeMsg.toStdString() << endl;

    qint64 sent = udpSocket->writeDatagram(typeMsg.toUtf8(), serverAddress, serverPort);

    if (sent == -1) {
        cout << "[ClientCommunicator] Ошибка отправки TYPE: "
             << udpSocket->errorString().toStdString() << endl;
        return false;
    }

    // Отправляем HELLO
    QByteArray helloMsg = "HELLO";
    cout << "[ClientCommunicator] Отправка: HELLO" << endl;

    sent = udpSocket->writeDatagram(helloMsg, serverAddress, serverPort);

    if (sent == -1) {
        cout << "[ClientCommunicator] Ошибка отправки HELLO: "
             << udpSocket->errorString().toStdString() << endl;
        return false;
    }

    cout << "[ClientCommunicator] HELLO отправлен, ожидание ответа..." << endl;

    // Таймаут 3 секунды
    timeoutTimer->start(3000);

    return true;
}

void ClientCommunicator::disconnectFromServer()
{
    cout << "\n[ClientCommunicator] disconnectFromServer()" << endl;

    if (isConnected) {
        QByteArray byeMsg = "BYE";
        cout << "[ClientCommunicator] Отправка: BYE" << endl;
        udpSocket->writeDatagram(byeMsg, serverAddress, serverPort);
        isConnected = false;
        emit disconnected();
    }
    timeoutTimer->stop();
}

bool ClientCommunicator::isConnectedToServer() const
{
    return isConnected;
}

void ClientCommunicator::requestPolinom()
{
    cout << "\n[ClientCommunicator] requestPolinom()" << endl;

    if (!isConnected) {
        cout << "[ClientCommunicator] Ошибка: нет подключения к серверу" << endl;
        emit errorOccurred("Нет подключения к серверу");
        return;
    }

    QByteArray request = "GET_POLINOM";
    cout << "[ClientCommunicator] Отправка: GET_POLINOM" << endl;

    qint64 sent = udpSocket->writeDatagram(request, serverAddress, serverPort);

    if (sent != -1) {
        cout << "[ClientCommunicator] Запрос полинома отправлен" << endl;
    } else {
        cout << "[ClientCommunicator] Ошибка отправки запроса: "
             << udpSocket->errorString().toStdString() << endl;
        emit errorOccurred(udpSocket->errorString());
    }
}

void ClientCommunicator::onTimeout()
{
    cout << "\n[ClientCommunicator] Таймаут подключения" << endl;

    if (!isConnected) {
        emit connectionTimeout();
    }
}

Polinom<double> ClientCommunicator::parsePolinomDouble(const QString& response)
{
    cout << "[ClientCommunicator] parsePolinomDouble()" << endl;

    int degree = 0;
    double leading = 0.0;
    Array<double> roots;

    QStringList parts = response.split(';');
    cout << "[ClientCommunicator]   Частей: " << parts.size() << endl;

    for (const QString& part : parts) {
        if (part.startsWith("DEGREE:")) {
            degree = part.mid(7).toInt();
            cout << "[ClientCommunicator]   Степень: " << degree << endl;
        }
        else if (part.startsWith("LEADING:")) {
            std::string str = part.mid(8).toStdString();
            std::istringstream iss(str);
            iss >> leading;
            cout << "[ClientCommunicator]   Старший коэффициент: " << leading << endl;
        }
        else if (part.startsWith("ROOTS:")) {
            QString rootsStr = part.mid(6);
            cout << "[ClientCommunicator]   Строка корней: " << rootsStr.toStdString() << endl;

            if (!rootsStr.isEmpty() && degree > 0) {
                QStringList rootList = rootsStr.split(',');
                cout << "[ClientCommunicator]   Количество корней в списке: " << rootList.size() << endl;

                if (rootList.size() == degree) {
                    for (const QString& rootStr : rootList) {
                        std::string str = rootStr.toStdString();
                        std::istringstream iss(str);
                        double r;
                        iss >> r;
                        roots.pushBack(r);
                        cout << "[ClientCommunicator]     Корень: " << r << endl;
                    }
                } else {
                    cout << "[ClientCommunicator]   Ошибка: количество корней (" << rootList.size()
                         << ") != степени (" << degree << ")" << endl;
                }
            }
        }
    }

    cout << "[ClientCommunicator]   Создание полинома с " << roots.getSize() << " корнями" << endl;
    return Polinom<double>(leading, roots);
}

Polinom<TComplex> ClientCommunicator::parsePolinomComplex(const QString& response)
{
    cout << "[ClientCommunicator] parsePolinomComplex()" << endl;

    int degree = 0;
    TComplex leading;
    Array<TComplex> roots;

    QStringList parts = response.split(';');
    cout << "[ClientCommunicator]   Частей: " << parts.size() << endl;

    for (const QString& part : parts) {
        if (part.startsWith("DEGREE:")) {
            degree = part.mid(7).toInt();
            cout << "[ClientCommunicator]   Степень: " << degree << endl;
        }
        else if (part.startsWith("LEADING:")) {
            std::string str = part.mid(8).toStdString();
            std::istringstream iss(str);
            iss >> leading;
            cout << "[ClientCommunicator]   Старший коэффициент: " << leading << endl;
        }
        else if (part.startsWith("ROOTS:")) {
            QString rootsStr = part.mid(6);
            cout << "[ClientCommunicator]   Строка корней: " << rootsStr.toStdString() << endl;

            if (!rootsStr.isEmpty() && degree > 0) {
                QStringList rootList = rootsStr.split(',');
                cout << "[ClientCommunicator]   Количество корней в списке: " << rootList.size() << endl;

                if (rootList.size() == degree) {
                    for (const QString& rootStr : rootList) {
                        std::string str = rootStr.toStdString();
                        std::istringstream iss(str);
                        TComplex r;
                        iss >> r;
                        roots.pushBack(r);
                        cout << "[ClientCommunicator]     Корень: " << r << endl;
                    }
                } else {
                    cout << "[ClientCommunicator]   Ошибка: количество корней (" << rootList.size()
                         << ") != степени (" << degree << ")" << endl;
                }
            }
        }
    }

    cout << "[ClientCommunicator]   Создание полинома с " << roots.getSize() << " корнями" << endl;
    return Polinom<TComplex>(leading, roots);
}

void ClientCommunicator::onReadyRead()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());

        QHostAddress senderAddress;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &senderAddress, &senderPort);

        // Нормализуем адрес отправителя
        if (senderAddress.protocol() == QAbstractSocket::IPv6Protocol) {
            QString addrStr = senderAddress.toString();
            if (addrStr.startsWith("::ffff:")) {
                QString ipv4Str = addrStr.mid(7);
                senderAddress = QHostAddress(ipv4Str);
            }
        }

        // Проверяем, что ответ от ожидаемого сервера
        if (senderAddress != serverAddress || senderPort != serverPort) {
            cout << "[ClientCommunicator] Получен ответ от другого сервера: "
                 << senderAddress.toString().toStdString() << ":" << senderPort << endl;
            continue;
        }

        QString response = QString::fromUtf8(datagram).trimmed();

        cout << "\n[ClientCommunicator] Получен ответ от сервера:" << endl;
        cout << "[ClientCommunicator]   " << response.left(100).toStdString();
        if (response.length() > 100) cout << "...";
        cout << endl;

        // Останавливаем таймаут
        timeoutTimer->stop();

        if (response == "HELLO_ACK") {
            cout << "[ClientCommunicator] ✓ Подключение к серверу установлено" << endl;
            isConnected = true;
            emit connected();
        }
        else if (response.startsWith("ERROR:")) {
            QString errorMsg = response.mid(6);
            cout << "[ClientCommunicator] ✗ Ошибка сервера: " << errorMsg.toStdString() << endl;
            emit errorOccurred(errorMsg);
        }
        else if (response.startsWith("DEGREE:")) {
            try {
                if (currentType == 1) {
                    Polinom<double> p = parsePolinomDouble(response);
                    cout << "[ClientCommunicator] ✓ Получен полином (double) степени " << p.getDegree() << endl;
                    emit polinomReceived(p);
                } else {
                    Polinom<TComplex> p = parsePolinomComplex(response);
                    cout << "[ClientCommunicator] ✓ Получен полином (complex) степени " << p.getDegree() << endl;
                    emit polinomReceived(p);
                }
            } catch (const std::exception& e) {
                string error = "Ошибка парсинга: " + string(e.what());
                cout << "[ClientCommunicator] ✗ " << error << endl;
                emit errorOccurred(QString::fromStdString(error));
            }
        }
    }
}
