#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QUdpSocket>
#include <QObject>
#include <QHostAddress>
#include "polinom.h"

class Communicator : public QObject
{
    Q_OBJECT

private:
    QUdpSocket* udpSocket;
    QHostAddress clientAddress;
    quint16 clientPort;
    bool hasClient;

private slots:
    void onReadyRead();

public:
    explicit Communicator(QObject* parent = nullptr);
    ~Communicator();

    bool start(quint16 port);
    void stop();
    bool isClientConnected() const;
    void sendPolinomToClient(const Polinom& polinom, bool polinomExists);

signals:
    void polinomRequested();
    void errorOccurred(const QString& error);
    void clientConnected(const QHostAddress& address, quint16 port);
    void clientDisconnected();
};

#endif // COMMUNICATOR_H
