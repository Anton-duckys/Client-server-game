#ifndef PLAYER_H
#define PLAYER_H
#include<QGraphicsRectItem>
#include<QTimer>
#include<QObject>
#include<QGraphicsItem>
#include<QKeyEvent>
#include<QGraphicsSceneMouseEvent>
#include<QMouseEvent>
#include "scene.h"
#include "bullet.h"
#include "enemy.h"
class Player:public QObject,public QGraphicsRectItem
{
    Q_OBJECT
public:
    Player(QObject *parent = 0);
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
    QPointF target;
    qreal angleToTarget;
    QTimer *gameTimer;
    bool up=false,down=false,left=false,right=false,space=false;
    bool reload=true;
    void setPlayerNumber(int player_number);
    int getPlayerNumber();

public slots:
    void slotGameTimer();   // Игровой слот
    void slotTarget(QPointF point);



private:
    int player_number;
signals:
    void addBullet(Bullet* bullet);
    void sendPosAndRotation(const QString&str,const QPointF &point,const QPointF &rotationtarget);
    void changeHealth(int player_number,int value);
    void signalTargetCoordinateEnemy(QPointF point);
};

#endif // PLAYER_H
