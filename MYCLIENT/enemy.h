#ifndef ENEMY_H
#define ENEMY_H
#include<QObject>
#include<QGraphicsPixmapItem>

#include<bullet.h>
#include<explosion.h>
class Enemy:public QObject, public QGraphicsPixmapItem
{ friend class Bullet;
    Q_OBJECT

public:
     Enemy();
     int speed=25;
     QTimer* timer;
     Explosion* explosion;
     QPointF target;
    ~Enemy();

public slots:
     void move();

signals:
void addExplosion(Explosion* explosion);
void increaseScore(int player_number,int value);
};


#endif // ENEMY_H
