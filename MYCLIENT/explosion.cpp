#include "explosion.h"

Explosion::Explosion(QPointF point):QGraphicsPixmapItem()
{

  p=new QPixmap(":/images/Explosion_2.png");
  QPixmap copy=p->copy(QRect(0,0,96,96));
  setPos(point.x(),point.y());
  setPixmap(copy.scaled(120,120,Qt::IgnoreAspectRatio));
  QTimer::singleShot(480,[&](){explosion_animation->stop();delete this;});
  explosion_animation=new QTimer();
  connect(explosion_animation,SIGNAL(timeout()),this,SLOT(increase_frame()));
  explosion_animation->setInterval(40);
  explosion_animation->start();

}



void Explosion::increase_frame()
{


    ++frame;
    QPixmap copy=p->copy(QRect(frame*96,0,96,96));
    setPixmap(copy.scaled(120,120,Qt::IgnoreAspectRatio));



}

Explosion::~Explosion()
{

    delete explosion_animation;
    delete p;
}
