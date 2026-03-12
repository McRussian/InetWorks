#ifndef SERVERAPPLICATION_H
#define SERVERAPPLICATION_H

#include <QObject>
#include <QThread>
#include "application.h"
#include "communicator.h"

// Структуры для передачи данных между потоками
struct DoublePolinomData {
    double an;
    size_t degree;
    QVector<double> roots;
};

struct ComplexPolinomData {
    TComplex an;
    size_t degree;
    QVector<TComplex> roots;
};

class MenuWorker : public QObject
{
    Q_OBJECT

public slots:
    void run();

signals:
    void commandEntered(int cmd);
    void doublePolinomDataReady(const DoublePolinomData& data);
    void complexPolinomDataReady(const ComplexPolinomData& data);
    void finished();
};

class ServerApplication : public ConsoleApplication
{
    Q_OBJECT

private:
    Communicator* communicator;
    QThread* serverThread;
    QThread* menuThread;
    MenuWorker* menuWorker;

    void stopServer();

private slots:
    void onPolinomRequested();
    void onClientConnected(const QHostAddress& address, quint16 port);
    void onClientDisconnected();
    void onDataTypeReceived(int type);
    void onError(const QString& error);
    void onCommandEntered(int cmd);
    void onDoublePolinomDataReceived(const DoublePolinomData& data);
    void onComplexPolinomDataReceived(const ComplexPolinomData& data);

public:
    explicit ServerApplication(QObject* parent = nullptr);
    ~ServerApplication();

    bool startServer(quint16 port);
    void run() override;

    void handleInput() override;
    void handleChangeCoeffOrRoot() override;
    void handleEvaluate() override;
    void handleResize() override;
    void handleDisplayForm1() override;
    void handleDisplayForm2() override;
    void handlePrintRoots() override;
    void handlePrintCoeffs() override;
};

#endif // SERVERAPPLICATION_H
