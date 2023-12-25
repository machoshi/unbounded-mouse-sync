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
    openFileDialogButton->move(407, 435);
    openFileDialogButton->resize(80, 24);
    mouseButton = new QPushButton("Start Unbounded Mouse",this);
    mouseButton->move(10,174);
    mouseButton->resize(235,24);

    mymouse = new Mousewb(50,false,server,client);


    // 设置信号槽
    connect(openFileDialogButton, &QPushButton::clicked, fileManager, &FileManager::getUrlFromFileDialog);
    connect(fileManager, &FileManager::sendFile, server, &serverWidget::sending);
    connect(client, &clientWidget::openFile, fileManager, &FileManager::openFile);
    connect(client,&clientWidget::recvMouseInfo,mymouse,&Mousewb::on_recvData);
    connect(mouseButton,&QPushButton::clicked,mymouse,&Mousewb::initMouse);
    connect(GlobalMouseEvent::getInstance(),&GlobalMouseEvent::mouseEvent,mymouse,&Mousewb::on_mouseEvent);
    connect(mymouse->MouseTimer,&QTimer::timeout,mymouse,&Mousewb::checkMouse);
    connect(client, &clientWidget::changeScale, fileManager, &FileManager::setScale);
    connect(client, &clientWidget::changeMainDevice, this, &MainWindow::setFlag);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete fileManager;
    delete server;
    delete client;
    delete openFileDialogButton;
}

void MainWindow::setFlag(bool _flag){
    mainDeviceFlag = _flag;
    mymouse->is_main = mainDeviceFlag;
    qDebug() << mainDeviceFlag;
}
