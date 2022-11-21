#ifndef TXTMEGHANDLER_H
#define TXTMEGHANDLER_H

#include <QTcpSocket>
#include "TextMessage.h"

class TxtMsgHandler
{
public:
    virtual void handle(QTcpSocket&, TextMessage&) = 0;
};

#endif // TXTMEGHANDLER_H
