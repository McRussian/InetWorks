#ifndef FUNCTIONCALCULATOR_H
#define FUNCTIONCALCULATOR_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include "function.hpp"

class FunctionCalculator : public QWidget
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
    QLineEdit* realPartEdit;
    QLineEdit* imagPartEdit;
    QPushButton* calculateButton;
    QPushButton* clearButton;
    QTextEdit* resultDisplay;
    QLabel* polynomLabel;
    QLabel* statusLabel;

    void setupUI();
    void updateStatus();

private slots:
    void onFunctionChanged(int index);
    void onCalculateClicked();
    void onClearClicked();

public:
    explicit FunctionCalculator(QWidget *parent = nullptr);
    ~FunctionCalculator();
};

#endif // FUNCTIONCALCULATOR_H
