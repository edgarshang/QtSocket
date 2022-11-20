#ifndef MAINWIN_H
#define MAINWIN_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

class MainWin : public QWidget
{
    Q_OBJECT
    QVBoxLayout vMainLayout;
    QGroupBox msgGrapBx;
    QGroupBox inputGrpBx;

    void initMsgGrpBx();
    void initInputGrpBx();
public:
    MainWin(QWidget *parent = 0);
    ~MainWin();
};

#endif // MAINWIN_H
