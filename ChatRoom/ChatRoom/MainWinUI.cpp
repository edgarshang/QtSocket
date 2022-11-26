#include "MainWinUI.h"
#include <QHBoxLayout>
#include <QGridLayout>


MainWin::MainWin(QWidget *parent)
    : QWidget(parent), loginDlg(this), m_level("")
{
    initMember();
    initMsgGrpBx();
    initInputGrpBx();
    initListWidgetMenu();
    connectSlots();

    vMainLayout.setSpacing(10);
    vMainLayout.addWidget(&msgGrapBx);
    vMainLayout.addWidget(&inputGrpBx);

    setWindowTitle("ChatRoom");
    setLayout(&vMainLayout);
    setMinimumSize(550,400);
    resize(550, 400);
}

void MainWin::connectSlots()
{
    connect(&sendBtn,SIGNAL(clicked(bool)), this, SLOT(sendBtnClicked()));
    connect(&logInOutBtn, SIGNAL(clicked(bool)), this, SLOT(logInOutBtnClicked()));
    connect(&listWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(listWidgetContextMenu(QPoint)));
}

void MainWin::initMsgGrpBx()
{
    QHBoxLayout *hbl = new QHBoxLayout();

    hbl->setContentsMargins(2,5,2,2);
    hbl->addWidget(&msgEditor, 7);

    hbl->addWidget(&listWidget, 3);
    msgEditor.setReadOnly(true);
    msgEditor.setFocusPolicy(Qt::NoFocus);
    listWidget.setFocusPolicy(Qt::NoFocus);
    listWidget.setContextMenuPolicy(Qt::CustomContextMenu);
    msgGrapBx.setLayout(hbl);
    msgGrapBx.setTitle("聊天消息");
}
void MainWin::initInputGrpBx()
{
    QGridLayout *gl = new QGridLayout();

    gl->setSpacing(10);
    gl->addWidget(&inputEdit, 0, 0, 1, 5);
    gl->addWidget(&statusLbl, 1, 0, 1, 3);
    gl->addWidget(&logInOutBtn, 1, 3);
    gl->addWidget(&sendBtn, 1, 4);

    inputEdit.setFixedHeight(100);
    inputEdit.setEnabled(false);
    statusLbl.setText("状态：未登录");
    logInOutBtn.setFixedHeight(50);
    logInOutBtn.setText("登陆");
    sendBtn.setFixedHeight(50);
    sendBtn.setText("发送");
    sendBtn.setEnabled(false);

    inputGrpBx.setFixedHeight(200);
    inputGrpBx.setLayout(gl);
    inputGrpBx.setTitle("用户名");
}

void MainWin::initListWidgetMenu()
{
    QAction* act = NULL;

    act = listWidgetMenu.addAction("禁言", this, SLOT(listWidgetMenuClicked()));
    act->setObjectName("silent");

    act = listWidgetMenu.addAction("恢复", this, SLOT(listWidgetMenuClicked()));
    act->setObjectName("recover");

    listWidgetMenu.addSeparator();
    act = listWidgetMenu.addAction("封账号", this, SLOT(listWidgetMenuClicked()));
    act->setObjectName("kick");
}

void MainWin::setCtrlEnable(bool enabled)
{

    inputEdit.setEnabled(enabled);
    statusLbl.setText(enabled ? "状态：连接成功" : "状态：未登录");
    logInOutBtn.setText(enabled ? "退出" : "登陆");
    sendBtn.setEnabled(enabled);
    if( !enabled )
    {
        msgEditor.clear();
        listWidget.clear();
        inputEdit.clear();
    }else
    {
        inputEdit.setFocus();
    }
}

MainWin::~MainWin()
{
    m_client.close();
}
