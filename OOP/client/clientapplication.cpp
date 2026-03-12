#include "clientapplication.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QHostAddress>
#include <QIntValidator>

ClientApplication::ClientApplication(QWidget *parent)
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
    connect(communicator, &ClientCommunicator::connectionTimeout,
            this, &ClientApplication::onConnectionTimeout);

    // Подключаем сигналы для разных типов полиномов
    connect(communicator, QOverload<const Polinom<double>&>::of(&ClientCommunicator::polinomReceived),
            this, QOverload<const Polinom<double>&>::of(&ClientApplication::onPolinomReceived));
    connect(communicator, QOverload<const Polinom<TComplex>&>::of(&ClientCommunicator::polinomReceived),
            this, QOverload<const Polinom<TComplex>&>::of(&ClientApplication::onPolinomReceived));

    // Добавляем сетевой интерфейс
    setupNetworkUI();

    // Отключаем кнопки, требующие подключения
    disconnectButton->setEnabled(false);
    requestButton->setEnabled(false);
}

ClientApplication::~ClientApplication()
{
}

void ClientApplication::setupNetworkUI()
{
    // Создаем группу для сетевых настроек
    QGroupBox* networkGroup = new QGroupBox("Подключение к серверу");
    QGridLayout* networkLayout = new QGridLayout(networkGroup);

    int row = 0;

    // IP адрес
    networkLayout->addWidget(new QLabel("Сервер:"), row, 0);
    serverIpEdit = new QLineEdit("127.0.0.1");
    networkLayout->addWidget(serverIpEdit, row, 1);
    row++;

    // Порт
    networkLayout->addWidget(new QLabel("Порт:"), row, 0);
    serverPortEdit = new QLineEdit("12345");
    serverPortEdit->setValidator(new QIntValidator(1, 65535, this));
    networkLayout->addWidget(serverPortEdit, row, 1);
    row++;

    // Выбор типа
    networkLayout->addWidget(new QLabel("Тип данных:"), row, 0);
    typeComboBox = new QComboBox();
    typeComboBox->addItem("Вещественные (double)");
    typeComboBox->addItem("Комплексные (TComplex)");
    networkLayout->addWidget(typeComboBox, row, 1);
    row++;

    // Кнопки подключения
    QHBoxLayout* connButtonLayout = new QHBoxLayout();
    connectButton = new QPushButton("Подключиться");
    disconnectButton = new QPushButton("Отключиться");
    connButtonLayout->addWidget(connectButton);
    connButtonLayout->addWidget(disconnectButton);
    networkLayout->addLayout(connButtonLayout, row, 0, 1, 2);
    row++;

    // Кнопка запроса
    requestButton = new QPushButton("Запросить полином");
    networkLayout->addWidget(requestButton, row, 0, 1, 2);
    row++;

    // Статус подключения
    connectionStatusLabel = new QLabel("Статус: Не подключен");
    connectionStatusLabel->setStyleSheet("color: gray;");
    networkLayout->addWidget(connectionStatusLabel, row, 0, 1, 2);

    // Вставляем сетевую группу в начало основного layout'а
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (mainLayout) {
        // Вставляем перед группой информации
        QLayoutItem* infoGroup = mainLayout->takeAt(0);
        mainLayout->insertWidget(0, networkGroup);
        mainLayout->insertItem(1, infoGroup);
    }

    // Подключаем сигналы кнопок
    connect(connectButton, &QPushButton::clicked,
            this, &ClientApplication::onConnectButtonClicked);
    connect(disconnectButton, &QPushButton::clicked,
            this, &ClientApplication::onDisconnectButtonClicked);
    connect(requestButton, &QPushButton::clicked,
            this, &ClientApplication::onRequestButtonClicked);
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

    // Получаем выбранный тип
    int type = (typeComboBox->currentIndex() == 0) ? 1 : 2;

    connectButton->setEnabled(false);
    connectButton->setText("Подключение...");
    connectionStatusLabel->setText("Статус: Подключение...");
    connectionStatusLabel->setStyleSheet("color: orange;");

    communicator->connectToServer(address, port, type);
}

void ClientApplication::onDisconnectButtonClicked()
{
    communicator->disconnectFromServer();
}

void ClientApplication::onRequestButtonClicked()
{
    requestButton->setEnabled(false);
    requestButton->setText("Запрос...");
    communicator->requestPolinom();
}

void ClientApplication::onConnected()
{
    connectionStatusLabel->setText("Статус: Подключен к " +
                                   serverIpEdit->text() + ":" +
                                   serverPortEdit->text());
    connectionStatusLabel->setStyleSheet("color: green;");

    connectButton->setEnabled(false);
    connectButton->setText("Подключено");
    disconnectButton->setEnabled(true);
    requestButton->setEnabled(true);

    // Автоматически запрашиваем полином после подключения
    communicator->requestPolinom();
}

void ClientApplication::onDisconnected()
{
    connectionStatusLabel->setText("Статус: Отключен");
    connectionStatusLabel->setStyleSheet("color: gray;");

    connectButton->setEnabled(true);
    connectButton->setText("Подключиться");
    disconnectButton->setEnabled(false);
    requestButton->setEnabled(false);
    requestButton->setText("Запросить полином");

    clearPolinom();
}

void ClientApplication::onConnectionTimeout()
{
    connectionStatusLabel->setText("Статус: Таймаут подключения");
    connectionStatusLabel->setStyleSheet("color: red;");

    connectButton->setEnabled(true);
    connectButton->setText("Подключиться");

    QMessageBox::warning(this, "Ошибка",
                         "Не удалось подключиться к серверу. Проверьте адрес и порт.");
}

void ClientApplication::onCommunicatorError(const QString& error)
{
    QMessageBox::critical(this, "Ошибка", error);
}

void ClientApplication::onPolinomReceived(const Polinom<double>& polinom)
{
    setRealPolinom(polinom);
    requestButton->setEnabled(true);
    requestButton->setText("Запросить полином");

    QMessageBox::information(this, "Успех",
                             "Полином (double) успешно получен от сервера");
}

void ClientApplication::onPolinomReceived(const Polinom<TComplex>& polinom)
{
    setComplexPolinom(polinom);
    requestButton->setEnabled(true);
    requestButton->setText("Запросить полином");

    QMessageBox::information(this, "Успех",
                             "Полином (TComplex) успешно получен от сервера");
}
