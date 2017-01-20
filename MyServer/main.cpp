#include "myserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyServer server(1212);

    server.show();

    return a.exec();
}
