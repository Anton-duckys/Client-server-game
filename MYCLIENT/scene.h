#ifndef SCENE_H
#define SCENE_H


#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include<qdebug.h>
#include<QTimer>
#include<QGraphicsPixmapItem>
#include<explosion.h>
class CustomScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CustomScene(QObject *parent = 0);
   // ~CustomScene();

private:
    QGraphicsPixmapItem* background;
   QGraphicsPixmapItem* background2;

signals:
    // Сигнал для передачи координат положения курсора мыши
    void signalTargetCoordinate(QPointF point);
    //void signalShot(); // Сигнал на стрельбу
    void readyFire();
    void stopFire();

public:
    // Функция, в которой производится отслеживание положения мыши
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

public slots:
    void back_ground_moving();

    void addExplosion(Explosion* explosion);

};
#endif // SCENE_H
