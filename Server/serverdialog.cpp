#include "serverdialog.h"
#include "./ui_serverdialog.h"
#include <QMessageBox>

ServerDialog::ServerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ServerDialog)
{
    ui->setupUi(this);

    connect(&tcpServer,&QTcpServer::newConnection,this,&ServerDialog::onNewConnection);

    connect(&timer,&QTimer::timeout,this,&ServerDialog::onTimeout);
}

ServerDialog::~ServerDialog()
{
    delete ui;
}

//创建服务器
void ServerDialog::on_createBtn_clicked()
{
    //获取服务器端口：
    port = ui->portEdit->text().toShort();
    if(tcpServer.listen(QHostAddress::Any,port)){
        QMessageBox::information(this,"提示","服务器创建成功！","确定");
        ui->createBtn->setEnabled(false);
        ui->portEdit->setEnabled(false);
        timer.start(3000);
    }
    else{
        QMessageBox::warning(this,"警告","服务器创建失败!","确定");
    }

}

//相应客户端连接请求，完成三次握手，即保存客户端的通信套接字
void ServerDialog::onNewConnection()
{
    QTcpSocket* tcpClient = tcpServer.nextPendingConnection();
    tcpClientList.push_back(tcpClient);//保存获取的套接字
    connect(tcpClient,&QTcpSocket::readyRead,this,&ServerDialog::onReadyRead);
}

//接受客户端聊天消息：一旦有客户端向服务器发送消息，通信套接字就会发送信号readyRead()，由此槽函数处理
void ServerDialog::onReadyRead()
{
    //由于所有客户端的套接字都放在同一个容器中，因此需要先辨别具体哪一个客户端发送了消息
    QList<QTcpSocket*>::Iterator it;
    for(it=tcpClientList.begin();it!=tcpClientList.end();it++){
        if((*it)->bytesAvailable()){
            QByteArray buff = (*it)->readAll();//读取消息内容。如果数据太大可能导致失败。建议不超过512字节。
            //显示聊天消息
            ui->listWidget->addItem(buff);
            ui->listWidget->scrollToBottom();
            //转发
            sendMessage(buff);
        }

    }
}

//转发消息：先将收到的消息保存到buff，在将此buff发送
void ServerDialog::sendMessage(const QByteArray& buff)
{
    QList<QTcpSocket*>::Iterator it;
    for(it=tcpClientList.begin();it!=tcpClientList.end();it++){
        (*it)->write(buff);//转发给所有客户端
    }
}

void ServerDialog::onTimeout(){
    QList<QTcpSocket*>::iterator it;
    for(it=tcpClientList.begin();it!=tcpClientList.end();it++){
        if((*it)->state()==QAbstractSocket::UnconnectedState){
            tcpClientList.erase(it);
        }
    }
}
