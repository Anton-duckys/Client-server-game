#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QWidget>
#include<QTcpSocket>
#include<QMessageBox>
#include<QTextEdit>
#include<QLabel>
#include<QTime>
#include<QVBoxLayout>
#include<QGraphicsItem>
#include<QTimer>
#include<QGraphicsScene>
#include<QGraphicsView>
#include<scene.h>
#include"bullet.h"
#include "enemy.h"
#include "text_labels.h"
#include<player.h>
class QTextEdit;
class QLineEdit;

class MyClient : public QWidget {
Q_OBJECT
private:
    QTcpSocket* m_pTcpSocket;
    QTextEdit*  m_ptxtInfo;
    QLineEdit*  m_ptxtInput;
    quint16     m_nNextBlockSize;
    quint16     m_nNextBlockSize2;
    Player *player;
    Player*second_player;
    QGraphicsView*view;
    CustomScene*scene;
    QTimer*back_moving;
    QTimer *timerspawn;
    Score* first_score;
    Ammo* first_ammo;
    Health* first_health;
    Score* second_score;
    Ammo* second_ammo;
    Health* second_health;
    bool start_the_game=false;
public:
    MyClient(const QString& strHost, int nPort, QWidget* pwgt = 0) ;
    void ShowScene();
    void createNewPlayer(QPointF point,int color_index);
    void moveSecondPlayer(const QString& direction);
    void stopToMove(const QString& direction);

private slots:
    void slotReadyRead   (                            );
    void slotError       (QAbstractSocket::SocketError);
    void slotSendToServer(                            );
    void slotSendToServerInfoAboutPlayer( const QString&str,const QString&direction);
    void slotConnected   (                            );
    void sendRotationToTheServer(const QString&str,const QString&direction,const QPointF &point);
    void sendPosAndRotationToServer(const QString&str,const QPointF &point,const QPointF &rotationtarget);
    void sendToServerToCreateBullet(const QString&str,const QString&direction,const QPointF &point);
    void slotReadyFire();
    void slotStopFire();
    void addBullet(Bullet*bullet);
    void SecondPlayerFire(const QString&str);
    void spawn();
    void increaseScore(int player_type, int value);
    void changeHealth(int player_type, int value);
};

#endif // MYCLIENT_H
