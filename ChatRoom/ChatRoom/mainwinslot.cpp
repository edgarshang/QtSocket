#include "MainWinUI.h"
#include <QMessageBox>
#include <QDebug>

void MainWin::initMember()
{
    m_client.setHandler(this);

    #define MapToHandler(MSG) m_handlerMap.insert(#MSG, MSG##_Handler)

    MapToHandler(CONN);
    MapToHandler(DSCN);
    MapToHandler(LIOK);
    MapToHandler(LIER);
    MapToHandler(MSGA);
    MapToHandler(USER);

}

QString MainWin::getCheckedUserId()
{
    QString ret = "";
    for(int i = 0; i < listWidget.count(); i++)
    {
        QListWidgetItem *item = listWidget.item(i);
        if( item->checkState() == Qt::Checked )
        {
            ret += item->text() + '\r';
        }
    }

    return ret;
}

void MainWin::sendBtnClicked()
{
    QString input = inputEdit.text().trimmed();
    bool ok;
    if( input != "")
    {
        QString self = inputGrpBx.title();
        QString text = self + ":\n" + "   " + input + "\n";
        QString uid = getCheckedUserId();

        if( uid == "" )
        {
            TextMessage tm("MSGA", text);
            ok = m_client.send(tm);
        }else
        {
            QString sid = (uid.indexOf(self) >= 0) ? uid : (uid + self + '\r');
            TextMessage tm("MSGP", sid + text);

            ok = m_client.send(tm);
        }

        if( ok )
        {
            inputEdit.clear();
        }
    }

//    QString text = inputGrpBx.title() + ":\n" + "   " + inputEdit.text() + "\n";
//    TextMessage tm("MSGA", text);

//    if( m_client.send(tm) )
//    {
//        inputEdit.clear();
//    }
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
    if (m_handlerMap.contains(message.type()) )
    {
        MSGHandler handler = m_handlerMap.value(message.type());
        (this->*handler)(obj, message);
    }
}

void MainWin::MSGA_Handler(QTcpSocket& obj, TextMessage& message)
{
    msgEditor.appendPlainText(message.data());
}

void MainWin::CONN_Handler(QTcpSocket& obj, TextMessage& message)
{

}
void MainWin::DSCN_Handler(QTcpSocket& obj, TextMessage& message)
{
    setCtrlEnable(false);

    inputGrpBx.setTitle("用户名");
}
void MainWin::LIOK_Handler(QTcpSocket& obj, TextMessage& message)
{
    setCtrlEnable(true);
    inputGrpBx.setTitle(message.data());
}

void MainWin::LIER_Handler(QTcpSocket& obj, TextMessage& message)
{
    QMessageBox::critical(this, "错误","身份验证失败!");
    m_client.close();
}

void MainWin::USER_Handler(QTcpSocket& obj, TextMessage& message)
{
    QStringList users = message.data().split("\r", QString::SkipEmptyParts);
    QStringList checked = getCheckedUserId().split("\r", QString::SkipEmptyParts);

    listWidget.clear();

    for(int i = 0; i < users.length(); i++)
    {
        QListWidgetItem* item = new QListWidgetItem();

        if( item != NULL)
        {
            item->setText(users[i]);
            item->setCheckState(Qt::Unchecked);

            listWidget.addItem(item);
        }
    }

    for( int i = 0; i < listWidget.count(); i++ )
    {
        QListWidgetItem *item = listWidget.item(i);
        for( int j = 0; j < checked.length(); j++ )
        {
            if( checked.at(j) == item->text())
            {
                item->setCheckState(Qt::Checked);
            }
        }
    }
}

