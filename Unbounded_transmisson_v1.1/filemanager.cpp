#include "filemanager.h"
#include <QMimeDatabase>
#include <QMimeType>
#include <QProcess>
#include <QPoint>
#include <QCursor>
#include <QDebug>
#include <QFileDialog>

FileManager::FileManager()
{
    // 初始化timer
    timer = new QTimer();
    timer->setInterval(200);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->start();
}

FileManager::~FileManager()
{
    delete timer;
}

void FileManager::getUrlFromFileDialog()
{
    // 使用“打开文件”对话框获取文件地址
    QString fileName;
    fileName = QFileDialog::getOpenFileName(NULL, tr("打开文件"), desktopPath, tr("Images or text files(*.png *.jpg *.txt)"));
    if (fileName.isEmpty())
    {
        return;
    }
    fileName = QDir::toNativeSeparators(fileName);

    // 打开文件
    openFile(fileName);
}

void FileManager::openFile(QString fileName)
{
    // 判断文件类型
    QString openWith;
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(fileName);
    if (mime.name().startsWith("image/")) {
        // qDebug()<<"Image";
        openWith = "mspaint";
    }
    if (mime.name().startsWith("text/")) {
        // qDebug()<<"Text File";
        openWith = "notepad";
    }

    // 打开文件，并获取对应进程的pid
    QProcess *proc = new QProcess();
    QStringList lst;
    lst<<fileName;
    proc->start(openWith, lst);

    qint64 procId;
    procId = proc->processId();

    // 记录进程地址与进程号的对应关系，当用户结束进程时删除该进程的相关信息
    paddrToPidMap.insert(proc, procId);
    QObject::connect(proc, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                     [proc, this] (int exitCode, QProcess::ExitStatus exitStatus) {
                         qint64 procId = paddrToPidMap.value(proc);
                         pidToUrlMap.remove(procId);
                         paddrToPidMap.remove(proc);
                         proc->deleteLater();
                         qDebug() << procId << "terminated";
                     });

    // 记录pid与url的对应关系
    pidToUrlMap.insert(procId, fileName);
}

void FileManager::onTimeout()
{
    timer->stop();

    // 获取鼠标的位置
    QPoint mousePos = QCursor::pos();

    double scale = 1.25;
    mousePos.rx() *= scale;
    mousePos.ry() *= scale;

    // 获取鼠标所在位置的窗口句柄
    HWND hwnd = ::WindowFromPoint(POINT{mousePos.x(), mousePos.y()});

    // 获取窗口句柄的pid
    DWORD dwPid;
    GetWindowThreadProcessId(hwnd, &dwPid);
    qint64 procId = (qint64)dwPid;

    // 获取pid对应的url
    QString fileName;
    if (pidToUrlMap.contains(procId))
    {
        fileName = pidToUrlMap.value(procId);
    }

    // 如果鼠标到达屏幕最左侧，传输文件，并关闭文件对应的窗口
    if(mousePos.x() == 0 && !fileName.isEmpty()){
        sent = true;
        emit sendFile(fileName);
        HANDLE hProcess=OpenProcess(PROCESS_TERMINATE, FALSE, procId);
        TerminateProcess(hProcess,0);
    }

    timer->start();
}


