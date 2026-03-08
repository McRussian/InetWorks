#include "functionplotter.h"
#include <QMessageBox>
#include <QDoubleValidator>
#include <limits>

// Если используете Qt6, добавьте:
// QT_CHARTS_USE_NAMESPACE

FunctionPlotter::FunctionPlotter(QWidget *parent)
    : QWidget(parent), currentFunction(nullptr)
{
    // Создаем функции
    sinFunc = new SinFunction();
    siFunc = new SiFunction();

    setupUI();
    setupChart();

    // Устанавливаем начальную функцию
    currentFunction = sinFunc;
}

FunctionPlotter::~FunctionPlotter()
{
    delete sinFunc;
    delete siFunc;
    delete chart;
    delete series;
    delete axisX;
    delete axisY;
}

void FunctionPlotter::setupUI()
{
    setWindowTitle("Построитель графиков функций");
    setMinimumSize(800, 600);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // === Панель управления ===
    QGroupBox* controlGroup = new QGroupBox("Параметры");
    QGridLayout* controlLayout = new QGridLayout(controlGroup);

    // Выбор функции
    controlLayout->addWidget(new QLabel("Функция:"), 0, 0);
    functionComboBox = new QComboBox();
    functionComboBox->addItem("sin(x)");
    functionComboBox->addItem("Si(x) = sin(x)/x");
    controlLayout->addWidget(functionComboBox, 0, 1);

    // Степень полинома (точность)
    controlLayout->addWidget(new QLabel("Степень полинома:"), 1, 0);
    degreeSpinBox = new QSpinBox();
    degreeSpinBox->setRange(1, 20);
    degreeSpinBox->setValue(10);
    controlLayout->addWidget(degreeSpinBox, 1, 1);

    // Количество точек
    controlLayout->addWidget(new QLabel("Количество точек:"), 2, 0);
    pointsSpinBox = new QSpinBox();
    pointsSpinBox->setRange(50, 1000);
    pointsSpinBox->setValue(200);
    controlLayout->addWidget(pointsSpinBox, 2, 1);

    // Диапазон по X
    controlLayout->addWidget(new QLabel("X от:"), 3, 0);
    xMinEdit = new QLineEdit("-5");
    xMinEdit->setValidator(new QDoubleValidator(this));
    controlLayout->addWidget(xMinEdit, 3, 1);

    controlLayout->addWidget(new QLabel("X до:"), 4, 0);
    xMaxEdit = new QLineEdit("5");
    xMaxEdit->setValidator(new QDoubleValidator(this));
    controlLayout->addWidget(xMaxEdit, 4, 1);

    // Кнопки
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    plotButton = new QPushButton("Построить график");
    clearButton = new QPushButton("Очистить");
    buttonLayout->addWidget(plotButton);
    buttonLayout->addWidget(clearButton);
    controlLayout->addLayout(buttonLayout, 5, 0, 1, 2);

    mainLayout->addWidget(controlGroup);

    // === Область графика ===
    chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(400);
    mainLayout->addWidget(chartView);

    // Подключаем сигналы
    connect(functionComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FunctionPlotter::onFunctionChanged);
    connect(plotButton, &QPushButton::clicked,
            this, &FunctionPlotter::onPlotButtonClicked);
    connect(clearButton, &QPushButton::clicked,
            this, &FunctionPlotter::onClearButtonClicked);
}

void FunctionPlotter::setupChart()
{
    // Создаем график
    chart = new QChart();
    chart->setTitle("График функции");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Создаем серию данных
    series = new QLineSeries();
    series->setName("Функция");

    // Создаем оси
    axisX = new QValueAxis();
    axisX->setTitleText("x");
    axisX->setLabelFormat("%.1f");
    axisX->setGridLineVisible(true);

    axisY = new QValueAxis();
    axisY->setTitleText("f(x)");
    axisY->setLabelFormat("%.2f");
    axisY->setGridLineVisible(true);

    chart->addSeries(series);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    // Прикрепляем серию к осям
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    chartView->setChart(chart);
}

void FunctionPlotter::clearChart()
{
    series->clear();
    chart->setTitle("График функции");
}

void FunctionPlotter::onFunctionChanged(int index)
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
}

void FunctionPlotter::onClearButtonClicked()
{
    clearChart();
}

void FunctionPlotter::onPlotButtonClicked()
{
    if (!currentFunction) {
        QMessageBox::warning(this, "Ошибка", "Функция не выбрана");
        return;
    }

    // Читаем границы интервала
    bool okMin, okMax;
    double xMin = xMinEdit->text().toDouble(&okMin);
    double xMax = xMaxEdit->text().toDouble(&okMax);

    if (!okMin || !okMax) {
        QMessageBox::warning(this, "Ошибка",
                             "Неверный формат границ интервала. Введите числа.");
        return;
    }

    if (xMin >= xMax) {
        QMessageBox::warning(this, "Ошибка",
                             "Нижняя граница должна быть меньше верхней.");
        return;
    }

    int degree = degreeSpinBox->value();
    int points = pointsSpinBox->value();
    currentFunction->setMaxDegree(degree);

    try {
        // Очищаем предыдущий график
        series->clear();

        double step = (xMax - xMin) / (points - 1);

        // Находим min и max y для настройки осей
        double yMin = std::numeric_limits<double>::max();
        double yMax = std::numeric_limits<double>::lowest();

        // Сначала вычисляем все точки для определения диапазона y
        QVector<QPointF> dataPoints;
        for (int i = 0; i < points; ++i) {
            double x = xMin + i * step;

            // Для графика нам нужна только действительная часть
            // (аргумент x - действительное число, функция возвращает комплексное)
            TComplex result = currentFunction->evaluate(TComplex(x, 0), degree);
            double y = result.getRe();  // Берем действительную часть

            if (std::isfinite(y)) {
                dataPoints.append(QPointF(x, y));
                yMin = std::min(yMin, y);
                yMax = std::max(yMax, y);
            }
        }

        // Добавляем небольшой отступ по y
        double yPadding = (yMax - yMin) * 0.1;
        if (yPadding < 0.1) yPadding = 0.1;

        // Настраиваем оси
        axisX->setRange(xMin, xMax);
        axisY->setRange(yMin - yPadding, yMax + yPadding);

        // Добавляем точки в серию
        series->append(dataPoints);

        // Обновляем заголовок
        chart->setTitle(QString("%1 (степень полинома: %2)")
                            .arg(currentFunction->getName())
                            .arg(degree));

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка вычисления",
                              QString("Ошибка: %1").arg(e.what()));
    }
}
