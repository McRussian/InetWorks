#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QUdpSocket>
#include <QObject>
#include <QHostAddress>
#include "polinom.hpp"
#include "complex.h"

class Communicator : public QObject
{
    Q_OBJECT

private:
    QUdpSocket* udpSocket;
    QHostAddress clientAddress;
    quint16 clientPort;
    bool hasClient;
    int clientType;  // 1 - double, 2 - TComplex

private slots:
    void onReadyRead();

public:
    explicit Communicator(QObject* parent = nullptr);
    ~Communicator();

    bool start(quint16 port);
    void stop();
    bool isClientConnected() const;
    void setDataType(int type);  // Установить тип данных

    // Перегруженные методы для отправки полиномов разных типов
    void sendPolinomToClient(const Polinom<double>& polinom, bool polinomExists);
    void sendPolinomToClient(const Polinom<TComplex>& polinom, bool polinomExists);

signals:
    void polinomRequested();
    void errorOccurred(const QString& error);
    void clientConnected(const QHostAddress& address, quint16 port);
    void clientDisconnected();
    void dataTypeReceived(int type);  // Сигнал о получении типа от клиента
};

#endif // COMMUNICATOR_H
