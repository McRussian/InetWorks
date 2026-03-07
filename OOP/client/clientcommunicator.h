#ifndef CLIENTCOMMUNICATOR_H
#define CLIENTCOMMUNICATOR_H

#include <QUdpSocket>
#include <QObject>
#include <QHostAddress>
#include "polinom.h"

class ClientCommunicator : public QObject
{
    Q_OBJECT

private:
    QUdpSocket* udpSocket;
    QHostAddress serverAddress;
    quint16 serverPort;
    bool isConnected;

private slots:
    void onReadyRead();

public:
    explicit ClientCommunicator(QObject* parent = nullptr);
    ~ClientCommunicator();

    bool connectToServer(const QHostAddress& address, quint16 port);
    void disconnectFromServer();
    bool isConnectedToServer() const;

    void requestPolinom();
    void sendHello();

signals:
    void connected();
    void disconnected();
    void errorOccurred(const QString& error);
    void polinomReceived(const Polinom& polinom);
    void connectionTimeout();
};

#endif // CLIENTCOMMUNICATOR_H
