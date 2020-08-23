#include "mainwindow.h"

#include <QApplication>
#include<myclient.h>
#include<player.h>
#include<QGraphicsScene>
#include<QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // MyClient     client("5.137.120.40", 2323);
     MyClient     client("localhost", 2323);
 //MyClient     client("192.168.2.2", 2323);
/*Player *player1=new Player();
QGraphicsScene *scene=new QGraphicsScene();
scene->addItem(player1);

QGraphicsView* view=new QGraphicsView(scene);
view->show();*/
       client.show();
      client.ShowScene();
    return a.exec();
}
