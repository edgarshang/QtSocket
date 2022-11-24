#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QTcpServer>
#include <QList>
#include <QMap>
#include "TextMessage.h"
#include "txtmeghandler.h"


class ServerHandler : public TxtMsgHandler
{
    typedef void(ServerHandler::*MSGHandler)(QTcpSocket&, TextMessage&);
    struct Node
    {
        QString id;
        QString pwd;
        QTcpSocket* socket;
    public:
        Node():id(""), pwd(""), socket(NULL)
        {

        }
    };

    QList<Node*> m_nodeList;
    QMap<QString, MSGHandler> m_handlerMap;

    void CONN_Handler(QTcpSocket& obj, TextMessage&);
    void DSCN_Handler(QTcpSocket& obj, TextMessage&);
    void LGIN_Handler(QTcpSocket& obj, TextMessage&);
    void MSGA_Handler(QTcpSocket& obj, TextMessage&);
public:
    ServerHandler();
    void handle(QTcpSocket& obj, TextMessage& message);
};

#endif // SERVERHANDLER_H
