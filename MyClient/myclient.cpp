#include "myclient.h"

MyClient::MyClient(const QString &strHost, int nPort, QWidget *parent)
    : QWidget(parent),nextBlockSize(0)
{
    myTcpSocket=new QTcpSocket(this);
    myTcpSocket->connectToHost(strHost,nPort);
    txt=new QTextEdit;
    btn=new QPushButton("&Send");
    btn2=new QPushButton("&Open File");
    line=new QLineEdit;
    lbl=new QLabel;
    line->setReadOnly(true);
    txt->setReadOnly(true);

    connect(myTcpSocket,SIGNAL(connected()),SLOT(slotConnected()));
    connect(myTcpSocket,SIGNAL(readyRead()),SLOT(slotReadyRead()));
    connect(btn2,SIGNAL(clicked()),SLOT(slotFileOpen()));
    connect(btn,SIGNAL(clicked(bool)),SLOT(slotSendToServer()));

    QVBoxLayout *lay=new QVBoxLayout;
    QHBoxLayout *lay2=new QHBoxLayout;

    lay->addWidget(new QLabel("<h1>Client</h1>"));
    lay->addWidget(txt);
    lay->addWidget(line);
    lay->addWidget(lbl);
    lay2->addWidget(btn2);
    lay2->addWidget(btn);
    lay->addLayout(lay2);
    setLayout(lay);
    resize(300,300);
}

void MyClient::slotReadyRead()
{
    QDataStream stream(myTcpSocket);
    stream.setVersion(QDataStream::Qt_5_7);

    for(;;)
    {
        if (nextBlockSize==0)
        {
            if(myTcpSocket->bytesAvailable()<sizeof(qint16))
               return;
            stream>>nextBlockSize;
        }
        if (nextBlockSize>myTcpSocket->bytesAvailable())
                return;
        QString str;
        QTime time;
        stream>>time>>str;
        txt->append(time.toString()+" "+str);
        nextBlockSize=0;
    }
}

void MyClient::slotSendToServer()
{
    QDataStream stream(&buf,QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_7);
    stream<<qint16(0)<<QTime::currentTime()<<fileName<<buf;
    stream.device()->seek(0);
    stream<<qint16(buf.size()-sizeof(qint16));
    myTcpSocket->write(buf);
    line->setText("");
    lbl->clear();
}

void MyClient::slotConnected()
{
    txt->append("Received the connected() signal");
}

void MyClient::slotFileOpen()
{
    fileName=QFileDialog::getOpenFileName(0,"Open...","","All files(*.*)");
    fileInfo=new QFileInfo(fileName);
    QString base=fileInfo->baseName();
    QString suff=fileInfo->suffix();
    QFile myFile(fileName);
    if (myFile.open(QIODevice::ReadOnly))
    {
     line->setText(base+"."+suff);
     lbl->setPixmap(QIcon(":/new/clip.ico").pixmap(20,20));

     buf=myFile.readAll();
    }
    myFile.close();
}

MyClient::~MyClient()
{

}
