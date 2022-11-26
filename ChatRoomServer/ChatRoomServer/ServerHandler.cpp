#include "ServerHandler.h"
#include <QDebug>
ServerHandler::ServerHandler()
{
    #define MapToHandler(MSG) m_handlerMap.insert(#MSG, MSG##_Handler)

    MapToHandler(CONN);
    MapToHandler(DSCN);
    MapToHandler(LGIN);
    MapToHandler(MSGA);
    MapToHandler(MSGP);
    MapToHandler(ADMN);

    static Node admin;

    admin.id = "Admin";
    admin.pwd = "dt0919";
    admin.level = "admin";

    m_nodeList.append(&admin);

}

QString ServerHandler::getOnlineUserID()
{
    QString ret = "";

    for(int i = 0; i < m_nodeList.length(); i++)
    {
        Node* n = m_nodeList.at(i);

        if( n->socket != NULL )
        {
            ret += n->id + '\r';
        }

    }

    return ret;
}

void ServerHandler::sentToAllOnlineUser(TextMessage& message)
{
    const QByteArray& ba = message.serialize();
    for(int i = 0; i < m_nodeList.length(); i++)
    {
        Node* n = m_nodeList.at(i);

        if(n->socket != NULL)
        {
            n->socket->write(ba);
        }
    }
}

void ServerHandler::handle(QTcpSocket& obj, TextMessage& message)
{
    if (m_handlerMap.contains(message.type()) )
    {
        MSGHandler handler = m_handlerMap.value(message.type());
        (this->*handler)(obj, message);
    }
}

void ServerHandler::MSGP_Handler(QTcpSocket& obj, TextMessage& message)
{
    QStringList tl = message.data().split("\r", QString::SkipEmptyParts);
    const QByteArray& ba = TextMessage("MSGA", tl.last()).serialize();


    tl.removeLast();

    for( int i = 0; i < tl.length(); i++ )
    {
        for( int j = 0; j < m_nodeList.length(); j++ )
        {
            Node* n = m_nodeList.at(j);

            if((tl[i] == n->id) && (n->socket != NULL))
            {
                n->socket->write(ba);
            }
        }
    }
}

void ServerHandler::MSGA_Handler(QTcpSocket& obj, TextMessage& message)
{
    sentToAllOnlineUser(message);
}

void ServerHandler::CONN_Handler(QTcpSocket& obj, TextMessage& message)
{

}
void ServerHandler::DSCN_Handler(QTcpSocket& obj, TextMessage& message)
{
    for(int i = 0; i < m_nodeList.length(); i++)
    {
        Node* n = m_nodeList.at(i);

        if(n->socket == &obj)
        {
            n->socket = NULL;
            break;
        }
    }

    TextMessage tm("USER", getOnlineUserID());
    sentToAllOnlineUser(tm);
}
void ServerHandler::LGIN_Handler(QTcpSocket& obj, TextMessage& message)
{
    QString data = message.data();
    int index = data.indexOf('\r');
    QString id = data.mid(0, index);
    QString pwd = data.mid(index+1);
    QString result = "";
    QString level = "";
    QString status = "";

    index = -1;

    for(int i = 0; i < m_nodeList.length(); i++)
    {
        if(id == m_nodeList.at(i)->id)
        {
            index = i;
            break;
        }
    }

    if(index == -1)
    {
        Node* newNode = new Node();

        if( newNode != NULL)
        {
            newNode->id = id;
            newNode->pwd = pwd;
            newNode->socket = &obj;

            m_nodeList.append(newNode);

            result = "LIOK";
            status = newNode->status;
            level = newNode->level;
        }
        else
        {
            result = "LIER";
        }
    }else
    {
        Node* n = m_nodeList.at(index);

        if( pwd == n->pwd)
        {
            n->socket = &obj;

            result = "LIOK";
            status = n->status;
            level = n->level;
        }else
        {
            result = "LIER";
        }
    }

    obj.write(TextMessage(result, id + '\r' + status + '\r' + level).serialize());

    if( result == "LIOK")
    {
        TextMessage tm("USER", getOnlineUserID());
        sentToAllOnlineUser(tm);
    }
}

void ServerHandler::ADMN_Handler(QTcpSocket& obj, TextMessage& message)
{
    QStringList data = message.data().split("\r", QString::SkipEmptyParts);
    QString op = data[0];
    QString id = data[1];

    for( int i = 0; i < m_nodeList.length(); i++ )
    {
        Node* n = m_nodeList.at(i);

        if( (id == n->id) && (n->socket != NULL) && (n->level == "user"))
        {
            n->socket->write(TextMessage("CTRL", op).serialize());
            n->status = op;
            break;
        }
    }
}
