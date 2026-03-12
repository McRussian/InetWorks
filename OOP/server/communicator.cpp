#include "communicator.h"
#include <iostream>
#include <sstream>

using namespace std;

Communicator::Communicator(QObject* parent)
    : QObject(parent), udpSocket(nullptr), clientPort(0), hasClient(false), clientType(1)
{
    cout << "[Communicator] Создан" << endl;
}

Communicator::~Communicator()
{
    cout << "[Communicator] Уничтожен" << endl;
    stop();
}

bool Communicator::start(quint16 port)
{
    cout << "[Communicator] Запуск на порту " << port << endl;

    udpSocket = new QUdpSocket(this);

    bool bound = udpSocket->bind(QHostAddress::AnyIPv4, port);

    if (!bound) {
        cout << "[Communicator] Ошибка: не удалось запустить UDP сервер на порту " << port << endl;
        cout << "[Communicator] Ошибка: " << udpSocket->errorString().toStdString() << endl;
        return false;
    }

    cout << "[Communicator] ✓ UDP коммуникатор запущен на порту: " << port << endl;
    cout << "[Communicator]   Адрес: " << udpSocket->localAddress().toString().toStdString() << endl;

    connect(udpSocket, &QUdpSocket::readyRead, this, &Communicator::onReadyRead);

    return true;
}

void Communicator::stop()
{
    cout << "[Communicator] Остановка" << endl;

    if (udpSocket) {
        udpSocket->close();
        udpSocket->deleteLater();
        udpSocket = nullptr;
    }
    hasClient = false;
    cout << "[Communicator] Остановлен" << endl;
}

bool Communicator::isClientConnected() const
{
    return hasClient;
}

void Communicator::setDataType(int type)
{
    clientType = type;
    cout << "[Communicator] Установлен тип данных: " << (type == 1 ? "double" : "TComplex") << endl;
}

void Communicator::onReadyRead()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());

        QHostAddress senderAddress;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &senderAddress, &senderPort);

        // Конвертируем в IPv4 если возможно
        if (senderAddress.protocol() == QAbstractSocket::IPv6Protocol) {
            QString addrStr = senderAddress.toString();
            if (addrStr.startsWith("::ffff:")) {
                QString ipv4Str = addrStr.mid(7);
                senderAddress = QHostAddress(ipv4Str);
            }
        }

        QString request = QString::fromUtf8(datagram).trimmed();

        cout << "\n[Communicator] Получен UDP запрос от "
             << senderAddress.toString().toStdString()
             << ":" << senderPort << " - "
             << request.toStdString() << endl;

        if (request == "HELLO") {
            // Клиент инициирует подключение
            clientAddress = senderAddress;
            clientPort = senderPort;
            hasClient = true;

            QString response = "HELLO_ACK";
            udpSocket->writeDatagram(response.toUtf8(), clientAddress, clientPort);

            cout << "[Communicator] ✓ Клиент зарегистрирован: "
                 << clientAddress.toString().toStdString()
                 << ":" << clientPort << endl;

            emit clientConnected(clientAddress, clientPort);
        }
        else if (request.startsWith("TYPE:")) {
            // Клиент сообщает свой тип данных
            QString typeStr = request.mid(5);
            if (typeStr == "DOUBLE") {
                clientType = 1;
                cout << "[Communicator]   → Клиент использует тип: double" << endl;
            } else if (typeStr == "COMPLEX") {
                clientType = 2;
                cout << "[Communicator]   → Клиент использует тип: TComplex" << endl;
            }
            emit dataTypeReceived(clientType);
        }
        else if (request == "GET_POLINOM") {
            if (senderAddress == clientAddress && senderPort == clientPort && hasClient) {
                cout << "[Communicator]   → Запрос полинома (тип: "
                     << (clientType == 1 ? "double" : "complex") << ")" << endl;

                cout << "[Communicator]   → Испускаем сигнал polinomRequested" << endl;
                emit polinomRequested();
                cout << "[Communicator]   → Сигнал испущен" << endl;
            } else {
                cout << "[Communicator]   → Отклонен запрос от неизвестного клиента" << endl;
            }
        }
        else if (request == "BYE") {
            if (senderAddress == clientAddress && senderPort == clientPort && hasClient) {
                hasClient = false;
                cout << "[Communicator] ✗ Клиент отключился" << endl;
                emit clientDisconnected();
            }
        }
    }
}

void Communicator::sendPolinomToClient(const Polinom<double>& polinom, bool polinomExists)
{
    cout << "\n[Communicator] sendPolinomToClient(double) вызван" << endl;

    if (!hasClient) {
        cout << "[Communicator] Нет подключенного клиента для отправки" << endl;
        return;
    }

    QString response;

    if (!polinomExists) {
        response = "ERROR:No polinom on server";
        cout << "[Communicator]   → Отправлен ответ: ERROR (полином не создан)" << endl;
    } else {
        // Формируем строковое представление полинома
        stringstream ss;
        ss << "DEGREE:" << polinom.getDegree() << ";";
        ss << "LEADING:" << polinom.getLeadingCoeff() << ";";

        // Корни
        const Array<double>& roots = polinom.getRoots();
        ss << "ROOTS:";
        cout << "[Communicator]   → Корни (" << roots.getSize() << "): ";
        for (size_t i = 0; i < roots.getSize(); ++i) {
            if (i > 0) {
                ss << ",";
                cout << ", ";
            }
            ss << roots[i];
            cout << roots[i];
        }
        cout << endl;

        response = QString::fromStdString(ss.str());
        cout << "[Communicator]   → Отправляем строку: " << response.toStdString() << endl;
    }

    qint64 sent = udpSocket->writeDatagram(response.toUtf8(), clientAddress, clientPort);
    cout << "[Communicator]   → Отправлено байт: " << sent << endl;
}

void Communicator::sendPolinomToClient(const Polinom<TComplex>& polinom, bool polinomExists)
{
    cout << "\n[Communicator] sendPolinomToClient(TComplex) вызван" << endl;

    if (!hasClient) {
        cout << "[Communicator] Нет подключенного клиента для отправки" << endl;
        return;
    }

    QString response;

    if (!polinomExists) {
        response = "ERROR:No polinom on server";
        cout << "[Communicator]   → Отправлен ответ: ERROR (полином не создан)" << endl;
    } else {
        // Формируем строковое представление полинома
        stringstream ss;
        ss << "DEGREE:" << polinom.getDegree() << ";";
        ss << "LEADING:" << polinom.getLeadingCoeff() << ";";

        // Корни
        const Array<TComplex>& roots = polinom.getRoots();
        ss << "ROOTS:";
        cout << "[Communicator]   → Корни (" << roots.getSize() << "): ";
        for (size_t i = 0; i < roots.getSize(); ++i) {
            if (i > 0) {
                ss << ",";
                cout << ", ";
            }
            ss << roots[i];
            cout << roots[i];
        }
        cout << endl;

        response = QString::fromStdString(ss.str());
        cout << "[Communicator]   → Отправляем строку: " << response.toStdString() << endl;
    }

    qint64 sent = udpSocket->writeDatagram(response.toUtf8(), clientAddress, clientPort);
    cout << "[Communicator]   → Отправлено байт: " << sent << endl;
}
