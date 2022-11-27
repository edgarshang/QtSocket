#include "MainWinUI.h"
#include <QMessageBox>
#include <QDebug>

static bool ValidateUserID(QString id)
{
    bool ret = true;
    QString invalid = "~`!@#$%^&*()_-+={}[]|\\'\",.?/<>";

    for( int i = 0; i < invalid.length(); i++)
    {
        if( id.contains(invalid[i]))
        {
            ret = false;
            break;
        }
    }

    return ret;
}

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
    MapToHandler(CTRL);
}


bool MainWin::eventFilter(QObject *obj, QEvent *evt)
{
    if( (obj == &inputEdit) && (evt->type() == QEvent::KeyPress) )
    {
        QKeyEvent* ke = dynamic_cast<QKeyEvent*>(evt);

        if( ke->text() == "\r")
        {
            sendBtnClicked();

            return true;
        }
    }

    return QWidget::eventFilter(obj, evt);
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

}

void MainWin::listWidgetMenuClicked()
{
    QAction* act = dynamic_cast<QAction*>(sender());
    if( act != NULL)
    {
        const QList<QListWidgetItem*>& sl = listWidget.selectedItems();

        if( sl.length() > 0 )
        {
            QString user = sl.at(0)->text();
            QString tip = "确认对聊天成员 [ " + user + " ] 进行" + act->text() + "操作吗?";

            if( QMessageBox::question(this, "提示", tip, QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
            {
                QString data = act->objectName() + '\r' + user;
                TextMessage tm("ADMN",data);

                m_client.send(tm);
            }
        }else
        {
            QMessageBox::information(this, "提示", "请选择聊天成员!");
        }
    }
}

void MainWin::listWidgetContextMenu(const QPoint&)
{
    if( m_level == "admin" )
    {
        listWidgetMenu.exec(QCursor::pos());
    }
}

void MainWin::logInOutBtnClicked()
{
    if(!m_client.isValid())
    {
        loginDlg.setValFunc(ValidateUserID);
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
    activateWindow();
}

void MainWin::CONN_Handler(QTcpSocket& obj, TextMessage& message)
{

}
void MainWin::DSCN_Handler(QTcpSocket& obj, TextMessage& message)
{
    setCtrlEnable(false);

    inputGrpBx.setTitle("用户名");

    m_level = "";
}
void MainWin::LIOK_Handler(QTcpSocket& obj, TextMessage& message)
{
    QStringList rl = message.data().split("\r", QString::SkipEmptyParts);
    QString id = rl[0];
    QString status = rl[1];

    m_level = rl[2];

    if( status == "silent" )
    {
        setCtrlEnable("true");
        inputGrpBx.setTitle(id);
        inputEdit.setEnabled(false);
        sendBtn.setEnabled(false);
    }else if( status == "kick")
    {
        m_client.close();
        QMessageBox::information(this, "提示", "账号 [" + id + "] 被禁止登录");
    }else
    {
        setCtrlEnable(true);
        inputGrpBx.setTitle(id);
    }
//    setCtrlEnable(true);
//    inputGrpBx.setTitle(message.data());
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

void MainWin::CTRL_Handler(QTcpSocket& obj, TextMessage& message)
{
    if( message.data() == "silent")
    {
        QMessageBox::information(this, "提示", "你已经被管理员禁言！");
        inputEdit.clear();
        inputEdit.setEnabled(false);
        sendBtn.setEnabled(false);
    }else if( message.data() == "recover")
    {
        QMessageBox::information(this, "提示", "管理员恢复你的聊天权限！");
        inputEdit.setEnabled(true);
        sendBtn.setEnabled(true);
    }else if( message.data() == "kick")
    {
//        QMessageBox::information(this, "提示", "账号 [" , + inputGrpBx.title() + "] 被禁止登录聊天室");
        m_client.close();
    }
}

