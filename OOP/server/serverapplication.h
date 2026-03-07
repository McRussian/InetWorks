#ifndef SERVERAPPLICATION_H
#define SERVERAPPLICATION_H

#include <QObject>
#include <QThread>
#include "application.h"
#include "communicator.h"

// Класс для работы сервера в отдельном потоке
class ServerWorker : public QObject
{
    Q_OBJECT

private:
    Communicator* communicator;
    Polinom polinom;
    bool polinomExists;

public:
    explicit ServerWorker(QObject* parent = nullptr);
    ~ServerWorker();

public slots:
    void startServer(quint16 port);
    void stopServer();
    void updatePolinom(const Polinom& newPolinom, bool exists);

    // Слоты для обработки сигналов от коммуникатора
    void onPolinomRequested();
    void onClientConnected(const QHostAddress& address, quint16 port);
    void onClientDisconnected();

signals:
    void serverStarted(quint16 port);
    void serverError(const QString& error);
    void clientConnected(const QHostAddress& address, quint16 port);
    void clientDisconnected();
    void logMessage(const QString& message);
};

class ServerApplication : public ConsoleApplication
{
    Q_OBJECT

private:
    QThread* serverThread;
    ServerWorker* serverWorker;

private slots:
    void onServerStarted(quint16 port);
    void onServerError(const QString& error);
    void onClientConnected(const QHostAddress& address, quint16 port);
    void onClientDisconnected();
    void onLogMessage(const QString& message);

public:
    explicit ServerApplication(QObject* parent = nullptr);
    ~ServerApplication();

    bool start(quint16 port);
    void run() override;
};

#endif // SERVERAPPLICATION_H
