#include "windowapplication.h"
#include <sstream>
#include <QMessageBox>

WindowApplication::WindowApplication(QWidget *parent)
    : QWidget(parent), realPolinom(nullptr), complexPolinom(nullptr),
    currentType(1), polinomExists(false)
{
    setupUI();
    updateDisplay();
}

WindowApplication::~WindowApplication()
{
    if (realPolinom) delete realPolinom;
    if (complexPolinom) delete complexPolinom;
}

void WindowApplication::setupUI()
{
    setWindowTitle("Просмотр полинома");
    setMinimumSize(600, 400);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // === Группа информации о типе ===
    QGroupBox* infoGroup = new QGroupBox("Информация");
    QHBoxLayout* infoLayout = new QHBoxLayout(infoGroup);

    infoLayout->addWidget(new QLabel("Тип полинома:"));
    typeDisplayComboBox = new QComboBox();
    typeDisplayComboBox->addItem("Вещественные числа (double)");
    typeDisplayComboBox->addItem("Комплексные числа (TComplex)");
    typeDisplayComboBox->setEnabled(false);  // Только для отображения
    infoLayout->addWidget(typeDisplayComboBox);
    infoLayout->addStretch();

    mainLayout->addWidget(infoGroup);

    // === Область отображения полинома ===
    QGroupBox* displayGroup = new QGroupBox("Полином");
    QVBoxLayout* displayLayout = new QVBoxLayout(displayGroup);

    displayArea = new QTextEdit();
    displayArea->setReadOnly(true);
    displayArea->setMinimumHeight(300);
    displayLayout->addWidget(displayArea);

    mainLayout->addWidget(displayGroup);
}

void WindowApplication::updateDisplay()
{
    if (!polinomExists) {
        displayArea->setText("Полином не задан.");
        return;
    }

    std::stringstream ss;

    if (currentType == 1 && realPolinom) {
        // Форма 1
        ss << "Форма 1 (an*x^n + ... + a0):\n";
        std::stringstream form1;
        realPolinom->printForm1(form1);
        ss << form1.str() << "\n\n";

        // Форма 2
        ss << "Форма 2 (an*(x - r1)*...*(x - rn)):\n";
        std::stringstream form2;
        realPolinom->printForm2(form2);
        ss << form2.str() << "\n\n";

        // Корни
        ss << "Корни полинома:\n";
        const Array<double>& roots = realPolinom->getRoots();
        for (size_t i = 0; i < roots.getSize(); ++i) {
            ss << "  r" << i << " = " << roots[i] << "\n";
        }
        ss << "\n";

        // Коэффициенты
        ss << "Коэффициенты (от старшей степени к младшей):\n";
        size_t degree = realPolinom->getDegree();
        for (size_t i = degree; i > 0; --i) {
            ss << "  a" << i << " = " << realPolinom->getCoefficient(i) << "\n";
        }
        ss << "  a0 = " << realPolinom->getCoefficient(0) << "\n";

        typeDisplayComboBox->setCurrentIndex(0);

    } else if (currentType == 2 && complexPolinom) {
        // Форма 1
        ss << "Форма 1 (an*x^n + ... + a0):\n";
        std::stringstream form1;
        complexPolinom->printForm1(form1);
        ss << form1.str() << "\n\n";

        // Форма 2
        ss << "Форма 2 (an*(x - r1)*...*(x - rn)):\n";
        std::stringstream form2;
        complexPolinom->printForm2(form2);
        ss << form2.str() << "\n\n";

        // Корни
        ss << "Корни полинома:\n";
        const Array<TComplex>& roots = complexPolinom->getRoots();
        for (size_t i = 0; i < roots.getSize(); ++i) {
            ss << "  r" << i << " = " << roots[i] << "\n";
        }
        ss << "\n";

        // Коэффициенты
        ss << "Коэффициенты (от старшей степени к младшей):\n";
        size_t degree = complexPolinom->getDegree();
        for (size_t i = degree; i > 0; --i) {
            ss << "  a" << i << " = " << complexPolinom->getCoefficient(i) << "\n";
        }
        ss << "  a0 = " << complexPolinom->getCoefficient(0) << "\n";

        typeDisplayComboBox->setCurrentIndex(1);
    }

    displayArea->setText(QString::fromStdString(ss.str()));
}

void WindowApplication::setRealPolinom(const Polinom<double>& polinom)
{
    if (realPolinom) delete realPolinom;
    realPolinom = new Polinom<double>(polinom);

    if (complexPolinom) {
        delete complexPolinom;
        complexPolinom = nullptr;
    }

    currentType = 1;
    polinomExists = true;
    updateDisplay();
}

void WindowApplication::setComplexPolinom(const Polinom<TComplex>& polinom)
{
    if (complexPolinom) delete complexPolinom;
    complexPolinom = new Polinom<TComplex>(polinom);

    if (realPolinom) {
        delete realPolinom;
        realPolinom = nullptr;
    }

    currentType = 2;
    polinomExists = true;
    updateDisplay();
}

void WindowApplication::setCurrentType(int type)
{
    currentType = type;
    if (polinomExists) {
        updateDisplay();
    }
}

void WindowApplication::clearPolinom()
{
    if (realPolinom) {
        delete realPolinom;
        realPolinom = nullptr;
    }
    if (complexPolinom) {
        delete complexPolinom;
        complexPolinom = nullptr;
    }
    polinomExists = false;
    displayArea->setText("Полином не задан.");
}
