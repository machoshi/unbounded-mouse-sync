#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化文件托管器
    fileManager = new FileManager();

    // 初始化server和client
    server = new serverWidget(this);
    client = new clientWidget(this);

    // 初始化“打开文件”按钮
    openFileDialogButton = new QPushButton("Open File", this);
    openFileDialogButton->move(200, 300);
    openFileDialogButton->resize(100, 50);

    // 设置信号槽
    connect(openFileDialogButton, &QPushButton::clicked, fileManager, &FileManager::getUrlFromFileDialog);
    connect(fileManager, &FileManager::sendFile, server, &serverWidget::sending);
    connect(client, &clientWidget::openFile, fileManager, &FileManager::openFile);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete fileManager;
    delete server;
    delete client;
    delete openFileDialogButton;
}

