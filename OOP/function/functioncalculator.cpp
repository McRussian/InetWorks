#include "functioncalculator.h"
#include <QMessageBox>
#include <sstream>
#include <iomanip>

FunctionCalculator::FunctionCalculator(QWidget *parent)
    : QWidget(parent), currentFunction(nullptr)
{
    // Создаем функции
    sinFunc = new SinFunction();
    siFunc = new SiFunction();

    setupUI();

    // Устанавливаем начальную функцию
    currentFunction = sinFunc;
    updateStatus();
}

FunctionCalculator::~FunctionCalculator()
{
    delete sinFunc;
    delete siFunc;
}

void FunctionCalculator::setupUI()
{
    setWindowTitle("Вычисление функций через ряд Маклорена");
    setMinimumSize(600, 500);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // === Группа выбора функции и параметров ===
    QGroupBox* inputGroup = new QGroupBox("Параметры вычисления");
    QGridLayout* inputLayout = new QGridLayout(inputGroup);

    // Выбор функции
    inputLayout->addWidget(new QLabel("Функция:"), 0, 0);
    functionComboBox = new QComboBox();
    functionComboBox->addItem("sin(x)");
    functionComboBox->addItem("Si(x) = sin(x)/x");
    inputLayout->addWidget(functionComboBox, 0, 1, 1, 2);

    // Степень полинома (точность)
    inputLayout->addWidget(new QLabel("Степень полинома:"), 1, 0);
    degreeSpinBox = new QSpinBox();
    degreeSpinBox->setRange(1, 20);
    degreeSpinBox->setValue(10);
    degreeSpinBox->setSuffix(" (точность)");
    inputLayout->addWidget(degreeSpinBox, 1, 1, 1, 2);

    // Точка x (комплексное число)
    inputLayout->addWidget(new QLabel("Точка x (Re, Im):"), 2, 0);

    QHBoxLayout* complexLayout = new QHBoxLayout();
    realPartEdit = new QLineEdit("0");
    realPartEdit->setPlaceholderText("Re");
    imagPartEdit = new QLineEdit("0");
    imagPartEdit->setPlaceholderText("Im");

    complexLayout->addWidget(new QLabel("Re:"));
    complexLayout->addWidget(realPartEdit);
    complexLayout->addWidget(new QLabel("Im:"));
    complexLayout->addWidget(imagPartEdit);

    inputLayout->addLayout(complexLayout, 2, 1, 1, 2);

    // Кнопки
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    calculateButton = new QPushButton("Вычислить");
    clearButton = new QPushButton("Очистить");
    buttonLayout->addWidget(calculateButton);
    buttonLayout->addWidget(clearButton);
    inputLayout->addLayout(buttonLayout, 3, 0, 1, 3);

    mainLayout->addWidget(inputGroup);

    // === Область отображения результатов ===
    QGroupBox* resultGroup = new QGroupBox("Результаты");
    QVBoxLayout* resultLayout = new QVBoxLayout(resultGroup);

    polynomLabel = new QLabel("Полином Маклорена:");
    polynomLabel->setWordWrap(true);
    resultLayout->addWidget(polynomLabel);

    resultDisplay = new QTextEdit();
    resultDisplay->setReadOnly(true);
    resultDisplay->setMinimumHeight(200);
    resultLayout->addWidget(resultDisplay);

    statusLabel = new QLabel("Готов к вычислениям");
    statusLabel->setStyleSheet("color: blue;");
    resultLayout->addWidget(statusLabel);

    mainLayout->addWidget(resultGroup);

    // Подключаем сигналы
    connect(functionComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FunctionCalculator::onFunctionChanged);
    connect(calculateButton, &QPushButton::clicked,
            this, &FunctionCalculator::onCalculateClicked);
    connect(clearButton, &QPushButton::clicked,
            this, &FunctionCalculator::onClearClicked);
}

void FunctionCalculator::updateStatus()
{
    if (currentFunction) {
        QString status = QString("Текущая функция: %1, макс. степень: %2")
                            .arg(currentFunction->getName())
                            .arg(degreeSpinBox->value());
        statusLabel->setText(status);
    }
}

void FunctionCalculator::onFunctionChanged(int index)
{
    switch (index) {
        case 0:
            currentFunction = sinFunc;
            break;
        case 1:
            currentFunction = siFunc;
            break;
        default:
            currentFunction = sinFunc;
    }

    updateStatus();
    resultDisplay->clear();
    polynomLabel->setText("Полином Маклорена:");
}

void FunctionCalculator::onClearClicked()
{
    realPartEdit->clear();
    imagPartEdit->clear();
    realPartEdit->setText("0");
    imagPartEdit->setText("0");
    resultDisplay->clear();
    polynomLabel->setText("Полином Маклорена:");
    updateStatus();
}

void FunctionCalculator::onCalculateClicked()
{
    if (!currentFunction) {
        QMessageBox::warning(this, "Ошибка", "Функция не выбрана");
        return;
    }

    // Читаем действительную часть
    bool okRe;
    double re = realPartEdit->text().toDouble(&okRe);

    // Читаем мнимую часть
    bool okIm;
    double im = imagPartEdit->text().toDouble(&okIm);

    if (!okRe || !okIm) {
        QMessageBox::warning(this, "Ошибка",
            "Неверный формат числа. Введите действительные числа для Re и Im.");
        return;
    }

    // Создаем комплексную точку
    TComplex x(re, im);
    int degree = degreeSpinBox->value();

    try {
        // Вычисляем значение функции
        TComplex result = currentFunction->evaluate(x, degree);

        // Формируем строку с результатами
        std::stringstream ss;
        ss << std::fixed << std::setprecision(10);

        ss << "Функция: " << currentFunction->getName().toStdString() << "\n";
        ss << "Степень полинома: " << degree << "\n";
        ss << "Точка x = " << x << "\n\n";
        ss << "Результат вычисления:\n";
        ss << "  P(x) = " << result << "\n\n";

        // Проверка с аналитическим значением (для вещественных чисел)
        if (std::abs(im) < 1e-10) {
            double x_real = re;
            double expected = 0.0;

            if (currentFunction == sinFunc) {
                expected = std::sin(x_real);
            } else if (currentFunction == siFunc) {
                if (std::abs(x_real) < 1e-10) {
                    expected = 1.0;
                } else {
                    expected = std::sin(x_real) / x_real;
                }
            }

            double error = std::abs(result.getRe() - expected);

            ss << "Аналитическое значение: " << expected << "\n";
            ss << "Погрешность: " << error << "\n";
        }

        // Показываем полином (упрощенно)
        std::stringstream poly;
        poly << "P(x) ≈ ";
        if (currentFunction == sinFunc) {
            poly << "x - x^3/6 + x^5/120 - x^7/5040 + ...";
        } else if (currentFunction == siFunc) {
            poly << "1 - x^2/6 + x^4/120 - x^6/5040 + ...";
        }

        polynomLabel->setText(QString::fromStdString(poly.str()));
        resultDisplay->setText(QString::fromStdString(ss.str()));

        statusLabel->setText("Вычисление выполнено успешно");
        statusLabel->setStyleSheet("color: green;");

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка вычисления",
                              QString("Ошибка: %1").arg(e.what()));
        statusLabel->setText("Ошибка вычисления");
        statusLabel->setStyleSheet("color: red;");
    }
}
