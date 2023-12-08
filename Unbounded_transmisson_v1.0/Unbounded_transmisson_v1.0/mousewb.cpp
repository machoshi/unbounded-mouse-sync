#include "mousewb.h"
/*
    执行流程：
        1、两台电脑各自开启无界鼠标程序，一台选择主电脑端模式，一台选择副电脑端模式
        2、输入相应的ip地址，建立tcp连接
        3、建立连接成功，启用无界鼠标功能
        4、若检测到连接断开或关闭窗口或强制措施ctrl+F11关闭无界鼠标

*/
Mousewb::Mousewb(int _fps,bool _is_main.serverWidget* _server, clientWidget* _client):server(_server),client(_client),fps(_fps),Cur_pc(1),is_main(_is_main){
    if(TCPconnect()){//执行屏幕数据交换
        qDebug()<<"TCP connect Success";
    }
    else{
        qDebug()<<"TCP connect Fail";
        return;
    }
    if(is_main){    //主电脑端进程
        MouseTimer = new QTimer;
        MouseTimer->start(5000);
        qDebug()<<"开始监听鼠标";
        GlobalMouseEvent::installMouseEvent();
    }
    else{//副电脑端进程

    }
}

/*************************************槽函数******************************************************/
void Mousewb::on_mouseEvent(INPUT input,QPoint point){
    MouseInput mouse_input;
    mouse_input.point = QPoint(point.x()*shade->get_scale(),point.y()*shade->get_scale());

    if(is_main){    //只允许Server端执行该函数 以下信号传输均向Client端传输
        if(Cur_pc){ //鼠标在主电脑端
            if(point.x()==0){   //穿越边界
                /*传输input结构体*/
                mouse_input.flag = 1;
                mouse_input.Cur_pc = 0;
                mouse_input.input = input;
                server->assignInput(mouse_input);
                emit readyServer();
                /*传输Cur_pc更改信号*/

                /*修改本地鼠标位置*/
                Cur_pc = 0;//鼠标移到副电脑端
                shade->show();
                QCursor::setPos(shade->size().width(),point.y()/shade->get_scale());//移动鼠标到遮罩边界

                return;
            }
        }
        else{   //鼠标在副电脑端
            if(point.x()>shade->width()){//穿越边界
                /*传输cur_pc更改信号*/
                mouse_input.flag = 1;
                mouse_input.Cur_pc = 1;
                mouse_input.input = input;
                server->assignInput(mouse_input);

                /*修改本地鼠标位置*/
                Cur_pc = 1;//鼠标移动到主电脑端
                shade->hide();
                QCursor::setPos(point.x()-shade->size().width(),point.y()*shade->get_scale());//移动鼠标到电脑边界

                return;
            }
            if(point.y()>=shade->height()){//鼠标到达底部边界
                QCursor::setPos(point.x(),shade->height());
                point.setY(shade->height());
                /*传输input结构体*/
                mouse_input.input = input;
                mouse_input.flag = 2;
                server->assignInput(mouse_input);
                emit readyServer();
                return;
            }
            //未到达任何边界

            /*传输input结构体*/
            mouse_input.input = input;
            mouse_input.flag = 2;
            server->assignInput(mouse_input);
            emit readyServer();
            return;

        }
    }
}


void Mousewb::TimeoutSlot(){
    GlobalMouseEvent::removeMouseEvent();
    qDebug()<<"停止监听鼠标";
}

void Mousewb::on_recvData(MouseInput input){
    /*该函数与Client端以及Server端的接收信号连接*/
    //flag = 0，代表为建立连接，对应data类型为QRect
    //flag = 1，代表Cur_pr更改信号，对应的data类型为bool
    //flag = 2，代表input结构体传输，对应的data类型为INPUT和point的结构体
    //flag = 3，代表文件传输，对应的data类型为文件
    //flag = 4，代表拖拽信号，对应的data类型为偏移量和文件名
    if(is_main){    //Server端只有第一次建立连接需要接收数据
        if(input.flag==0){
            *Remote_screen = input.Screen;
        }
    }
    else{
        switch (input.flag) {
        case 0:
            *Remote_screen = input.Screen;
            break;
        case 1:
            Cur_pc = input.Cur_pc;
            QCursor::setPos(input.point);
            break;
        case 2:
            if(!is_main&&!Cur_pc){
                QCursor::setPos(input.point);
                SendInput(1,&input.input,sizeof(INPUT));
            }
            break;
        case 3:
            //文件传输
            break;
        case 4:
            //根据文件名索引对应窗口，并按照偏移量进行平移
        default:
            break;
        }
    }
}


/////////////////////////////////////////////槽函数///////////////////////////////////////////////////////////////////
/*
    if
*/


void Mousewb::worker_vice(INPUT input,QPoint point){
    if(!is_main&&!Cur_pc){//只有在鼠标处于副电脑端时允许调用
        QCursor::setPos(point);
        SendInput(1,&input,sizeof(INPUT));
    }
}


bool Mousewb::TCPconnect(){
    /*该函数初始化TCP连接，并交换屏幕数据信息*/
    return true;
    if(is_main){
        /*实例化Server 监听客户端请求，交换屏幕数据*/
        MouseInput input;
        input.flag = 0;
        input.Screen = GetScreenInfo();
        server->assignInput(input);
        emit readyServer();
        while(!Remote_screen){}//等待remote_screen更新
        initShade(*Remote_screen);
        return true;
    }
    else{
        /*实例化client 根据输入的ip发送连接请求，交换屏幕数据*/

        // Added
        MouseInput input;
        input.flag = 0;
        input.Screen = GetScreenInfo();
        server->assignInput(input);
        emit readyServer();
        while(!Remote_screen){}//等待remote_screen更新
        initShade(*Remote_screen);
        return true;
    }
    return false;
}

QRect Mousewb::GetScreenInfo(){
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect rect1 = screen->geometry();
    return rect1;
}

void Mousewb::initShade(QRect screen){
    if(is_main){
        shade = new MyShade();
        shade->move(0,0);
        QRect Local_screen = GetScreenInfo();
        if(Local_screen.width()>screen.width()&&Local_screen.height()>screen.height()){
            //目标屏幕小于当前屏幕
            shade->resize(screen.width(),screen.height());
        }
        else{
            //目标屏幕大于或等于当前屏幕
            shade->set_scale(2);
            shade->resize(screen.width()/shade->get_scale(),screen.height()/shade->get_scale());
        }
    }
    else{
        qDebug()<<"Client computer doesn't need shade";
        return;
    }

}





