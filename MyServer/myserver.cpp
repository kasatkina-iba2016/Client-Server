#include "myserver.h"

MyServer::MyServer(int port, QWidget *parent)
    : QWidget(parent),nextBlockSize(0)
{
     myTcpServer=new QTcpServer(this);

     if (!myTcpServer->listen(QHostAddress::Any,port))
     {
       QMessageBox::critical(0,"Server Error!"," Unable to start the server: "+myTcpServer->errorString());
       myTcpServer->close();
       return;
     }
     connect(myTcpServer,SIGNAL(newConnection()),SLOT(slotNewConnection()));

     txt=new QTextEdit;
     txt->setReadOnly(true);
     QVBoxLayout *lay=new QVBoxLayout;
     lay->addWidget(new QLabel("<h1>Server</h1>"));
     lay->addWidget(txt);
     setLayout(lay);
     resize(300,300);
}

void MyServer::slotNewConnection()
{
    myClientSocket=myTcpServer->nextPendingConnection();
    connect(myClientSocket,SIGNAL(disconnected()),myClientSocket,SLOT(deleteLater()));
    connect(myClientSocket,SIGNAL(readyRead()),this,SLOT(slotReadClient()));
    sentToClient(myClientSocket,"Connected!");
}

void MyServer::slotReadClient()
{
     myClientSocket=(QTcpSocket*)sender();
     QDataStream stream(myClientSocket);
     stream.setVersion(QDataStream::Qt_5_7);
     for(;;)
     {
         if (nextBlockSize==0)
         {
             if(myClientSocket->bytesAvailable()<sizeof(qint16))
                return;
             stream>>nextBlockSize;
         }
         if (nextBlockSize>myClientSocket->bytesAvailable())
                 return;
         QByteArray arrFile;
         QTime time;
         QString fileName,strMessage;
         stream>>time>>fileName>>arrFile;
         QFileInfo fileInfo(fileName);
         QString baseName=fileInfo.baseName();
         QString suff=fileInfo.suffix();
         QFile file("D:\\"+baseName+"."+suff);
         file.open(QIODevice::WriteOnly);
         file.write(arrFile);
         file.close();

         strMessage=time.toString()+" Client has sent "+baseName+"."+suff;
         txt->append(strMessage);
         nextBlockSize=0;
         sentToClient(myClientSocket,"Server reseved "+baseName+"."+suff);
     }
  }

void MyServer::sentToClient(QTcpSocket *pSocket, const QString &str)
{
    QByteArray arrBlock;
    QDataStream stream(&arrBlock,QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_7);
    stream<<qint16(0)<<QTime::currentTime()<<str;
    stream.device()->seek(0);
    stream<<qint16(arrBlock.size()-sizeof(qint16));
    pSocket->write(arrBlock);
}

MyServer::~MyServer()
{

}
