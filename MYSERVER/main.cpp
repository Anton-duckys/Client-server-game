#include "mainwindow.h"


#include <QApplication>
#include<myserver.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyServer     server(2323);

        server.show();
    return a.exec();
}
