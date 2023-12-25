#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QTimer>

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

public slots:
    void sending(QString path);

private:
    Ui::serverWidget *ui;
    QTcpServer *server;
    QTcpSocket *socket;
    QFile file;
    QString fileName;
    qint64 fileSize;
    qint64 sendSize;
    QTimer timer;
};
#endif // SERVERWIDGET_H
