#include "clientdialog.h"
#include "./ui_clientdialog.h"

#include <QMessageBox>

ClientDialog::ClientDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ClientDialog)
{
    ui->setupUi(this);
    status = false;
    connect(&tcpSocket,&QTcpSocket::connected,this,&ClientDialog::onConnected);
    connect(&tcpSocket,&QTcpSocket::disconnected,this,&ClientDialog::onDisconnected);
    connect(&tcpSocket,&QTcpSocket::readyRead,this,&ClientDialog::onReadyRead);
    connect(&tcpSocket,&QTcpSocket::errorOccurred,this,&ClientDialog::onError);
    connect(ui->msgEdit,&QLineEdit::textEdited,ui->sendBtn,[=](){ui->sendBtn->setEnabled(true);});
}

ClientDialog::~ClientDialog()
{
    delete ui;
}


void ClientDialog::on_sendBtn_clicked()
{
    QString msg = ui->msgEdit->text();
    msg = name + ":" + msg;
    tcpSocket.write(msg.toUtf8());
    ui->msgEdit->clear();
    ui->sendBtn->setEnabled(false);
}


void ClientDialog::on_connectBtn_clicked()
{
    if(status == false){
        //获取服务器相关信息：IP，端口，名称
        serverIP.setAddress(ui->addressEdit->text());
        serverPort = ui->portEdit->text().toShort();
        name = ui->nameEdit->text();

        //如果上述成功，则发送连接请求connected，否则发送errorConnected
        tcpSocket.connectToHost(serverIP,serverPort);
    }
    else{//当前为准备离开聊天室
        QString msg = name + " 离开了聊天室。";
        tcpSocket.write(msg.toUtf8());
        tcpSocket.disconnectFromHost();
    }
}

void ClientDialog::onConnected()
{
    status = true;
    ui->sendBtn->setEnabled(true);
    ui->addressEdit->setEnabled(false);
    ui->portEdit->setEnabled(false);
    ui->nameEdit->setEnabled(false);
    ui->connectBtn->setText("离开聊天室");

    //向服务器发送进入聊天室的消息
    QString msg = name + " 进入了聊天室";
    tcpSocket.write(msg.toUtf8());
}

void ClientDialog::onDisconnected()
{
    status = false;
    ui->sendBtn->setEnabled(false);
    ui->addressEdit->setEnabled(true);
    ui->portEdit->setEnabled(true);
    ui->nameEdit->setEnabled(true);
    ui->connectBtn->setEnabled(true);
    ui->connectBtn->setText("连接服务器");
}

void ClientDialog::onReadyRead()
{
    if(tcpSocket.bytesAvailable()){
        QByteArray buff = tcpSocket.readAll();
        //显示消息到界面
        ui->listWidget->addItem(buff);
        ui->listWidget->scrollToBottom();
    }
}

void ClientDialog::onError()
{
    //tcpSocket.errorString可以显示错误原因
    QMessageBox::critical(this,"错误！",tcpSocket.errorString(),"确定");
}
