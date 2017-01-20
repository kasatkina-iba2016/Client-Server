#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QtWidgets>
#include <QTcpSocket>
#include <QFile>
#include <QFileDialog>
#include <QDir>

class MyClient : public QWidget
{
    Q_OBJECT
    QTcpSocket *myTcpSocket;
    QTextEdit *txt;
    QLineEdit *line;
    QPushButton *btn,*btn2;
    qint16 nextBlockSize;
    QFileInfo *fileInfo;
    QString fileName;
    QByteArray buf;
    QLabel *lbl;

public:
    MyClient(const QString &strHost,int nPort,QWidget *parent = 0);
    ~MyClient();
private slots:
    void slotReadyRead();
    void slotSendToServer();
    void slotConnected();
    void slotFileOpen();

};

#endif // MYCLIENT_H
