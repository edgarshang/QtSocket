#include <QCoreApplication>
#include <QTcpSocket>
#include <QDebug>
#include <QThread>
#include "Clientdemo.h"
#include "ServerDemo.h"
#include "TextMessage.h"
#include "TestMsgAssembler.h"
#include "txtmeghandler.h"

class Handler : public TxtMsgHandler
{
public:
    void handle(QTcpSocket& socket, TextMessage& message)
    {
        qDebug() << &socket;
        qDebug() << message.type();
        qDebug() << message.length();
        qDebug() << message.data();
    }
};

// 同步接收数据

void SyncClientDemo()
{
    QTcpSocket client;
    char buf[256] = {0};

    client.connectToHost("127.0.0.1", 8080);

    qDebug() << "Connected:" << client.waitForConnected();

    qDebug() << "Send Bytes:" << client.write("D.T.Software");

    qDebug() << "Send Status:" << client.waitForBytesWritten();

    qDebug() << "Data Available:" << client.waitForReadyRead();

    qDebug() << "Revieived Bytes:" << client.read(buf, sizeof(buf) - 1);

    QThread::sleep(5000);
    client.close();

    client.waitForDisconnected();
}

// 异步编程数据

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 同步编程
//    SyncClientDemo();

    // 异步编程
//    ClientDemo client;

//    char data[] = "D.T.Software";

//    client.connectTo("127.0.0.1", 8080);
//    client.send(data, sizeof(data) - 1);
//    TextMessage tm("AB", "1234567890");
//    QString s = tm.serialize();

//    qDebug() << s;

//    TestMsgAssembler as;
//    QSharedPointer<TextMessage> pt;

//    as.prepare(s.toStdString().c_str(), s.length());

//    pt = as.assemble();

//    if(pt != NULL)
//    {
//        qDebug() << "hello, world";
//        qDebug() << pt->type();
//        qDebug() << pt->length();
//        qDebug() << pt->data();
//    }

//    TextMessage message("Demo", "狄泰dsfd软件学院");
//    QByteArray d = message.serialize();

//    qDebug() << d;

////    // type 1
////    TextMessage unknown;

////    unknown.unserialize(d);

////    qDebug() << unknown.type();
////    qDebug() << unknown.length();
////    qDebug() << unknown.data();

//    // type 2
//    TestMsgAssembler as;
//    QSharedPointer<TextMessage> pt;

//    pt = as.assemble(d.data(), d.length());

//    qDebug() << pt->type();
//    qDebug() << pt->length();
//    qDebug() << pt->data();
        qDebug() << "1";
        TextMessage message("Demo", "狄泰dsfd软件学院");
        QByteArray d = message.serialize();
        Handler handler;
        ServerDemo server;
        ClientDemo client;
        qDebug() << "2";
        server.setHandler(&handler);
        server.start(8890);
        qDebug() << "3";
        client.setHandler(&handler);
        qDebug() << client.connectTo("127.0.0.1", 8890);
        client.send(message);


    return a.exec();
}
