#ifndef MYSERVER_H
#define MYSERVER_H

#include <QtWidgets>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>


class MyServer : public QWidget
{
    Q_OBJECT
    QTcpServer *myTcpServer;
    QTcpSocket *myClientSocket;
    QTextEdit *txt;
    qint16 nextBlockSize;

private:
    void sentToClient(QTcpSocket *pSocket, const QString &str);
public:
    MyServer(int port,QWidget *parent = 0);
    ~MyServer();
public slots:
    void slotNewConnection();
    void slotReadClient();
};

#endif // MYSERVER_H
