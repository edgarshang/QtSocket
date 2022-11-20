#include "MainWin.h"

MainWin::MainWin(QWidget *parent)
    : QWidget(parent)
{
    initMsgGrpBx();
    initInputGrpBx();

    vMainLayout.setSpacing(10);
    vMainLayout.addWidget(&msgGrapBx);
    vMainLayout.addWidget(&inputGrpBx);

    setWindowTitle("ChatRoom");
    setLayout(&vMainLayout);
    setMinimumSize(550,400);
    resize(550, 400);
}

void MainWin::initMsgGrpBx()
{

}
void MainWin::initInputGrpBx()
{

}

MainWin::~MainWin()
{

}
