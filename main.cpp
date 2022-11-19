#include <QCoreApplication>
#include <QTcpSocket>
#include <QDebug>
#include <QThread>
#include "Clientdemo.h"

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
    ClientDemo client;

    char data[] = "D.T.Software";

    client.connectTo("127.0.0.1", 8080);
    client.send(data, sizeof(data) - 1);


    return a.exec();
}
