#include "globalmouseevent.h"
static HHOOK g_hook = nullptr;
//MOUSEEVENTF_ABSOLUTE
//MOUSEEVENTF_LEFTDOWN
//MOUSEEVENTF_LEFTUP
//MOUSEEVENTF_RIGHTDOWN
//MOUSEEVENTF_RIGHTUP
//MOUSEEVENTF_MOVE
//    WM_MOUSEMOVE = 0x200
//    WM_LBUTTONDOWN = 0x201
//    WM_LBUTTONUP = 0x202
//    WM_LBUTTONDBLCLK = 0x203
//    WM_RBUTTONDOWN = 0x204
//    WM_RBUTTONUP = 0x205
//    WM_RBUTTONDBLCLK = 0x206

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    MSLLHOOKSTRUCT *msl =  reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);

    INPUT input;
    input.type = INPUT_MOUSE;
    input.mi.dx = 0;
    input.mi.dy = 0;
    QPoint point(msl->pt.x,msl->pt.y);
    input.mi.dwExtraInfo = 0;
    input.mi.time = 0;
    switch (wParam) {
    case WM_LBUTTONDOWN:
        input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTDOWN;
        break;
    case WM_LBUTTONUP:
        input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTUP;
        break;
    case WM_RBUTTONDOWN:
        input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTDOWN;
        break;
    case WM_RBUTTONUP:
        input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTUP;
        break;
    case WM_MOUSEMOVE:
        input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE;
        break;
    default:
        return CallNextHookEx(nullptr, nCode, wParam, lParam);
        break;
    }
    emit GlobalMouseEvent::getInstance()->mouseEvent(input,point);

    return CallNextHookEx(nullptr, nCode, wParam, lParam);   // 注意这一行一定不能少，否则会出大问题
}

bool GlobalMouseEvent::installMouseEvent()
{
    if(g_hook) return true;     // 避免重复安装
    g_hook = SetWindowsHookExW(WH_MOUSE_LL, LowLevelMouseProc, GetModuleHandleW(nullptr), 0);
    return g_hook;
}
bool GlobalMouseEvent::removeMouseEvent()
{
    if(!g_hook) return true;   // 避免重复卸载
    bool ret = UnhookWindowsHookEx(g_hook);
    if(ret)
    {
        g_hook = nullptr;
        return true;
    }
    return false;
}
