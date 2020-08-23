#ifndef TEXT_LABELS_H
#define TEXT_LABELS_H


#include<QGraphicsItem>

class Text_labels:public QGraphicsTextItem
{
public:
    Text_labels(QGraphicsTextItem*parent);
    virtual void increase(int value=0)=0;
    virtual void decrease(int value=0)=0;
    virtual int get_value()=0;

};

class Health:public Text_labels{
public:
    Health(int color_index,QGraphicsTextItem*parent=nullptr);
    void increase(int value=1) override;
    void decrease(int value=1) override;
    int get_value() override;
    private:
        int value;

};
class Score:public Text_labels
{
public:
    Score(int color_index,QGraphicsTextItem*parent =nullptr);
     void increase(int value=1) override;
     int get_value() override;
     void decrease(int value=1) override;
private:
     int value;

};

class Ammo:public Text_labels
{
public:
    Ammo(int color_index,QGraphicsTextItem*parent =nullptr);

     void increase(int value=0) override;
     int get_value() override;
     void decrease(int value=0) override;
public:

     int value;
};

#endif // TEXT_LABELS_H
