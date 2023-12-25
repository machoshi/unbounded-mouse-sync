#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
#include <QProcess>
#include <QDateTime>
#include <QCheckBox>

clientWidget::clientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::clientWidget)
{
    ui->setupUi(this);
    setWindowTitle("client");
    socketFile = new QTcpSocket(this);
    socketMouse = new QTcpSocket(this);
    isHead = true;
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    connect(socketFile, &QTcpSocket::readyRead,
            [=]()
            {
                QByteArray buf = socketFile->readAll();
                if(isHead)
                {
                    qDebug() << "head received: " << QString(buf);
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
                    qDebug() << "receiving: " << fileNameClient << "file size: " << fileSize;
                    bool isOk = file.open(QIODevice::WriteOnly);
                    if(!isOk)
                    {
                        qDebug() << "Fail to open the file in WriteOnly mod";
                    }
                } else {
                    qint64 len = file.write(buf);
                    recvSize += len;
                    qDebug() << "received size: " << recvSize;
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

    connect(socketMouse, &QTcpSocket::readyRead,
            [=]()
            {
                QByteArray buf = socketMouse->readAll();

                QDataStream dataStream(buf);
                dataStream.readRawData(reinterpret_cast<char*>(&mouseInfo), sizeof(MouseInput));
                emit recvMouseInfo(mouseInfo);

            }
            );
}

clientWidget::~clientWidget()
{
    delete ui;
}

void clientWidget::on_buttonConnect_released()
{
    QString ip = ui->lineEditIP->text();
    socketFile->abort();
    socketFile->connectToHost(QHostAddress(ip), 8000);
    socketMouse->abort();
    socketMouse->connectToHost(QHostAddress(ip), 8001);
}

void clientWidget::on_buttonDisconnect_released()
{
    socketFile->disconnectFromHost();
    socketFile->close();
    socketMouse->disconnectFromHost();
    socketMouse->close();
}





void clientWidget::on_setScaleButton_released()
{
    double scale = ui->lineEdit->text().toDouble();
    emit changeScale(scale);
}


void clientWidget::on_checkBox_2_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked){
        emit changeMainDevice(true);
    } else {
        emit changeMainDevice(false);
    }
}

