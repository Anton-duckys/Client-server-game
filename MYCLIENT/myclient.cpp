#include "myclient.h"
#include<QLineEdit>
#include<QPushButton>
#include"qmath.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;
MyClient::MyClient(const QString& strHost, int nPort, QWidget* pwgt) : QWidget(pwgt), m_nNextBlockSize(0){
    m_pTcpSocket = new QTcpSocket(this);
    m_pTcpSocket->connectToHost(strHost, nPort);
    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,         SLOT(slotError(QAbstractSocket::SocketError))
           );

    m_ptxtInfo  = new QTextEdit;
    m_ptxtInput = new QLineEdit;

    connect(m_ptxtInput, SIGNAL(returnPressed()),
            this,        SLOT(slotSendToServer())
           );
    m_ptxtInfo->setReadOnly(true);

    QPushButton* pcmd = new QPushButton("&Send");
    connect(pcmd, SIGNAL(clicked()), SLOT(slotSendToServer()));
    QPushButton* exit = new QPushButton("&Exit");
    connect(exit, &QPushButton::clicked,[&](){
        QByteArray  arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_2);
        QString str="disconnect "+QString::number(m_pTcpSocket->socketDescriptor());
        out << quint16(0) << QTime::currentTime() << str;

        out.device()->seek(0);
        out << quint16(arrBlock.size() - sizeof(quint16));

        m_pTcpSocket->write(arrBlock);
        m_pTcpSocket->disconnectFromHost();
    });


    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(new QLabel("<H1>Client</H1>"));
    pvbxLayout->addWidget(m_ptxtInfo);
    pvbxLayout->addWidget(m_ptxtInput);
    pvbxLayout->addWidget(pcmd);
    pvbxLayout->addWidget(exit);
    setLayout(pvbxLayout);

    this->scene=new CustomScene();
    player=new Player();
    scene->addItem(player);
    view=new QGraphicsView(scene);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player-> setFocus();

    scene->setSceneRect(0,0,1000,800);
    qDebug()<<"Scene constructed "<<endl;
    view->setFixedSize(1000,800);
    view->setMouseTracking(true);
    view->setRenderHint(QPainter::Antialiasing);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    back_moving=new QTimer();
    connect(back_moving,SIGNAL(timeout()),scene,SLOT(back_ground_moving()));

    timerspawn=new QTimer();


    connect(timerspawn,SIGNAL(timeout()),this,SLOT(spawn()));
    connect(scene,&CustomScene::signalTargetCoordinate,player,&Player::slotTarget);

    connect(scene,&CustomScene::readyFire,this,&MyClient::slotReadyFire);
    connect(scene,&CustomScene::stopFire,this,&MyClient::slotStopFire);
    connect(player,&Player::addBullet,this,&MyClient::addBullet);
    connect(player,&Player::sendPosAndRotation,this,&MyClient::sendPosAndRotationToServer);
    connect(player, &Player::changeHealth,this,&MyClient::changeHealth);


}

void MyClient::ShowScene()
{
    this->view->show();
}

void MyClient::createNewPlayer(QPointF point,int color_index)
{
    second_player=new Player();
    second_player->setPos(point);
    if(color_index==0){
        second_player->setBrush(Qt::red);
        second_player->setPlayerNumber(color_index);
        first_score=new Score(color_index);
        scene->addItem(first_score);
        first_health=new Health(color_index);
        first_health->setPos(first_health->x(),first_health->y()+25);
        scene->addItem(first_health);
        first_ammo=new Ammo(color_index);
        first_ammo->setPos(first_ammo->x(),first_ammo->y()+50);
        scene->addItem(first_ammo);
    }
    else if(color_index==1){
        second_player->setBrush(Qt::blue);
        second_player->setPlayerNumber(color_index);
        second_score=new Score(color_index);
        second_score->setPos(second_score->x()+700,second_score->y());
        scene->addItem(second_score);
        second_health=new Health(color_index);
        second_health->setPos(second_health->x()+700,second_health->y()+25);
        scene->addItem(second_health);
        second_ammo=new Ammo(color_index);
        second_ammo->setPos(second_ammo->x()+700,second_ammo->y()+50);
        scene->addItem(second_ammo);
    }
    scene->addItem(second_player);
    connect(second_player,&Player::addBullet,this,&MyClient::addBullet);
}

void MyClient::moveSecondPlayer(const QString&direction){

    if(direction=="left"){
        second_player->left=true;

    }
    else if(direction=="down"){
        second_player->down=true;

    }
    else if(direction=="right"){
        second_player->right=true;

    }
    else if(direction=="up"){
        second_player->up=true;
}

}

void MyClient::stopToMove(const QString &direction)
{
    if(direction=="left"){
        second_player->left=false;
    }
    else if(direction=="down"){
        second_player->down=false;

    }
    else if(direction=="right"){
        second_player->right=false;

    }
    else if(direction=="up"){
        second_player->up=false;

    }
}
void MyClient::slotReadyRead()
{

    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_5_13);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (m_pTcpSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }

        QString str;
        in  >> str;

        if(str=="newclient"){
            qreal x,y;
            int color_index;
            in>>x>>y>>color_index;
            createNewPlayer(QPointF(x,y),color_index);
        }
        if(str=="info"){

            qreal x,y,rotationtarget_x,rotationtarget_y;
            int player_number,score_value,health_value;
            in>>x>>y>>rotationtarget_x>>rotationtarget_y>>player_number>>score_value>>health_value;
            second_player->setX(x);
            second_player->setY(y);
            second_player->target=QPointF(rotationtarget_x,rotationtarget_y);
            if(player_number==0){
                first_ammo->setPlainText(QString("Ammo: ")+QString::number( score_value));
                first_health->setPlainText(QString("Health: ")+QString::number( health_value));
            }
            else if(player_number==1){
                second_ammo->setPlainText(QString("Ammo: ")+QString::number( score_value));
                second_health->setPlainText(QString("Health: ")+QString::number( health_value));
            }

        }
        if(str=="rotate"){
            qreal target_x,target_y;
            in>>target_x>>target_y;
            second_player->target=QPointF(target_x,target_y);
        }
        if(str=="fire"){

            qreal target_x,target_y;
            in>>target_x>>target_y;
           SecondPlayerFire(str);


        }
        if(str=="firestop"){

            qreal target_x,target_y;
            in>>target_x>>target_y;
            SecondPlayerFire(str);
        }
        if(str=="move"){
            QString direction;
            in >> direction;
            moveSecondPlayer(direction);

        }
        else if(str=="stop"){
            QString direction;
            in>>direction;
            stopToMove(direction);
        }
        if(str=="color"){
            int color_index;
            in>>color_index;
            if(color_index==0){
                player->setBrush(Qt::red);
                player->setPlayerNumber(color_index);
                first_score=new Score(color_index);
                scene->addItem(first_score);
                first_health=new Health(color_index);
                first_health->setPos(first_health->x(),first_health->y()+25);
                scene->addItem(first_health);
                first_ammo=new Ammo(color_index);
                first_ammo->setPos(first_ammo->x(),first_ammo->y()+50);
                scene->addItem(first_ammo);
            }
            else if(color_index==1){
                player->setBrush(Qt::blue);
                player->setPlayerNumber(color_index);
                second_score=new Score(color_index);
                second_score->setPos(second_score->x()+700,second_score->y());
                scene->addItem(second_score);
               second_health=new Health(color_index);
                second_health->setPos(second_health->x()+700,second_health->y()+25);
                scene->addItem(second_health);
                second_ammo=new Ammo(color_index);
                second_ammo->setPos(second_ammo->x()+700,second_ammo->y()+50);
                scene->addItem(second_ammo);
            }

        }
        if(str=="start"){
            int useless;
            in>>useless;
            start_the_game=true;
            back_moving->start(25);
            timerspawn->start(2000);

        }

        m_nNextBlockSize = 0;
    }


}

void MyClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(m_pTcpSocket->errorString())
                    );
    m_ptxtInfo->append(strError);
}

void MyClient::slotSendToServer()
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);
    out << quint16(0) << QTime::currentTime() << m_ptxtInput->text();

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    m_pTcpSocket->write(arrBlock);
    m_ptxtInput->setText("");
}

void MyClient::slotSendToServerInfoAboutPlayer(const QString &str, const QString& direction)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);
    out << quint16(0) << str << direction<<player->x()<<player->y();


    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    m_pTcpSocket->write(arrBlock);
}

void MyClient::slotConnected()
{
    m_ptxtInfo->append("Received the connected() signal");
}

void MyClient::sendRotationToTheServer(const QString&str,const QString&direction,const QPointF &point)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    out << quint16(0) <<str<<direction<<point.x()<<point.y() ;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    m_pTcpSocket->write(arrBlock);
}

void MyClient::sendPosAndRotationToServer(const QString &str, const QPointF &point, const QPointF &rotationtarget)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    out << quint16(0) <<str<<point.x()<<point.y()<<rotationtarget.x()<<rotationtarget.y()<<player->getPlayerNumber();

    if(start_the_game){

        if(player->getPlayerNumber()==0){
                out<<first_ammo->get_value()<<first_health->get_value();
            }
            else if(player->getPlayerNumber()==1){
                out<<second_ammo->get_value()<<second_health->get_value();
            }
    }
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    m_pTcpSocket->write(arrBlock);
}

void MyClient::sendToServerToCreateBullet(const QString &str,const QString&direction, const QPointF &point){
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);

    out << quint16(0) <<str<<direction<<point.x()<<point.y() ;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    m_pTcpSocket->write(arrBlock);
}

void MyClient::slotReadyFire(){

    player->space=true;
    sendToServerToCreateBullet("fire","nothing",QPointF(player->x()-20,player->y()-50));

}

void MyClient::slotStopFire(){
    player->space=false;
    sendToServerToCreateBullet("firestop","nothing",QPointF(player->x()-20,player->y()-50));
}

void MyClient::addBullet(Bullet *bullet){

    scene->addItem(bullet);
    if(bullet->getPlayerNumber()==0)
        first_ammo->decrease(1);
    else if(bullet->getPlayerNumber()==1)
        second_ammo->decrease(1);

}

void MyClient::SecondPlayerFire(const QString &str)
{
    if(str=="fire"){
        second_player->space=true;
    }
    else second_player->space=false;
}

void MyClient::spawn()
{
    Enemy *enemy=new Enemy();
    scene->addItem(enemy);
    connect(enemy,&Enemy::addExplosion,scene,&CustomScene::addExplosion);
    connect(enemy,&Enemy::increaseScore,this,&MyClient::increaseScore);

}

void MyClient::increaseScore(int player_type, int value)
{
    if(player_type==0){
        first_score->increase(value);

    }
    else if(player_type==1){
        second_score->increase(value);
    }
}

void MyClient::changeHealth(int player_type, int value)
{
    if(player_type==0){
        first_health->decrease(value);

    }
    else if(player_type==1){
        second_health->decrease(value);
    }
}

