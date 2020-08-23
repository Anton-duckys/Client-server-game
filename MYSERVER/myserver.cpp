#include "myserver.h"

MyServer::MyServer(int nPort, QWidget* pwgt) : QWidget(pwgt), m_nNextBlockSize(0)
{
    m_ptcpServer = new QTcpServer(this);
    if (!m_ptcpServer->listen(QHostAddress::Any, nPort)) {
        QMessageBox::critical(0,
                              "Server Error",
                              "Unable to start the server:"
                              + m_ptcpServer->errorString()
                             );
        m_ptcpServer->close();
        return;
    }
    connect(m_ptcpServer, SIGNAL(newConnection()),
            this,         SLOT(slotNewConnection())
           );

    m_ptxt = new QTextEdit;
    m_ptxt->setReadOnly(true);
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(new QLabel("<H1>Server</H1>"));
    pvbxLayout->addWidget(m_ptxt);
    setLayout(pvbxLayout);
}
void MyServer::slotNewConnection()
{
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    int handle=pClientSocket->socketDescriptor();
    m_sockets[pClientSocket->socketDescriptor()]=pClientSocket;
    position[pClientSocket->socketDescriptor()]={0,0};
    players_color[pClientSocket->socketDescriptor()]=color_index++;
    this->m_ptxt->append(QString::number(pClientSocket->socketDescriptor())+" connected.");

    setPlayerSettings(pClientSocket,"color",players_color[handle]);

    for(auto iter=m_sockets.begin();iter!=m_sockets.end();++iter){
        if(iter.key()!=pClientSocket->socketDescriptor()){
            sendToClienToCreateNewPlayer(iter.value(),"newclient",0,0,players_color[pClientSocket->socketDescriptor()]);
        }
    }
    for(auto iter=position.begin();iter!=position.end();++iter){
        if(iter.key()!=pClientSocket->socketDescriptor()){
            sendToClienToCreateNewPlayer(pClientSocket,"newclient",position[iter.key()].first,position[iter.key()].second,players_color[iter.key()]);
        }
    }

    connect(pClientSocket, &QTcpSocket::disconnected,[&](){
    this->socketDis();

    }
           );

    connect(pClientSocket, SIGNAL(readyRead()),
            this,          SLOT(slotReadClient())
           );

if(color_index==2){
    for(auto iter=m_sockets.begin();iter!=m_sockets.end();++iter){
        setPlayerSettings(iter.value(),"start",0);
    }
}

}

void MyServer::slotReadClient()
{
        QTcpSocket* pClientSocket = (QTcpSocket*)sender();
        this->m_ptxt->append(QString::number(pClientSocket->socketDescriptor())+" sended.");
        QDataStream in(pClientSocket);
        in.setVersion(QDataStream::Qt_5_13);
        for (;;) {
            if (!m_nNextBlockSize) {
                if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                    break;
                }
                in >> m_nNextBlockSize;
            }

            if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
                break;
            }
            QTime   time;
            QString str,direction;
            qreal x,y;
            in>>str;
            if(str=="info"){
qDebug()<<str<<endl;
                qreal targetrotation_x,targetrotation_y;
                int player_number,score_value,health_value;
                in>>x>>y>>targetrotation_x>>targetrotation_y>>player_number>>score_value>>health_value;
                  qDebug()<<"Ammo: "<<score_value<<endl;
                for(auto iter=m_sockets.begin();iter!=m_sockets.end();++iter){
                    if(iter.key()!=pClientSocket->socketDescriptor()){
                        sendToClientPosAndRotation(iter.value(),str,x,y,targetrotation_x,targetrotation_y,player_number,score_value,health_value);
                    }
            }
            }
            if(str=="fire"||str=="firestop"){
                            QString strMessage =
                                time.toString() + " " + "Client has sended - " + str;
                            m_ptxt->append(strMessage);

                            m_nNextBlockSize = 0;
                            for(auto iter=m_sockets.begin();iter!=m_sockets.end();++iter){
                                if(iter.key()!=pClientSocket->socketDescriptor()){
                                    sendToClientToCreateBullet(iter.value(),str,x,y);
                                }
                        }
              }
            m_nNextBlockSize = 0;

        }
}

void MyServer::socketDis()
{   for(auto el:m_sockets){

    sendToClient(el,QString::number(el->socketDescriptor())+"Disconnected");
    }


}

void MyServer::sendToClient(QTcpSocket* pSocket, const QString& str)
{   qDebug()<<"Send to client: "<<str<<endl;
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);
    out << quint16(0) << QTime::currentTime() << str;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);


}
//for game server
void MyServer::sendToClienToCreateNewPlayer(QTcpSocket *pSocket,const QString& str,const qreal&x, const qreal&y,int color_index)
{

        QByteArray  arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_13);
        out << quint16(0) << str<<x<<y<<color_index;

        out.device()->seek(0);
        out << quint16(arrBlock.size() - sizeof(quint16));

        pSocket->write(arrBlock);
}

void MyServer::sendToClientPosionOfSecondPlayer(QTcpSocket *pSocket, const QString &str, const QString &direction)
{
        QByteArray  arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_13);
        out << quint16(0) << str<<direction;

        out.device()->seek(0);
        out << quint16(arrBlock.size() - sizeof(quint16));

        pSocket->write(arrBlock);
}

void MyServer::changeClientPosition(qintptr handle, const QString &str)
{
    if(str=="left"){
        position[handle].first-=2;
    }
    else if(str=="right"){
        position[handle].first+=2;
    }
    else if(str=="up"){
        position[handle].second-=2;
    }
    else if(str=="down"){
        position[handle].second+=2;
    }

}

void MyServer::setPlayerSettings(QTcpSocket *pSocket, const QString &str, int color_index)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);
    out << quint16(0) << str<<color_index;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);
}

void MyServer::sendToClientsRotation(QTcpSocket *pSocket, const QString &str, const qreal &x, const qreal &y)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);
    out << quint16(0) << str<<x<<y;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);
}

void MyServer::sendToClientToCreateBullet(QTcpSocket *pSocket, const QString &str, const qreal &x, const qreal &y)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);
    out << quint16(0) << str<<x<<y;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);
}

void MyServer::sendToClientPosAndRotation(QTcpSocket *pSocket, const QString &str, const qreal &x, const qreal &y, const qreal &target_rotation_x, const qreal &target_rotation_y,int player_number,int score_value,int health_value)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);
    out << quint16(0) << str<<x<<y<<target_rotation_x<<target_rotation_y<<player_number<<score_value<<health_value;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);
}
