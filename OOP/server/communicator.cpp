#include "communicator.h"
#include <iostream>
#include <sstream>

using namespace std;

Communicator::Communicator(QObject* parent)
    : QObject(parent), udpSocket(nullptr), clientPort(0), hasClient(false)
{
}

Communicator::~Communicator()
{
    stop();
}

bool Communicator::start(quint16 port)
{
    udpSocket = new QUdpSocket(this);

    // Принудительно используем IPv4
    bool bound = udpSocket->bind(QHostAddress::AnyIPv4, port);

    if (!bound) {
        cout << "Ошибка: не удалось запустить UDP сервер на порту " << port << endl;
        cout << "Ошибка: " << udpSocket->errorString().toStdString() << endl;
        return false;
    }

    cout << "✓ UDP коммуникатор запущен на порту: " << port << endl;
    cout << "  Адрес: " << udpSocket->localAddress().toString().toStdString() << endl;

    connect(udpSocket, &QUdpSocket::readyRead, this, &Communicator::onReadyRead);

    return true;
}

void Communicator::stop()
{
    if (udpSocket) {
        udpSocket->close();
        udpSocket->deleteLater();
        udpSocket = nullptr;
    }
    hasClient = false;
}

bool Communicator::isClientConnected() const
{
    return hasClient;
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
            // Проверяем, является ли адрес IPv4-mapped IPv6 адресом (::ffff:xxx.xxx.xxx.xxx)
            QString addrStr = senderAddress.toString();
            if (addrStr.startsWith("::ffff:")) {
                QString ipv4Str = addrStr.mid(7); // Убираем "::ffff:"
                senderAddress = QHostAddress(ipv4Str);
            }
        }

        QString request = QString::fromUtf8(datagram).trimmed();

        cout << "Получен UDP запрос от "
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

            cout << "✓ Клиент зарегистрирован: "
                 << clientAddress.toString().toStdString()
                 << ":" << clientPort << endl;

            emit clientConnected(clientAddress, clientPort);
        }
        else if (request == "GET_POLINOM") {
            if (senderAddress == clientAddress && senderPort == clientPort && hasClient) {
                cout << "  → Запрос полинома" << endl;
                emit polinomRequested();
            } else {
                cout << "  → Отклонен запрос от неизвестного клиента" << endl;
            }
        }
        else if (request == "BYE") {
            if (senderAddress == clientAddress && senderPort == clientPort && hasClient) {
                hasClient = false;
                cout << "✗ Клиент отключился" << endl;
                emit clientDisconnected();
            }
        }
    }
}

void Communicator::sendPolinomToClient(const Polinom& polinom, bool polinomExists)
{
    if (!hasClient) {
        cout << "Нет подключенного клиента для отправки" << endl;
        return;
    }

    QString response;

    if (!polinomExists) {
        response = "ERROR:No polinom on server";
        cout << "  → Отправлен ответ: ERROR (полином не создан)" << endl;
    } else {
        // Формируем строковое представление полинома
        stringstream ss;
        ss << "DEGREE:" << polinom.getDegree() << ";";
        ss << "LEADING:" << polinom.getLeadingCoeff() << ";";

        // Корни
        const Array& roots = polinom.getRoots();
        ss << "ROOTS:";
        for (size_t i = 0; i < roots.getSize(); ++i) {
            if (i > 0) ss << ",";
            ss << roots[i];
        }
        ss << ";";

        // Коэффициенты
        ss << "COEFFS:";
        for (size_t i = 0; i <= polinom.getDegree(); ++i) {
            if (i > 0) ss << ",";
            ss << polinom.getCoefficient(i);
        }

        response = QString::fromStdString(ss.str());
        cout << "  → Отправлен полином (степень " << polinom.getDegree() << ")" << endl;
    }

    udpSocket->writeDatagram(response.toUtf8(), clientAddress, clientPort);
}
