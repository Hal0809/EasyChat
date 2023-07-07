#ifndef SERVERDIALOG_H
#define SERVERDIALOG_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class ServerDialog; }
QT_END_NAMESPACE

class ServerDialog : public QDialog
{
    Q_OBJECT

public:
    ServerDialog(QWidget *parent = nullptr);
    ~ServerDialog();

private slots:
    void on_createBtn_clicked();
    //相应客户端连接请求，即保存客户端的通信套接字
    void onNewConnection();
    //接受客户端聊天消息：一旦有客户端向服务器发送消息，通信套接字就会发送信号readyRead()，由此槽函数处理
    void onReadyRead();
    //转发消息：先将收到的消息保存到buff，在将此buff发送
    void sendMessage(const QByteArray& buff);
    //定时器的槽函数
    void onTimeout();

private:
    Ui::ServerDialog *ui;
    QTcpServer tcpServer;
    qint16 port;//端口号
    QList<QTcpSocket*> tcpClientList;//保存所有和客户端通信的套接字
    QTimer timer;//定时器，检查断开的套接字
};
#endif // SERVERDIALOG_H
