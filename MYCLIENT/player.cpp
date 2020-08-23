#include "player.h"
#include"qmath.h"
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

void Player::slotTarget(QPointF point)
{
    target = point;

        QLineF lineToTarget(QPointF(0, 0), mapFromScene(target));
        // Угол поворота в направлении к цели
        qreal angleToTarget = ::acos(lineToTarget.dx() / lineToTarget.length());
        if (lineToTarget.dy() < 0)
            angleToTarget = TwoPi - angleToTarget;
        angleToTarget = normalizeAngle((Pi - angleToTarget) + Pi / 2);

        /* В Зависимости от того, слева или справа находится Цель от Героя,
         * устанавливаем направление поворота Героя в данном тике таймера
         * */
        if (angleToTarget >= 0 && angleToTarget < Pi) {
            // Rotate left
            setRotation(rotation() - angleToTarget * 180 /Pi);
        } else if (angleToTarget <= TwoPi && angleToTarget > Pi) {
            // Rotate right
            setRotation(rotation() + (angleToTarget - TwoPi )* (-180) /Pi);
        }


}

void Player::setPlayerNumber(int player_number)
{
    this->player_number=player_number;
}

int Player::getPlayerNumber()
{
    return player_number;
}

Player::Player(QObject *parent):QObject(parent),QGraphicsRectItem()
{
    setRotation(0);
    this->setRect(-50,-50,100,100);
    gameTimer = new QTimer();   // Инициализируем игровой таймер
        // Подключаем сигнал от таймера и слоту обработки игрового таймера
    connect(gameTimer, &QTimer::timeout, this, &Player::slotGameTimer);

    gameTimer->start(10);
    target=QPointF(0,0);


}

void Player::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_A ){

    left=true;
    }
    else if(event->key()==Qt::Key_D){

    right=true;
    }
    else if(event->key()==Qt::Key_W){

    up=true;
    }
    else if(event->key()==Qt::Key_S){

    down=true;
    }
}

void Player::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_A ){

    left=false;
}
    else if(event->key()==Qt::Key_D){

    right=false;
    }
    else if(event->key()==Qt::Key_W){

    up=false;
    }
    else if(event->key()==Qt::Key_S){

    down=false;
    }
}


void Player::slotGameTimer()
{
    emit signalTargetCoordinateEnemy(QPointF(this->x(),this->y()));
    QList<QGraphicsItem *> colide=collidingItems();
    for(int i=0,n=colide.size();i<n;++i){
        if(typeid (*(colide[i]))==typeid (Bullet)){

            if(static_cast<Bullet*>(colide[i])->getPlayerNumber()!=this->getPlayerNumber()){
            scene()->removeItem(colide[i]);
            delete colide[i];

            }
        }
    }
    if(left){
           this->setX(this->x() - 2);
       }

       if(right){
           this->setX(this->x() + 2);
       }
       if(up){
           this->setY(this->y() - 2);
       }
       if(down){
           this->setY(this->y() + 2);
       }
       if(space&&reload){


        Bullet * bullet=new Bullet(this->getPlayerNumber(),mapToScene(-20,-50),QPointF(this->target.x()-20*(cos(rotation()*Pi/180)),this->target.y()-20*(sin(rotation()*Pi/180))));
        emit addBullet(bullet);
        reload=false;
        QTimer::singleShot(50, this, [&](){reload=true;});

       }

       QLineF lineToTarget(QPointF(0, 0), mapFromScene(target));
           // Угол поворота в направлении к цели
           qreal angleToTarget = ::acos(lineToTarget.dx() / lineToTarget.length());
           if (lineToTarget.dy() < 0)
               angleToTarget = TwoPi - angleToTarget;
           angleToTarget = normalizeAngle((Pi - angleToTarget) + Pi / 2);

           /* В Зависимости от того, слева или справа находится Цель от Героя,
            * устанавливаем направление поворота Героя в данном тике таймера
            * */
          if (angleToTarget >= 0 && angleToTarget < Pi) {
               // Rotate left

               setRotation(rotation() - angleToTarget * 180 /Pi);
           } else if (angleToTarget <= TwoPi && angleToTarget > Pi) {
               // Rotate right
               setRotation(rotation() + (angleToTarget - TwoPi )* (-180) /Pi);
           }
      emit sendPosAndRotation("info",QPointF(this->x(),this->y()),target);
          QList<QGraphicsItem *> colide_enemy=collidingItems();
              for(int i=0,n=colide_enemy.size();i<n;++i){
                  if(typeid (*(colide_enemy[i]))==typeid (Enemy)){

                            emit changeHealth(this->getPlayerNumber(),1);
                            delete colide_enemy[i];
                  }
              }
}
