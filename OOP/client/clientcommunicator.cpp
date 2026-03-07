#include "clientcommunicator.h"
#include <iostream>
#include <sstream>
#include <QTimer>
#include <QDebug>

using namespace std;

ClientCommunicator::ClientCommunicator(QObject* parent)
    : QObject(parent), udpSocket(nullptr), serverPort(0), isConnected(false)
{
    udpSocket = new QUdpSocket(this);
    connect(udpSocket, &QUdpSocket::readyRead, this, &ClientCommunicator::onReadyRead);
}

ClientCommunicator::~ClientCommunicator()
{
    disconnectFromServer();
}

bool ClientCommunicator::connectToServer(const QHostAddress& address, quint16 port)
{
    // Убеждаемся, что используем IPv4 адрес
    QHostAddress ipv4Address;
    if (address.protocol() == QAbstractSocket::IPv6Protocol) {
        QString addrStr = address.toString();
        if (addrStr.startsWith("::ffff:")) {
            QString ipv4Str = addrStr.mid(7);
            ipv4Address = QHostAddress(ipv4Str);
        } else {
            ipv4Address = address;
        }
    } else {
        ipv4Address = address;
    }

    serverAddress = ipv4Address;
    serverPort = port;

    cout << "Подключение к серверу "
         << serverAddress.toString().toStdString() << ":" << port << endl;

    QByteArray helloMsg = "HELLO";
    qint64 sent = udpSocket->writeDatagram(helloMsg, serverAddress, serverPort);

    if (sent == -1) {
        cout << "Ошибка отправки HELLO: "
             << udpSocket->errorString().toStdString() << endl;
        return false;
    }

    cout << "HELLO отправлен, ожидание ответа..." << endl;

    QTimer::singleShot(3000, this, [this]() {
        if (!isConnected) {
            cout << "Таймаут подключения" << endl;
            emit connectionTimeout();
        }
    });

    return true;
}

void ClientCommunicator::disconnectFromServer()
{
    if (isConnected) {
        QByteArray byeMsg = "BYE";
        udpSocket->writeDatagram(byeMsg, serverAddress, serverPort);
        isConnected = false;
        emit disconnected();
    }
}

bool ClientCommunicator::isConnectedToServer() const
{
    return isConnected;
}

void ClientCommunicator::requestPolinom()
{
    if (!isConnected) return;

    QByteArray request = "GET_POLINOM";
    udpSocket->writeDatagram(request, serverAddress, serverPort);
    cout << "Запрос полинома отправлен" << endl;
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

        // Конвертируем в IPv4
        if (senderAddress.protocol() == QAbstractSocket::IPv6Protocol) {
            QString addrStr = senderAddress.toString();
            if (addrStr.startsWith("::ffff:")) {
                QString ipv4Str = addrStr.mid(7);
                senderAddress = QHostAddress(ipv4Str);
            }
        }

        // Проверяем, что ответ от нашего сервера
        if (senderAddress != serverAddress || senderPort != serverPort) {
            continue;
        }

        QString response = QString::fromUtf8(datagram).trimmed();
        cout << "Получен ответ: " << response.toStdString() << endl;

        if (response == "HELLO_ACK") {
            cout << "✓ Подключение установлено" << endl;
            isConnected = true;
            emit connected();
        }
        else if (response.startsWith("ERROR:")) {
            emit errorOccurred(response.mid(6));
        }
        else if (response.startsWith("DEGREE:")) {
            try {
                // УПРОЩЕННЫЙ ПАРСИНГ
                QStringList parts = response.split(';');

                // 1. Парсим степень
                int degree = 0;
                for (const QString& part : parts) {
                    if (part.startsWith("DEGREE:")) {
                        degree = part.mid(7).toInt();
                        cout << "Степень: " << degree << endl;
                        break;
                    }
                }

                // 2. Парсим старший коэффициент
                number leading;
                for (const QString& part : parts) {
                    if (part.startsWith("LEADING:")) {
                        string str = part.mid(8).toStdString();
                        istringstream iss(str);
                        iss >> leading;
                        cout << "Старший коэффициент: " << leading << endl;
                        break;
                    }
                }

                // 3. Парсим корни
                Array roots;
                for (const QString& part : parts) {
                    if (part.startsWith("ROOTS:")) {
                        QString rootsStr = part.mid(6);
                        cout << "Строка корней: " << rootsStr.toStdString() << endl;

                        if (!rootsStr.isEmpty() && degree > 0) {
                            QStringList rootList = rootsStr.split(',');
                            cout << "Количество корней: " << rootList.size() << endl;

                            if (rootList.size() == degree) {
                                roots = Array(degree);
                                for (int i = 0; i < rootList.size(); ++i) {
                                    string str = rootList[i].toStdString();
                                    istringstream iss(str);
                                    number r;
                                    iss >> r;
                                    roots.pushBack(r);
                                    cout << "Корень " << i << ": " << r << endl;
                                }
                            }
                        }
                        break;
                    }
                }

                // 4. Создаем полином
                cout << "Создание полинома... Roots size: " << roots.getSize() << endl;
                Polinom polinom(leading, roots);
                cout << "✓ Полином создан" << endl;
                emit polinomReceived(polinom);

            } catch (const std::exception& e) {
                cout << "✗ Ошибка: " << e.what() << endl;
                emit errorOccurred(e.what());
            }
        }
    }
}
