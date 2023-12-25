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
    serverFile = new QTcpServer(this);
    serverFile->listen(QHostAddress::Any, 8000);

    connect(serverFile, &QTcpServer::newConnection,
            [=]()
            {
                socketFile = serverFile->nextPendingConnection();
                QString ip = socketFile->peerAddress().toString();
                quint16 port = socketFile->peerPort();
                QString str = QString("[%1, %2] successfully connected").arg(ip).arg(port);
                qDebug() << str;
            }
            );

    connect(&timer, &QTimer::timeout,
            [=]()
            {
                timer.stop();
                sendData();
            }
            );

    serverMouse = new QTcpServer(this);
    serverMouse->listen(QHostAddress::Any, 8001);
    connect(serverMouse, &QTcpServer::newConnection,
            [=]()
            {
                socketMouse = serverMouse->nextPendingConnection();
                QString ip = socketMouse->peerAddress().toString();
                quint16 port = socketMouse->peerPort();
                QString str = QString("[%1, %2] successfully connected").arg(ip).arg(port);

                qDebug() << str;
            }
            );


    connect(&mouseInfoTimer, &QTimer::timeout,
            [=]()
            {
                mouseInfoTimer.stop();
                sendMouseInfo();
            }
            );
}

serverWidget::~serverWidget()
{
    delete ui;
}

void serverWidget::sendMouseInfo()
{
    QByteArray data;
    QDataStream dataStream(&data, QIODevice::WriteOnly);
    dataStream.writeRawData(reinterpret_cast<char*>(&input), sizeof(MouseInput));
    socketMouse->write(data);
}

void serverWidget::sendData()
{
    qint64 len = 0;
    do
    {
        char buf[4 * 1024] = {0};
        len = file.read(buf, sizeof(buf));
        len = socketFile->write(buf, len);
        sendSize += len;
    } while(len > 0);

    if(fileSize == sendSize)
    {
//        ui->textEdit->append("sending process finished");
        file.close();
        qDebug() << "Sending complete. Sent size: " << sendSize;
    }
}

void serverWidget::sendMouseInfoTrigger(MouseInput _input){
    input = _input;
    mouseInfoTimer.start(20);

}

void serverWidget::sending(QString path)
{
    qDebug() << "sending: " << path;
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
    qint64 len = socketFile->write(head.toUtf8());
    if(len > 0)
    {
        // 倒计时结束后开始传输，用于解决tcp粘包问题
        timer.start(200);
    } else {
        qDebug() << "Fail to send the head info.";
        file.close();
    }
}



