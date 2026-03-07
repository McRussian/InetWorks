#ifndef WINDOWAPPLICATION_H
#define WINDOWAPPLICATION_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "polinom.h"

class WindowApplication : public QWidget
{
    Q_OBJECT

protected:
    Polinom polinom;           // Текущий полином
    bool polinomExists;        // Флаг наличия полинома

    // Элементы интерфейса (protected для доступа из наследников)
    QLineEdit* inputEdit;
    QPushButton* inputButton;
    QTextEdit* displayArea;
    QLineEdit* pointEdit;
    QPushButton* calcButton;
    QLabel* resultLabel;

    virtual void setupUI();
    virtual void updateDisplay();

private slots:
    void onInputButtonClicked();
    void onCalcButtonClicked();

public:
    explicit WindowApplication(QWidget *parent = nullptr);
    virtual ~WindowApplication();
};

#endif // WINDOWAPPLICATION_H
