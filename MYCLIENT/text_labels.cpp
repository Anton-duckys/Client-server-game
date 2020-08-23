#include "text_labels.h"

#include<QFont>

Text_labels::Text_labels(QGraphicsTextItem *parent):QGraphicsTextItem(parent)
{

};
Health::Health(int color_index,QGraphicsTextItem *parent):Text_labels(parent)
{

    this->value=3;
    setPlainText(QString("Health: ")+QString::number( this->value));
    if(color_index==0){
        setDefaultTextColor(Qt::red);
    }
    else if(color_index==1){

        setDefaultTextColor(Qt::blue);
    }
    setFont(QFont("times",16));
}

void Health::increase(int value)
{
    this->value+=value;
    setPlainText(QString("Health: ")+QString::number( this->value));
}

void Health::decrease(int value)
{
    this->value-=value;
    setPlainText(QString("Health: ")+QString::number( this->value));
}

int Health::get_value()
{
    return this->value;
}



Score::Score(int color_index,QGraphicsTextItem *parent):Text_labels(parent)
{
    this->value=0;
    setPlainText(QString("Score: ")+QString::number( this->value));
    if(color_index==0){
        setDefaultTextColor(Qt::red);
    }
    else if(color_index==1){

        setDefaultTextColor(Qt::blue);
    }
    setFont(QFont("times",16));
}
void Score::increase(int value)
{
    this->value+=value;
    setPlainText(QString("Score: ")+QString::number( this->value));
}

int Score::get_value()
{
    return this->value;
}
void Score::decrease(int value)
{
this->value-=value;
}
Ammo::Ammo(int color_index,QGraphicsTextItem*parrent):Text_labels(parrent){
    this->value=9000;
    setPlainText(QString("Ammo: ")+QString::number(this->value));
    if(color_index==0){
        setDefaultTextColor(Qt::red);
    }
    else if(color_index==1){

        setDefaultTextColor(Qt::blue);
    }
    setFont(QFont("times",16));
}
void Ammo::decrease(int value)
{
    this->value-=value;
    this->setPlainText(QString("Ammo: ")+QString::number(this->value));
}

void Ammo::increase(int value)
{
    this->value+=value;
    setPlainText(QString("Ammo: ")+QString::number(this->value));
}

int Ammo:: get_value(){
    return this->value;
}

