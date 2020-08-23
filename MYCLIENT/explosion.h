#ifndef EXPLOSION_H
#define EXPLOSION_H


#include<QGraphicsPixmapItem>
#include<QTimer>
class Explosion:public QObject,public QGraphicsPixmapItem
{Q_OBJECT
public:
    Explosion(QPointF point);
    QTimer*explosion_animation;
    int frame=0;
    QPixmap *p;
    ~Explosion();

public slots:
    void increase_frame();

};

#endif // EXPLOSION_H
