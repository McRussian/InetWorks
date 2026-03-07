#ifndef CLIENTAPPLICATION_H
#define CLIENTAPPLICATION_H

#include "windowapplication.h"
#include "clientcommunicator.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>

class ClientApplication : public WindowApplication
{
    Q_OBJECT

private:
    ClientCommunicator* communicator;

    // Дополнительные элементы интерфейса
    QLineEdit* serverIpEdit;
    QLineEdit* serverPortEdit;
    QPushButton* connectButton;
    QPushButton* syncButton;
    QLabel* connectionStatusLabel;

    void setupNetworkUI();

private slots:
    void onConnectButtonClicked();
    void onSyncButtonClicked();
    void onConnected();
    void onDisconnected();
    void onConnectionTimeout();
    void onCommunicatorError(const QString& error);
    void onPolinomReceived(const Polinom& receivedPolinom);

public:
    explicit ClientApplication(QWidget* parent = nullptr);
    ~ClientApplication();
};

#endif // CLIENTAPPLICATION_H
