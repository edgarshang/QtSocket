#ifndef TESTMSGASSEMBLER_H
#define TESTMSGASSEMBLER_H

#include <QObject>
#include <QQueue>
#include <QSharedPointer>
#include "TextMessage.h"

class TestMsgAssembler : public QObject
{
    QQueue<char> m_queue;
    QString m_type;
    int m_length;
    QByteArray m_data;

    void clear();
    QByteArray fetch(int n);
    bool makeTypeAndLength();
    TextMessage* makeMessage();
public:
    TestMsgAssembler(QObject *parent = NULL);
    void prepare(const char *data, int len);
    QSharedPointer<TextMessage> assemble(const char *data, int len);
    QSharedPointer<TextMessage> assemble();
    void reset();

};

#endif // TESTMSGASSEMBLER_H
