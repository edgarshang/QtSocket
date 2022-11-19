#ifndef SERVERDEMO_H
#define SERVERDEMO_H

#include <QObject>
#include <QTcpServer>

class ServerDemo:public QObject
{
    Q_OBJECT
    QTcpServer m_server;
public:
    ServerDemo(QObject *parent = NULL);
    bool start(int port);
    void stop();
    ~ServerDemo();

protected slots:
    void onNewConnection();
    void onDisconnected();
    void onDataReady();
    void onBytesWritten(qint64 bytes);
    void onConnected();
};

#endif // SERVERDEMO_H
