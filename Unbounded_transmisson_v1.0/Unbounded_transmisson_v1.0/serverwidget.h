#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QTimer>
#include"windows.h"
#include"common.h"
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
    void assignInput(MouseInput updated_input);
public slots:
    void sending(QString path);
    void sendingProcess();
    void disconnectProcess();

private:
    Ui::serverWidget *ui;
    QTcpServer *server;
    QTcpSocket *socket;
    QFile file;
    QString fileName;
    qint64 fileSize;
    qint64 sendSize;
    QTimer timer;
    MouseInput input;
};
#endif // SERVERWIDGET_H
