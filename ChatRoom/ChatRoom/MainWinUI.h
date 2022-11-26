#ifndef MAINWINUI_H
#define MAINWINUI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMap>
#include "QLoginDialog.h"
#include "../../Clientdemo.h"
#include "../../txtmeghandler.h"

class MainWin : public QWidget, public TxtMsgHandler
{
    Q_OBJECT

    typedef void(MainWin::*MSGHandler)(QTcpSocket&, TextMessage&);

    QVBoxLayout vMainLayout;
    QGroupBox msgGrapBx;
    QListWidget listWidget;
    QGroupBox inputGrpBx;
    QPlainTextEdit msgEditor;
    QLineEdit inputEdit;
    QPushButton logInOutBtn;
    QPushButton sendBtn;
    QLabel statusLbl;
    QLoginDialog loginDlg;

    ClientDemo m_client;

    QMap<QString, MSGHandler> m_handlerMap;
    void initMember();
    void initMsgGrpBx();
    void initInputGrpBx();
    void connectSlots();

    void setCtrlEnable(bool enabled);
    QString getCheckedUserId();

    void CONN_Handler(QTcpSocket& obj, TextMessage& message);
    void DSCN_Handler(QTcpSocket& obj, TextMessage& message);
    void LIOK_Handler(QTcpSocket& obj, TextMessage& message);
    void LIER_Handler(QTcpSocket& obj, TextMessage& message);
    void MSGA_Handler(QTcpSocket& obj, TextMessage& message);
    void USER_Handler(QTcpSocket& obj, TextMessage& message);
private slots:
    void sendBtnClicked();
    void logInOutBtnClicked();

public:
    MainWin(QWidget *parent = 0);
    void handle(QTcpSocket& obj, TextMessage& message);
    ~MainWin();
};

#endif // MAINWINUI_H
