#ifndef MYSERVER_H
#define MYSERVER_H


#include <QtGui>

#include<QTcpServer>

#include<QTcpSocket>
#include<QMessageBox>
#include<QTextEdit>
#include<QLabel>
#include<QVBoxLayout>
class QTcpServer;
class QTextEdit;
class QTcpSocket;

class MyServer : public QWidget {
Q_OBJECT
private:
    QTcpServer* m_ptcpServer;
    QTextEdit*  m_ptxt;
    quint16     m_nNextBlockSize;
    int color_index=0;


private:
    void sendToClient(QTcpSocket* pSocket, const QString& str);
    void sendToClienToCreateNewPlayer(QTcpSocket* pSocket,const QString& str,const qreal&x,const qreal&y,int color_index);
    void sendToClientPosionOfSecondPlayer(QTcpSocket* pSocket,const QString& str,const QString& direction);
    void changeClientPosition(qintptr handle,const QString& str);
    void setPlayerSettings(QTcpSocket* pSocket,const QString& str,int color_index);
    void sendToClientsRotation(QTcpSocket* pSocket,const QString&str,const qreal&x,const qreal&y);
    void sendToClientToCreateBullet(QTcpSocket* pSocket,const QString&str,const qreal&x,const qreal&y);
    void sendToClientPosAndRotation(QTcpSocket* pSocket,const QString&str,const qreal&x,const qreal&y,const qreal&target_rotation_x,const qreal&target_rotation_y,int player_number,int score_value,int health_value);
    void spawnEnemies(QTcpSocket* pSocket);
public:
    QMap<qintptr,QTcpSocket*>m_sockets;
    QMap<qintptr,QPair<qreal,qreal>>position;
    QMap<qintptr,int>players_color;
    MyServer(int nPort, QWidget* pwgt = 0);
    virtual ~MyServer(){};

public slots:
    virtual void slotNewConnection();
            void slotReadClient   ();
            void socketDis();
};

#endif // MYSERVER_H
