#include <QApplication>
#include "functionplotter.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    FunctionPlotter plotter;
    plotter.show();

    return app.exec();
}
