#include <QApplication>
#include "clientapplication.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ClientApplication client;
    client.show();

    return app.exec();
}
