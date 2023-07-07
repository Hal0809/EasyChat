#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QHostAddress>

QT_BEGIN_NAMESPACE
namespace Ui { class ClientDialog; }
QT_END_NAMESPACE

class ClientDialog : public QDialog
{
    Q_OBJECT

public:
    ClientDialog(QWidget *parent = nullptr);
    ~ClientDialog();

private slots:
    void on_sendBtn_clicked();

    void on_connectBtn_clicked();

    void onConnected();

    void onDisconnected();

    void onReadyRead();

    void onError();

private:
    Ui::ClientDialog *ui;
    bool status;//标记当前客户端在线状态
    QTcpSocket tcpSocket;
    QHostAddress serverIP;
    quint16 serverPort;
    QString name;

};
#endif // CLIENTDIALOG_H
