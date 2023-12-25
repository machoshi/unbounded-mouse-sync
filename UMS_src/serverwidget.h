#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QTimer>
#include "common.h"

QT_BEGIN_NAMESPACE
namespace Ui { class serverWidget; }
QT_END_NAMESPACE

class serverWidget : public QWidget
{
    Q_OBJECT

public:
    serverWidget(QWidget *parent = nullptr);
    ~serverWidget();

    void sendData();
    void sendMouseInfo();

    void sending(QString path);
    void sendMouseInfoTrigger(MouseInput input);


private:
    Ui::serverWidget *ui;
    QTcpServer *serverFile;
    QTcpSocket *socketFile;
    QTcpServer *serverMouse;
    QTcpSocket *socketMouse;
    QFile file;
    QString fileName;
    qint64 fileSize;
    qint64 sendSize;
    QTimer timer;
    QTimer mouseInfoTimer;
    MouseInput input;
};
#endif // SERVERWIDGET_H
