#ifndef GLOBALMOUSEEVENT_H
#define GLOBALMOUSEEVENT_H

#include <QObject>
#include<QDebug>
#include<QPoint>
#include"Windows.h"
class GlobalMouseEvent : public QObject
{
    Q_OBJECT
public:
    static GlobalMouseEvent* getInstance(){
        static GlobalMouseEvent mouseEvent;
        return &mouseEvent;
    }
    static bool installMouseEvent();
    static bool removeMouseEvent();

signals:
    void mouseEvent(INPUT input,QPoint point); //input触发事件，point提供事件触发位置

private:
    GlobalMouseEvent(){}
};

#endif // GLOBALMOUSEEVENT_H
