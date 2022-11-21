#include "ServerDemo.h"
#include <QHostAddress>
#include <QTcpSocket>
#include <QObjectList>
#include <QDebug>


ServerDemo::ServerDemo(QObject *parent) : QObject(parent), m_handler(NULL)
{
    connect(&m_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    qDebug() << "ServerDemo::ServerDemo()";
}

void ServerDemo::setHandler(TxtMsgHandler *handler)
{
    m_handler = handler;
}

void ServerDemo::onNewConnection()
{
    qDebug() << "ServerDemo::onNewConnection()";
    QTcpSocket *tcp = m_server.nextPendingConnection();
    TestMsgAssembler* assembler = new TestMsgAssembler();
    m_map.insert(tcp, assembler);

    connect(tcp, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(tcp, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(tcp, SIGNAL(readyRead()), this, SLOT(onDataReady()));
    connect(tcp, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten(qint64)));

    if( m_handler != NULL )
    {
        TextMessage msg("CONN", tcp->peerAddress().toString() + ":" + QString::number(tcp->peerPort()));
        m_handler->handle(*tcp, msg);
    }
}

bool ServerDemo::start(int port)
{
    bool ret = true;
    if (!m_server.isListening())
    {
        qDebug() << "ServerDemo::start(int port)";
        ret = m_server.listen(QHostAddress("127.0.0.1"), port);
    }

    return ret;
}

void ServerDemo::stop()
{
    if(m_server.isListening())
    {
        m_server.close();
    }
}

void ServerDemo::onConnected()
{


}


void ServerDemo::onDisconnected()
{
    QTcpSocket* tcp = dynamic_cast<QTcpSocket*>(sender());
    if(tcp != NULL)
    {
        delete m_map.take(tcp);

        if( m_handler != NULL )
        {
            TextMessage msg("DSCN", "");
            m_handler->handle(*tcp, msg);
        }
    }
}

void ServerDemo::onDataReady()
{
    QTcpSocket *tcp = dynamic_cast<QTcpSocket*>(sender());

    char buf[256] = {0};

    int len = 0;

    if(tcp != NULL)
    {
        TestMsgAssembler *assembler = m_map.value(tcp);
        while ((len = tcp->read(buf, sizeof(buf))) > 0)
        {
            QSharedPointer<TextMessage> ptm = (assembler != NULL) ? assembler->assemble(buf, len) : NULL;

            if((ptm != NULL) && (m_handler != NULL))
            {
                qDebug() << "ServerDemo::onDataReady()";
                m_handler->handle(*tcp, *ptm);
            }
        }
    }
}

void ServerDemo::onBytesWritten(qint64 bytes)
{
//    qDebug() << "onBytesWritten" << bytes;
    (void)bytes;
}

ServerDemo::~ServerDemo()
{
    const QObjectList& list = m_server.children();
    for(int i = 0; i<list.length(); i++)
    {
        QTcpSocket *tcp = dynamic_cast<QTcpSocket*>(list[i]);
        if(tcp != NULL)
        {
            tcp->close();
        }
    }

    const QList<TestMsgAssembler*>& al = m_map.values();
    for(int i = 0; i<al.length(); i++)
    {
        delete al.at(i);
    }
}
