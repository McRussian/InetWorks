#include "clientapplication.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QHostAddress>

ClientApplication::ClientApplication(QWidget* parent)
    : WindowApplication(parent), communicator(nullptr)
{
    communicator = new ClientCommunicator(this);

    // Подключаем сигналы коммуникатора
    connect(communicator, &ClientCommunicator::connected,
            this, &ClientApplication::onConnected);
    connect(communicator, &ClientCommunicator::disconnected,
            this, &ClientApplication::onDisconnected);
    connect(communicator, &ClientCommunicator::errorOccurred,
            this, &ClientApplication::onCommunicatorError);
    connect(communicator, &ClientCommunicator::polinomReceived,
            this, &ClientApplication::onPolinomReceived);
    connect(communicator, &ClientCommunicator::connectionTimeout,
            this, &ClientApplication::onConnectionTimeout);

    // Добавляем сетевой интерфейс
    setupNetworkUI();

    // Отключаем кнопки, требующие подключения
    syncButton->setEnabled(false);
}

ClientApplication::~ClientApplication()
{
}

void ClientApplication::setupNetworkUI()
{
    // Создаем группу для сетевых настроек
    QGroupBox* networkGroup = new QGroupBox("Подключение к серверу");
    QGridLayout* networkLayout = new QGridLayout(networkGroup);

    // Поля ввода
    networkLayout->addWidget(new QLabel("Сервер:"), 0, 0);
    serverIpEdit = new QLineEdit("127.0.0.1");
    networkLayout->addWidget(serverIpEdit, 0, 1);

    networkLayout->addWidget(new QLabel("Порт:"), 1, 0);
    serverPortEdit = new QLineEdit("12345");
    networkLayout->addWidget(serverPortEdit, 1, 1);

    // Кнопки
    connectButton = new QPushButton("Подключиться");
    networkLayout->addWidget(connectButton, 2, 0, 1, 2);

    syncButton = new QPushButton("Получить полином");
    networkLayout->addWidget(syncButton, 3, 0, 1, 2);

    connectionStatusLabel = new QLabel("Статус: Не подключен");
    connectionStatusLabel->setStyleSheet("color: gray;");
    networkLayout->addWidget(connectionStatusLabel, 4, 0, 1, 2);

    // Вставляем сетевую группу в начало основного layout'а
    // Предполагаем, что в WindowApplication есть mainLayout
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (mainLayout) {
        mainLayout->insertWidget(0, networkGroup);
    }

    // Подключаем сигналы кнопок
    connect(connectButton, &QPushButton::clicked,
            this, &ClientApplication::onConnectButtonClicked);
    connect(syncButton, &QPushButton::clicked,
            this, &ClientApplication::onSyncButtonClicked);
}

void ClientApplication::onConnectButtonClicked()
{
    QString ipStr = serverIpEdit->text().trimmed();
    quint16 port = serverPortEdit->text().toUShort();

    if (ipStr.isEmpty() || port == 0) {
        QMessageBox::warning(this, "Ошибка", "Введите корректный IP и порт");
        return;
    }

    QHostAddress address(ipStr);
    if (address.isNull()) {
        QMessageBox::warning(this, "Ошибка", "Неверный IP адрес");
        return;
    }

    connectButton->setEnabled(false);
    connectButton->setText("Подключение...");
    connectionStatusLabel->setText("Статус: Подключение...");
    connectionStatusLabel->setStyleSheet("color: orange;");

    communicator->connectToServer(address, port);
}

void ClientApplication::onConnected()
{
    connectionStatusLabel->setText("Статус: Подключен к " +
                                   serverIpEdit->text() + ":" +
                                   serverPortEdit->text());
    connectionStatusLabel->setStyleSheet("color: green;");
    connectButton->setText("Подключено");
    connectButton->setEnabled(false);
    syncButton->setEnabled(true);
}

void ClientApplication::onDisconnected()
{
    connectionStatusLabel->setText("Статус: Отключен");
    connectionStatusLabel->setStyleSheet("color: gray;");
    connectButton->setText("Подключиться");
    connectButton->setEnabled(true);
    syncButton->setEnabled(false);
    polinomExists = false;
    updateDisplay();
}

void ClientApplication::onConnectionTimeout()
{
    connectionStatusLabel->setText("Статус: Таймаут подключения");
    connectionStatusLabel->setStyleSheet("color: red;");
    connectButton->setText("Подключиться");
    connectButton->setEnabled(true);

    QMessageBox::warning(this, "Ошибка",
                         "Не удалось подключиться к серверу. Проверьте адрес и порт.");
}

void ClientApplication::onCommunicatorError(const QString& error)
{
    QMessageBox::critical(this, "Ошибка", error);
}

void ClientApplication::onSyncButtonClicked()
{
    syncButton->setEnabled(false);
    syncButton->setText("Запрос...");
    communicator->requestPolinom();
}

void ClientApplication::onPolinomReceived(const Polinom& receivedPolinom)
{
    polinom = receivedPolinom;
    polinomExists = true;

    syncButton->setEnabled(true);
    syncButton->setText("Получить полином");

    updateDisplay();

    QMessageBox::information(this, "Успех",
                             "Полином успешно получен от сервера");
}
