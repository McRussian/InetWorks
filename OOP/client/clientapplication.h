#ifndef CLIENTAPPLICATION_H
#define CLIENTAPPLICATION_H
#include <QPushButton>

#include "windowapplication.h"
#include "clientcommunicator.h"

class ClientApplication : public WindowApplication
{
    Q_OBJECT

private:
    ClientCommunicator* communicator;

    // Элементы интерфейса
    QLineEdit* serverIpEdit;
    QLineEdit* serverPortEdit;
    QComboBox* typeComboBox;      // Выбор типа для запроса
    QPushButton* connectButton;
    QPushButton* disconnectButton;
    QPushButton* requestButton;
    QLabel* connectionStatusLabel;

    void setupNetworkUI();

private slots:
    void onConnectButtonClicked();
    void onDisconnectButtonClicked();
    void onRequestButtonClicked();
    void onConnected();
    void onDisconnected();
    void onConnectionTimeout();
    void onCommunicatorError(const QString& error);
    void onPolinomReceived(const Polinom<double>& polinom);
    void onPolinomReceived(const Polinom<TComplex>& polinom);

public:
    explicit ClientApplication(QWidget *parent = nullptr);
    ~ClientApplication();
};

#endif // CLIENTAPPLICATION_H
