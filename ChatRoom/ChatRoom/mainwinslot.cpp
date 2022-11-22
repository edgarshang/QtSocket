#include "MainWinUI.h"
#include <QMessageBox>
#include <QDebug>

void MainWin::initMember()
{
    m_client.setHandler(this);
}

void MainWin::sendBtnClicked()
{

}
void MainWin::logInOutBtnClicked()
{
    if(!m_client.isValid())
    {
        if(loginDlg.exec() == QDialog::Accepted)
        {
            QString usr = loginDlg.getUser().trimmed();
            QString pwd = loginDlg.getPwd();

            if( m_client.connectTo("127.0.0.1", 8890))
            {
                TextMessage tm("LGIN", usr + '\r' + pwd);
                m_client.send(tm);
            }
            else
            {
                QMessageBox::critical(this, "失败", "连接不到远程服务端");
            }
        }
    }
    else
    {
        m_client.close();
    }

}

void MainWin::handle(QTcpSocket &obj, TextMessage &message)
{
    qDebug() << message.type();
    qDebug() << message.data();

    if(message.type() == "CONN")
    {

    }
    else if(message.type() == "DSCN")
    {
        setCtrlEnable(false);

        inputGrpBx.setTitle("用户名");
    }else if(message.type() == "LIOK")
    {
        setCtrlEnable(true);
        inputGrpBx.setTitle(message.data());
    }else if(message.type() == "LIER")
    {
        QMessageBox::critical(this, "错误","身份验证失败!");
        m_client.close();
    }
}
