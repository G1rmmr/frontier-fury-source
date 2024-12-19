// FrontierFury.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "FrontierFury.h"

using namespace DirectX;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 윈도우 클래스 설정
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"DirectX12WindowClass";

    // 윈도우 클래스 등록
    if(!RegisterClass(&wc)) return -1;

    // 윈도우 생성
    HWND hwnd = CreateWindowEx(
        0, L"DirectX12WindowClass", L"DirectX12 Window",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600, nullptr, nullptr, hInstance, nullptr);

    if(!hwnd) return -1;

    ShowWindow(hwnd, nCmdShow);

    // 메시지 루프
    MSG msg = {};
    while(GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}