#include "stdafx.h"
#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    SystemClass* System;
    bool result;

    // System 객체 생성
    System  = new SystemClass;
    if(!System)
    {
        return 0;
    }

    // system 객체 초기화하고 run을 호출한다.
    result  = System->Initialize();
    if(result)
    {
        System->Run();
    }

    // system 객체를 종료하고 메모리를 반환한다.
    System->Shutdown();
    delete System;
    System  = nullptr;

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
        return 0;
    }

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}