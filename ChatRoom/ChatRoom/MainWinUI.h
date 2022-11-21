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

class MainWin : public QWidget
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

    void initMsgGrpBx();
    void initInputGrpBx();
    void connectSlots();
private slots:
    void sendBtnClicked();
    void logInOutBtnClicked();

public:
    MainWin(QWidget *parent = 0);
    ~MainWin();
};

#endif // MAINWINUI_H
