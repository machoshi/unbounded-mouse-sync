#ifndef MOUSEWB_H
#define MOUSEWB_H
#include"globalmouseevent.h"
#include <QObject>
#include<QObject>
#include<QScreen>
#include<QCursor>
#include<QTimer>
#include<QRect>
#include<QGuiApplication>
#include<QDebug>
#include"myshade.h"
#include"windows.h"
#include "serverwidget.h"
#include "clientwidget.h"
#include"common.h"
class Mousewb : public QObject
{
    Q_OBJECT
public:
    explicit Mousewb(int _fps = 10, bool _is_main = true,serverWidget* _server=nullptr, clientWidget* _client=nullptr);//实例鼠标
    QTimer *MouseTimer; //计时器
private:
    /*void worker_main();//工作函数
    bool DataSender();//传输鼠标的位置信息等
    bool DataReceiver();//接收鼠标的位置信息等*/
    void worker_vice(INPUT input, QPoint point);//副电脑端工作函数
    void GetScreen(QPoint&center);
    QRect GetScreenInfo();

    void initShade(QRect screen);//初始化遮罩

    bool TCPconnect();//建立两台电脑的端到端连接
public slots:
    void TimeoutSlot();
    void on_mouseEvent(INPUT input,QPoint point);
    void on_recvData(MouseInput input);
private:
    QPoint lastpos;
    QPoint curpos;
    serverWidget* server;
    clientWidget* client;

    int fps; //模拟鼠标的刷新频率
    bool Cur_pc;//为1在主电脑端，为0在副电脑端
    bool is_main;//判断是否为主电脑上进程
//    QRect Cur_screen;
    QRect* Remote_screen =nullptr;
    MyShade* shade;//屏幕遮罩，隐藏鼠标
    //bool eventFilter(QObject*obj,QEvent *event) override;
signals:
    // Added
    void readyServer();
    void connectClient();
};

#endif // MOUSEWB_H
