#ifndef SERVERDEMO_H
#define SERVERDEMO_H

#include <QObject>
#include <QTcpServer>
#include <QMap>
#include "TextMessage.h"
#include "txtmeghandler.h"
#include "TestMsgAssembler.h"

class ServerDemo:public QObject
{
    Q_OBJECT
    QTcpServer m_server;
    TestMsgAssembler m_assembler;
    QMap<QTcpSocket*, TestMsgAssembler*> m_map;
    TxtMsgHandler *m_handler;
public:
    ServerDemo(QObject *parent = NULL);
    bool start(int port);
    void stop();
    void setHandler(TxtMsgHandler* handler);
    ~ServerDemo();

protected slots:
    void onNewConnection();
    void onDisconnected();
    void onDataReady();
    void onBytesWritten(qint64 bytes);
    void onConnected();
};

#endif // SERVERDEMO_H
