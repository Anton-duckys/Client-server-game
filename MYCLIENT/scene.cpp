#include "scene.h"
CustomScene::CustomScene(QObject *parent) :
    QGraphicsScene()
{
    QPixmap p1(":/images/space_back_2.png");
    QPixmap p2(":/images/space_back_2.png");
    background=new QGraphicsPixmapItem();
    background2=new QGraphicsPixmapItem();
    background->setPixmap(p1.scaled(1000,800,Qt::IgnoreAspectRatio));
    background2->setPixmap(p2.scaled(1000,800,Qt::IgnoreAspectRatio));

    background->setPos(0,0);
    this->addItem(background);
    background2->setPos(0,-800);
    this->addItem(background2);



}
void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit signalTargetCoordinate(event->scenePos());
 //  // qDebug()<<game->player->x()<<" and "<<game->player->y()<<endl;
}
void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    Q_UNUSED(event);
    emit readyFire();
}
void CustomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    Q_UNUSED(event);
    emit stopFire();
}
void CustomScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){
    Q_UNUSED(event);
}

void CustomScene::back_ground_moving()
{
    this->background->setPos(this->background->x(),this->background->y()+5);
    this->background2->setPos(this->background2->x(),this->background2->y()+5);

    if(this->background->y()>this->height()){
        this->background->setPos(0,background2->y()-800);
    }
   if(this->background2->y()>this->height()){
        this->background2->setPos(0,background->y()-800);
   }


     //qDebug()<<background->y()<<endl;

}

void CustomScene::addExplosion(Explosion* explosion){
    this->addItem(explosion);
}
