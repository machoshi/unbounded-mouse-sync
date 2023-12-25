#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "windows.h"
#include <QObject>
#include <QTimer>
#include <QMap>
#include <QStandardPaths>
#include <QProcess>
#include <QString>

class FileManager: public QObject
{
    Q_OBJECT
private:
    QTimer *timer;
    QMap<qint64, QString> pidToUrlMap;
    QMap<QProcess*, qint64> paddrToPidMap;
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

private slots:
    void onTimeout();

public:
    FileManager();
    ~FileManager();
    bool sent;

public slots:
    void getUrlFromFileDialog();
    void openFile(QString fileName);

signals:
    void sendFile(QString path);
};

#endif // FILEMANAGER_H
