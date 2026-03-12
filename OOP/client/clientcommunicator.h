#ifndef CLIENTCOMMUNICATOR_H
#define CLIENTCOMMUNICATOR_H

#include <QUdpSocket>
#include <QObject>
#include <QHostAddress>
#include <QTimer>
#include "polinom.hpp"
#include "complex.h"

class ClientCommunicator : public QObject
{
    Q_OBJECT

private:
    QUdpSocket* udpSocket;
    QHostAddress serverAddress;
    quint16 serverPort;
    bool isConnected;
    int currentType;  // 1 - double, 2 - TComplex
    QTimer* timeoutTimer;

    Polinom<double> parsePolinomDouble(const QString& response);
    Polinom<TComplex> parsePolinomComplex(const QString& response);

private slots:
    void onReadyRead();
    void onTimeout();

public:
    explicit ClientCommunicator(QObject* parent = nullptr);
    ~ClientCommunicator();

    bool connectToServer(const QHostAddress& address, quint16 port, int type);
    void disconnectFromServer();
    bool isConnectedToServer() const;
    void requestPolinom();
    int getCurrentType() const { return currentType; }

signals:
    void connected();
    void disconnected();
    void errorOccurred(const QString& error);
    void polinomReceived(const Polinom<double>& polinom);
    void polinomReceived(const Polinom<TComplex>& polinom);
    void connectionTimeout();
};

#endif // CLIENTCOMMUNICATOR_H
