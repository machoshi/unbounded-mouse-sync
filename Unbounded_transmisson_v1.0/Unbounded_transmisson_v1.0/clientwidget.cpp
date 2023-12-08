#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
#include <QProcess>
#include <QDateTime>

clientWidget::clientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::clientWidget)
{
    ui->setupUi(this);
    setWindowTitle("client");
    socket = new QTcpSocket(this);
    isHead = true;
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    connect(socket, &QTcpSocket::readyRead,
            [=]()
            {
                QByteArray buf = socket->readAll();
                if(isHead)
                {
                    isHead = false;

                    if(buf != "sending##struct")
                    {
                        qDebug() << "Fail to open the file in WriteOnly mod";
                    }
                } else {
                    QDataStream stream(buf);
                    stream >> info;
                    QMessageBox::information(this, "success", "sending process completed");
                    emit InputRec(info);
                }

            }
            );

    connect(socket, &QTcpSocket::readyRead,
            [=]()
            {
                QByteArray buf = socket->readAll();
                if(isHead)
                {
                    isHead = false;

                    // 创建文件的保存路径
                    qint64 timeT = QDateTime::currentSecsSinceEpoch();
                    fileSavePath = documentsPath + "\\UnboundedTransmission\\" + QString::number(timeT);

                    QDir dir;
                    if(!dir.exists(fileSavePath))
                    {
                        dir.mkpath(fileSavePath);
                    }

                    // 获取文件名与文件路径
                    fileNameServer = QString(buf).section("##", 0, 0);
                    fileName = fileNameServer.section("\\", -1, -1);
                    fileNameClient = fileSavePath + "\\" + fileName;

                    fileSize = QString(buf).section("##", 1, 1).toInt();
                    ui->progressBar->setMaximum(fileSize);
                    recvSize = 0;

                    file.setFileName(fileNameClient);
                    qDebug() << fileNameClient;
                    bool isOk = file.open(QIODevice::WriteOnly);
                    if(!isOk)
                    {
                        qDebug() << "Fail to open the file in WriteOnly mod";
                    }
                } else {
                    qint64 len = file.write(buf);
                    recvSize += len;
                    ui->progressBar->setValue(recvSize);
                    if(recvSize == fileSize)
                    {
                        isHead = true;
                        ui->progressBar->setValue(0);
                        file.close();
                        // QMessageBox::information(this, "success", "sending process completed");

                        // 文件传输完毕，发送“打开文件”信号
                        emit(openFile(fileNameClient));
                    }
                }

            }
           );
}

clientWidget::~clientWidget()
{
    delete ui;
}

void clientWidget::on_buttonConnect_released()
{
    quint16 port = ui->lineEditPort->text().toInt();
    QString ip = ui->lineEditIP->text();
    socket->connectToHost(QHostAddress(ip), port);
}

void clientWidget::on_buttonDisconnect_released()
{
    socket->disconnectFromHost();
    socket->close();
}

