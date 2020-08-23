
#include<QGraphicsScene>
#include<QTimer>
#include<QList>
#include "bullet.h"
#include "qmath.h"
#include "enemy.h"
static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

static qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
        angle += TwoPi;
    while (angle > TwoPi)
        angle -= TwoPi;
    return angle;
}

Enemy::Enemy():QGraphicsPixmapItem()
{

    int rand_num=rand() % 700;
    setPos(rand_num,-100);
    QPixmap p(":/images/190276.png");
    setPixmap(p.scaled(100,100,Qt::IgnoreAspectRatio));
    timer=new QTimer();
    setTransformOriginPoint(50,50);
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));
    timer->start(speed);


}

Enemy::~Enemy()
{
   delete timer;
}



void Enemy::move()
{

    QList<QGraphicsItem *> colide=collidingItems();
        for(int i=0,n=colide.size();i<n;++i){
            if(typeid (*(colide[i]))==typeid (Bullet)){

                explosion=new Explosion(QPointF(this->pos()));
                emit addExplosion(explosion);
                emit increaseScore(static_cast<Bullet*>(colide[i])->getPlayerNumber(),1);
                scene()->removeItem(colide[i]);
                scene()->removeItem(this);
                delete colide[i];
                delete this;
                return;
                }
        }

    double y=this->y()+2;
    double x=this->x()+7*sin(this->y()*0.069);
    setPos(x,y);

    if(pos().y()>800){
        delete this;

    }
}

