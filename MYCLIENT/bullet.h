#ifndef BULLET_H
#define BULLET_H
#include<QObject>
#include<QGraphicsPixmapItem>
#include<QTimer>
#include<explosion.h>
class Bullet:public QObject, public QGraphicsPixmapItem
{
   Q_OBJECT

public:
    Bullet(int player_number,QPointF start,QPointF end, QGraphicsPixmapItem*parent=nullptr);
    QTimer* bullet_timer;
    int speed=12;
    int player_number;
    Explosion* explosion;
    int getPlayerNumber();
   ~Bullet();

public slots:
    void move();
};
#endif // BULLET_H
