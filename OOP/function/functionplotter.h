#ifndef FUNCTIONPLOTTER_H
#define FUNCTIONPLOTTER_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

// Правильное подключение Qt Charts
#include <QtCharts/QtCharts>

#include "function.hpp"

class FunctionPlotter : public QWidget
{
    Q_OBJECT

private:
    // Доступные функции
    SinFunction* sinFunc;
    SiFunction* siFunc;
    Function* currentFunction;

    // Элементы интерфейса
    QComboBox* functionComboBox;
    QSpinBox* degreeSpinBox;
    QLineEdit* xMinEdit;
    QLineEdit* xMaxEdit;
    QSpinBox* pointsSpinBox;
    QPushButton* plotButton;
    QPushButton* clearButton;

    // Область для графика
    QChartView* chartView;
    QChart* chart;
    QLineSeries* series;

    // Оси
    QValueAxis* axisX;
    QValueAxis* axisY;

    void setupUI();
    void setupChart();
    void clearChart();

private slots:
    void onFunctionChanged(int index);
    void onPlotButtonClicked();
    void onClearButtonClicked();

public:
    explicit FunctionPlotter(QWidget *parent = nullptr);
    ~FunctionPlotter();
};

#endif // FUNCTIONPLOTTER_H
