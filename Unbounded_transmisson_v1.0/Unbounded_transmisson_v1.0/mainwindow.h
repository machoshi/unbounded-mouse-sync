#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "filemanager.h"
#include "clientwidget.h"
#include "serverwidget.h"
#include"mousewb.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QPushButton *openFileDialogButton;
    FileManager *fileManager;
    serverWidget *server;
    clientWidget *client;
    Mousewb *mymouse;
};
#endif // MAINWINDOW_H
