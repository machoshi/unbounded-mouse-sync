#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>
#include <QStandardPaths>
#include <QDir>

QT_BEGIN_NAMESPACE
namespace Ui { class clientWidget; }
QT_END_NAMESPACE

class clientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit clientWidget(QWidget *parent = nullptr);
    ~clientWidget();

private slots:
    void on_buttonConnect_released();
    void on_buttonDisconnect_released();

private:
    Ui::clientWidget *ui;
    QTcpSocket *socket;
    QFile file;
    qint64 fileSize;
    qint64 recvSize;
    bool isHead;

    QString documentsPath = QDir::toNativeSeparators(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)); // “我的文档”路径
    QString fileSavePath; // “我的文档”路径下的文件保存路径
    QString fileNameServer; // Server上的文件路径
    QString fileNameClient; // Client上的文件路径
    QString fileName; // 文件名

signals:
    void openFile(QString fileName);
};

#endif // CLIENTWIDGET_H
