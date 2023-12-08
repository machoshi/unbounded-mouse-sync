#include "serverwidget.h"
#include "ui_serverwidget.h"
#include "filemanager.h"
#include <QHostAddress>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QTimer>

serverWidget::serverWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::serverWidget)
{
//    ui->setupUi(this);
//    setWindowTitle("Server (port : 8000)");
//    ui->buttonFile->setEnabled(false);
//    ui->buttonSend->setEnabled(false);
    server = new QTcpServer(this);
    server->listen(QHostAddress::Any, 8000);

    connect(server, &QTcpServer::newConnection,
            [=]()
            {
                socket = server->nextPendingConnection();
                QString ip = socket->peerAddress().toString();
                quint16 port = socket->peerPort();
                QString str = QString("[%1, %2] successfully connected").arg(ip).arg(port);
            }
            );

    connect(&timer, &QTimer::timeout,
            [=]()
            {
                timer.stop();
                sendData();
            }
            );
}

serverWidget::~serverWidget()
{
    delete ui;
}

void serverWidget::sendData()
{
    qint64 len = 0;
    do
    {
        char buf[4 * 1024] = {0};
        len = file.read(buf, sizeof(buf));
        len = socket->write(buf, len);
        sendSize += len;
    } while(len > 0);

    if(fileSize == sendSize)
    {
//        ui->textEdit->append("sending process finished");
        file.close();
    }
}


void serverWidget::sending(QString path)
{
    qDebug() << path;
    if(!path.isEmpty())
    {
        fileName.clear();
        QFileInfo info(path);
        fileName = info.fileName();
        fileSize = info.size();
        sendSize = 0;
        file.setFileName(path);
        bool flag = file.open(QIODevice::ReadOnly);
        if(!flag)
        {
            qDebug() << "Fail to open in ReadOnly mode.";
        }
    } else {
        qDebug() << "Fail to find the path.";
    }

    QString head = QString("%1##%2").arg(fileName).arg(fileSize);
    qint64 len = socket->write(head.toUtf8());
    if(len > 0)
    {
        // 倒计时结束后开始传输，用于解决tcp粘包问题
        timer.start(20);
    } else {
        qDebug() << "Fail to send the head info.";
        file.close();
    }
}

void serverWidget::assignInput(MouseInput updated_input)
{
    input = updated_input;
}

void serverWidget::sendingProcess()
{
    QString head = QString("sending##struct");
    qint64 len = socket->write(head.toUtf8());
    if(len > 0)
    {
        //tcp粘包问题 timer
        timer.start(20);
    } else {
        qDebug() << "Fail to send the head info.";
    }
}

void serverWidget::disconnectProcess()
{
    socket->disconnectFromHost();
    socket->close();
}


