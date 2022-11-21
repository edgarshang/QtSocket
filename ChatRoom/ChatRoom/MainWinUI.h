#ifndef MAINWINUI_H
#define MAINWINUI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "QLoginDialog.h"
#include "../../Clientdemo.h"
#include "../../txtmeghandler.h"

class MainWin : public QWidget, public TxtMsgHandler
{
    Q_OBJECT
    QVBoxLayout vMainLayout;
    QGroupBox msgGrapBx;
    QGroupBox inputGrpBx;
    QPlainTextEdit msgEditor;
    QLineEdit inputEdit;
    QPushButton logInOutBtn;
    QPushButton sendBtn;
    QLabel statusLbl;
    QLoginDialog loginDlg;

    ClientDemo m_client;
    void initMember();
    void initMsgGrpBx();
    void initInputGrpBx();
    void connectSlots();

    void setCtrlEnable(bool enabled);
private slots:
    void sendBtnClicked();
    void logInOutBtnClicked();

public:
    MainWin(QWidget *parent = 0);
    void handle(QTcpSocket& obj, TextMessage& message);
    ~MainWin();
};

#endif // MAINWINUI_H
