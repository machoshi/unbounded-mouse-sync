#include "mousewb.h"
/*
    执行流程：
        1、两台电脑各自开启无界鼠标程序，一台选择主电脑端模式，一台选择副电脑端模式
        2、输入相应的ip地址，建立tcp连接
        3、建立连接成功，启用无界鼠标功能
        4、若检测到连接断开或关闭窗口或强制措施ctrl+F11关闭无界鼠标

*/
Mousewb::Mousewb(int _fps,bool _is_main,serverWidget* _server, clientWidget* _client):server(_server),client(_client),fps(_fps),Cur_pc(1),is_main(_is_main){
    enabled = 0;
    Cur_pc = is_main;
    MouseTimer = new QTimer;
    lastpos = QCursor::pos();
    //初始化发送鼠标连接
}
void Mousewb::initMouse(){
    //接收到特定消息才启动mouse
    if(TCPconnect()){//执行屏幕数据交换
        qDebug()<<"TCP connect Success";
        enabled = 1;
    }
    else{
        qDebug()<<"TCP connect Fail";
        enabled = 0;
        return;
    }
    if(is_main){    //主电脑端进程

        MouseTimer->start(fps);
        qDebug()<<"开始监听鼠标";
        GlobalMouseEvent::installMouseEvent();
    }
    else{//副电脑端进程
        qDebug()<<"开始接收鼠标信息";
    }
}

void Mousewb::checkMouse(){
    QPoint point = QCursor::pos();
    curpos = point;
    if(curpos==lastpos){
        return;

    }
    else{
        lastpos = curpos;
    }
    INPUT input;
    input.type = INPUT_MOUSE;
    input.mi.dx = 0;
    input.mi.dy = 0;
    input.mi.dwExtraInfo = 0;
    input.mi.time = 0;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;

    MouseInput mouse_input;
    mouse_input.point = QPoint(point.x()*shade->get_scale(),point.y()*shade->get_scale());
    if(is_main){
        if(Cur_pc){
            if(point.x()==0){
                /*传输input结构体*/
                Cur_pc = 0;
                QCursor::setPos(shade->size().width(),point.y()/shade->get_scale());
                mouse_input.flag = 1;
                mouse_input.Cur_pc = 0;
                mouse_input.input = input;
                server->sendMouseInfoTrigger(mouse_input);
                emit readyServer();
                /*传输Cur_pc更改信号*/


                shade->show();
                return;
            }

        }
        else{
            //鼠标在副电脑端
            if(point.x()>shade->width()){//穿越边界
                /*传输cur_pc更改信号*/
                Cur_pc = 1;//鼠标移动到主电脑端
                QCursor::setPos(point.x()-shade->size().width(),point.y()*shade->get_scale());//移动鼠标到电脑边界
                shade->hide();


                mouse_input.flag = 1;
                mouse_input.Cur_pc = 1;
                mouse_input.input = input;
                server->sendMouseInfoTrigger(mouse_input);
                emit readyServer();
                /*修改本地鼠标位置*/


                return;
            }
            if(point.y()>=shade->height()){//鼠标到达底部边界
                QCursor::setPos(point.x(),shade->height());
                point.setY(shade->height());
                /*传输input结构体*/
                mouse_input.input = input;
                mouse_input.flag = 2;
                server->sendMouseInfoTrigger(mouse_input);
                emit readyServer();
                return;
            }
            mouse_input.input = input;
            mouse_input.flag = 2;
            server->sendMouseInfoTrigger(mouse_input);
            emit readyServer();
            return;
        }
    }
}

/*************************************槽函数******************************************************/
void Mousewb::on_mouseEvent(INPUT input,QPoint point){
    MouseInput mouse_input;
    mouse_input.point = QPoint(point.x()*shade->get_scale(),point.y()*shade->get_scale());
//    qDebug()<<"获取鼠标信息";
    if(is_main){    //只允许Server端执行该函数 以下信号传输均向Client端传输
        if(Cur_pc){ //鼠标在主电脑端
            qDebug()<<"鼠标点击";
            return;
//            if(point.x()==0){   //穿越边界
//                /*传输input结构体*/
//                mouse_input.flag = 1;
//                mouse_input.Cur_pc = 0;
//                mouse_input.input = input;
//                server->sendMouseInfoTrigger(mouse_input);
//                emit readyServer();
//                /*传输Cur_pc更改信号*/

//                /*修改本地鼠标位置*/
//                Cur_pc = 0;//鼠标移到副电脑端
//                QCursor::setPos(shade->size().width(),point.y()/shade->get_scale());//移动鼠标到遮罩边界
//                shade->show();


//                return;
//            }
        }
        else{   //鼠标在副电脑端
//            if(point.x()>shade->width()){//穿越边界
//                /*传输cur_pc更改信号*/
//                mouse_input.flag = 1;
//                mouse_input.Cur_pc = 1;
//                mouse_input.input = input;
//                server->sendMouseInfoTrigger(mouse_input);
//                emit readyServer();
//                /*修改本地鼠标位置*/
//                Cur_pc = 1;//鼠标移动到主电脑端
//                shade->hide();
//                QCursor::setPos(point.x()-shade->size().width(),point.y()*shade->get_scale());//移动鼠标到电脑边界

//                return;
//            }
//            if(point.y()>=shade->height()){//鼠标到达底部边界
//                QCursor::setPos(point.x(),shade->height());
//                point.setY(shade->height());
//                /*传输input结构体*/
//                mouse_input.input = input;
//                mouse_input.flag = 2;
//                server->sendMouseInfoTrigger(mouse_input);
//                emit readyServer();
//                return;
//            }
            //未到达任何边界

            /*传输input结构体*/
            qDebug()<<"鼠标点击";
            mouse_input.input = input;
            mouse_input.flag = 3;
            server->sendMouseInfoTrigger(mouse_input);
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
    //flag = 3，代表文件传输，对应的data类型为文件（已舍弃）
    //flag = 4，代表拖拽信号，对应的data类型为偏移量和文件名（已舍弃）
    qDebug()<<"接收信息 "<<input.flag;
//    QCursor::setPos(1,0);
    if(is_main){    //Server端只有第一次建立连接需要接收数据
        if(input.flag==0){
            qDebug()<<"接收屏幕信息"<<input.Screen.width()<<' '<<input.Screen.height();
            Remote_screen = new QRect(input.Screen);
        }
    }
    else{
        if(!enabled){
            qDebug()<<"无界鼠标尚未启动";
//            return;
        }
        qDebug()<<"无界鼠标信息";
        switch (input.flag) {
        case 0:
            qDebug()<<"接收屏幕信息"<<input.Screen.width()<<' '<<input.Screen.height();
            Remote_screen = new QRect(input.Screen);
//            *Remote_screen = input.Screen;
            break;
        case 1:
            Cur_pc = input.Cur_pc;
            QCursor::setPos(input.point);
            break;
        case 2:
            if(!is_main&&!Cur_pc){
                QCursor::setPos(input.point);
                qDebug()<<input.point.x()<<' '<<input.point.y();
                //SendInput(1,&input.input,sizeof(INPUT));
            }
            break;
        case 3:
            if(!is_main&&!Cur_pc){
                QCursor::setPos(input.point);
                qDebug()<<input.point.x()<<' '<<input.point.y();
                worker_vice(input.input,input.point);
                //SendInput(1,&input.input,sizeof(INPUT));
            }
        default:
            break;
        }
    }
}


/////////////////////////////////////////////槽函数///////////////////////////////////////////////////////////////////



void Mousewb::worker_vice(INPUT input,QPoint point){
    if(!is_main&&!Cur_pc){//只有在鼠标处于副电脑端时允许调用
        QCursor::setPos(point);

            input.mi.dwFlags=(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTDOWN);
            SendInput(1,&input,sizeof(INPUT));
            Sleep(20);
            input.mi.dwFlags= MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTUP;
            SendInput(1,&input,sizeof(INPUT));
            qDebug()<<"鼠标点击";
    }
}


bool Mousewb::TCPconnect(){
    /*该函数初始化TCP连接，并交换屏幕数据信息*/
    if(is_main){
        /*实例化Server 监听客户端请求，交换屏幕数据*/
        MouseInput input;
        input.flag = 0;
        input.Screen = GetScreenInfo();
        qDebug()<<"发送屏幕信息"<<input.Screen.width()<<' '<<input.Screen.height();
        server->sendMouseInfoTrigger(input);
        emit readyServer();
        if(!Remote_screen){ //初始化未成功


            return false;}//等待remote_screen更新
        initShade(*Remote_screen);
        return true;
    }
    else{
        /*实例化client 根据输入的ip发送连接请求，交换屏幕数据*/

        // Added
        MouseInput input;
        input.flag = 0;
        input.Screen = GetScreenInfo();
        server->sendMouseInfoTrigger(input);
        emit readyServer();
        qDebug()<<"发送屏幕信息"<<input.Screen.width()<<' '<<input.Screen.height();
        if(!Remote_screen){

            return false;}//等待remote_screen更新
        //副电脑端无需启用遮罩
//        initShade(*Remote_screen);
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
        qDebug()<<"初始化shade";
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





