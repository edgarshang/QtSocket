#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QTcpServer>
#include <QList>
#include "TextMessage.h"
#include "txtmeghandler.h"


class ServerHandler : public TxtMsgHandler
{
    struct Node
    {
        QString id;
        QString pwd;
        QTcpSocket* socket;
    };

    QList<Node> m_nodeList;
public:
    ServerHandler();
    void handle(QTcpSocket& obj, TextMessage& message);
};

#endif // SERVERHANDLER_H
