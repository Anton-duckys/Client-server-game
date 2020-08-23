#include "bullet.h"
#include <QList>
#include <math.h>
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

Bullet::Bullet(int player_number,QPointF start,QPointF end,QGraphicsPixmapItem *parent):QGraphicsPixmapItem(parent)
{
     this->player_number=player_number;
     QPixmap p(":/images/bullet.png");
     setPixmap(p.scaled(40,40));
     this->setPos(start);
     QLineF lineToTarget(start, end);
     // Угол поворота в направлении к цели
     qreal angleToTarget = ::acos(lineToTarget.dx() / lineToTarget.length());

     if (lineToTarget.dy() < 0)
         angleToTarget = TwoPi - angleToTarget;
     angleToTarget = normalizeAngle((Pi - angleToTarget) + Pi / 2);

     if (angleToTarget >= 0 && angleToTarget < Pi) {
         /// Rotate left
         setRotation(rotation() - angleToTarget * 180 /Pi);
     }
     else if (angleToTarget <= TwoPi && angleToTarget > Pi) {
         /// Rotate right
         setRotation(rotation() + (angleToTarget - TwoPi )* (-180) /Pi);
     }
     bullet_timer=new QTimer();
     connect(bullet_timer,SIGNAL(timeout()),this,SLOT(move()));
     bullet_timer->start(speed);

}

int Bullet::getPlayerNumber()
{
    return this->player_number;
}
Bullet::~Bullet()
{
    delete bullet_timer;

}

void Bullet::move()
{
    setPos(mapToScene(0,-10));
    if(pos().y()>800||this->y()<-100||this->x()<-100||this->x()>1000){
        delete this;
    }

}
