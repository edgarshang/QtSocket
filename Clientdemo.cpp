#include "Clientdemo.h"
#include <QHostAddress>
#include <QDebug>

ClientDemo::ClientDemo(QObject* parent):QObject(parent),m_handler(NULL)
{
    qDebug() << "ClientDemo::ClientDemo";
    connect(&m_client, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(&m_client, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(&m_client, SIGNAL(readyRead()), this, SLOT(onDataReady()));
    connect(&m_client, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten(qint64)));
}


void ClientDemo::onConnected()
{
    if (m_handler != NULL)
    {
        TextMessage conn("CONN",m_client.peerAddress().toString()+":"+QString::number(m_client.peerPort()));
        m_handler->handle(m_client,conn);
    }
}

void ClientDemo::setHandler(TxtMsgHandler* handler)
{
    m_handler = handler;
}
void ClientDemo::onDisconnected()
{
    m_assembler.reset();
    if(m_handler != NULL)
    {
        TextMessage dscn("DSCN", "");
        m_handler->handle(m_client, dscn);
    }
}

void ClientDemo::onDataReady()
{
    char buf[256] = {0};

    int len = 0;

    while ((len = m_client.read(buf, sizeof(buf))) > 0 )
    {
        QSharedPointer<TextMessage> ptm = m_assembler.assemble(buf, len);

        if((ptm != NULL) && (m_handler != NULL))
        {
            qDebug() << "ClientDemo::onDataReady()";
            m_handler->handle(m_client, *ptm);
        }
    }
}

void ClientDemo::onBytesWritten(qint64 bytes)
{
//    qDebug() << "onBytesWritten:" << bytes;
    (void)bytes;
}

bool ClientDemo::connectTo(QString ip, int port)
{
    m_client.connectToHost(ip, port);
    return m_client.waitForConnected();
}

qint64 ClientDemo::send(TextMessage& message)
{
    QByteArray ba = message.serialize();

    return m_client.write(ba.data(), ba.length());
}

qint64 ClientDemo::available()
{
    return m_client.bytesAvailable();
}

void ClientDemo::close()
{
    m_client.close();
}
