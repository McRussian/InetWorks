#include "windowapplication.h"
#include <sstream>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

WindowApplication::WindowApplication(QWidget *parent)
    : QWidget(parent), polinomExists(false)
{
    setupUI();
    updateDisplay();
    calcButton->setEnabled(false);
}

WindowApplication::~WindowApplication()
{
}

void WindowApplication::setupUI()
{
    setWindowTitle("Работа с полиномом");
    setMinimumSize(600, 500);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // === Группа ввода полинома ===
    QGroupBox* inputGroup = new QGroupBox("Ввод полинома");
    QVBoxLayout* inputLayout = new QVBoxLayout(inputGroup);

    QLabel* inputLabel = new QLabel("Формат: an n r1 r2 ... rn");
    QLabel* exampleLabel = new QLabel("Пример: 2 2 1 3");
    exampleLabel->setStyleSheet("color: gray; font-size: 10pt;");

    QHBoxLayout* inputLineLayout = new QHBoxLayout();
    inputEdit = new QLineEdit();
    inputEdit->setPlaceholderText("Введите полином...");
    inputButton = new QPushButton("Ввести полином");
    inputLineLayout->addWidget(inputEdit);
    inputLineLayout->addWidget(inputButton);

    inputLayout->addWidget(inputLabel);
    inputLayout->addWidget(exampleLabel);
    inputLayout->addLayout(inputLineLayout);

    mainLayout->addWidget(inputGroup);

    // === Область отображения информации ===
    QGroupBox* displayGroup = new QGroupBox("Информация о полиноме");
    QVBoxLayout* displayLayout = new QVBoxLayout(displayGroup);

    displayArea = new QTextEdit();
    displayArea->setReadOnly(true);
    displayArea->setMinimumHeight(200);
    displayLayout->addWidget(displayArea);

    mainLayout->addWidget(displayGroup);

    // === Группа вычисления значения ===
    QGroupBox* calcGroup = new QGroupBox("Вычисление значения полинома");
    QVBoxLayout* calcLayout = new QVBoxLayout(calcGroup);

    QHBoxLayout* calcLineLayout = new QHBoxLayout();
    pointEdit = new QLineEdit();
    pointEdit->setPlaceholderText("Введите точку x...");
    calcButton = new QPushButton("Вычислить");
    calcLineLayout->addWidget(pointEdit);
    calcLineLayout->addWidget(calcButton);

    resultLabel = new QLabel("Результат: ");
    resultLabel->setStyleSheet("font-weight: bold; color: blue;");

    calcLayout->addLayout(calcLineLayout);
    calcLayout->addWidget(resultLabel);

    mainLayout->addWidget(calcGroup);

    // Подключаем сигналы
    connect(inputButton, &QPushButton::clicked,
            this, &WindowApplication::onInputButtonClicked);
    connect(calcButton, &QPushButton::clicked,
            this, &WindowApplication::onCalcButtonClicked);
}

void WindowApplication::updateDisplay()
{
    if (!polinomExists) {
        displayArea->setText("Полином не задан. Введите полином в поле выше.");
        return;
    }

    std::stringstream ss;

    // Форма 1
    ss << "Форма 1 (an*x^n + ... + a0):\n";
    std::stringstream form1;
    polinom.printForm1(form1);
    ss << form1.str() << "\n\n";

    // Форма 2
    ss << "Форма 2 (an*(x - r1)*...*(x - rn)):\n";
    std::stringstream form2;
    polinom.printForm2(form2);
    ss << form2.str() << "\n\n";

    // Корни
    ss << "Корни полинома:\n";
    const Array& roots = polinom.getRoots();
    for (size_t i = 0; i < roots.getSize(); ++i) {
        ss << "  r" << i << " = " << roots[i] << "\n";
    }
    ss << "\n";

    // Коэффициенты
    ss << "Коэффициенты (от старшей степени к младшей):\n";
    size_t degree = polinom.getDegree();
    for (size_t i = degree; i > 0; --i) {
        ss << "  a" << i << " = " << polinom.getCoefficient(i) << "\n";
    }
    ss << "  a0 = " << polinom.getCoefficient(0) << "\n";

    displayArea->setText(QString::fromStdString(ss.str()));
    calcButton->setEnabled(true);
}

void WindowApplication::onInputButtonClicked()
{
    QString input = inputEdit->text().trimmed();

    if (input.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите данные полинома");
        return;
    }

    std::string inputStr = input.toStdString();
    std::istringstream iss(inputStr);

    try {
        number an;
        size_t degree;

        iss >> an >> degree;

        Array roots(degree);
        for (size_t i = 0; i < degree; ++i) {
            number root;
            iss >> root;
            roots[i] = root;
        }

        if (iss.fail()) {
            QMessageBox::warning(this, "Ошибка ввода",
                                 "Неверный формат ввода. Используйте: an n r1 r2 ... rn");
            polinomExists = false;
        } else {
            polinom = Polinom(an, roots);
            polinomExists = true;
            updateDisplay();
            inputEdit->clear();
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка",
                              QString("Ошибка при создании полинома: %1").arg(e.what()));
        polinomExists = false;
    }
}

void WindowApplication::onCalcButtonClicked()
{
    if (!polinomExists) {
        QMessageBox::warning(this, "Ошибка", "Сначала введите полином");
        return;
    }

    QString pointStr = pointEdit->text().trimmed();

    if (pointStr.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите точку x");
        return;
    }

    std::string pointStrStd = pointStr.toStdString();
    std::istringstream iss(pointStrStd);

    number x;
    iss >> x;

    if (iss.fail()) {
        QMessageBox::warning(this, "Ошибка ввода",
                             "Неверный формат числа");
        return;
    }

    try {
        number result = polinom.evaluate(x);
        std::stringstream ss;
        ss << "Результат: P(" << x << ") = " << result;
        resultLabel->setText(QString::fromStdString(ss.str()));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка",
                              QString("Ошибка при вычислении: %1").arg(e.what()));
    }
}
