// FrontierFury.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "FrontierFury.h"
#include "SwapChain.h"

using namespace DirectX;
using namespace Microsoft::WRL;

ComPtr<ID3D12Device> device;
ComPtr<ID3D12CommandQueue> cmdQueue;
ComPtr<IDXGIFactory4> factory;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void InitDX()
{
    // TO-DO
}

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
        0,
        L"DirectX12WindowClass",
        L"DirectX12 Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        800, 600,
        nullptr,
        nullptr,
        hInstance,
        nullptr);

    if(!hwnd) return -1;

    ShowWindow(hwnd, nCmdShow);

    // DXGI 팩토리 생성
    if(FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
    {
        MessageBox(nullptr, L"Failed to create DXGI Factory.", L"Error", MB_OK);
        return -1;
    }

    // 디바이스 생성
    if(FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device))))
    {
        MessageBox(nullptr, L"Failed to create D3D12 Device.", L"Error", MB_OK);
        return -1;
    }

    // 커맨드 큐 생성
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    if(FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdQueue))))
    {
        MessageBox(nullptr, L"Failed to create Command Queue.", L"Error", MB_OK);
        return -1;
    }

    // 스왑체인 초기화
    SwapChain swapChain;
    if(!swapChain.Init(factory.Get(), cmdQueue.Get(), hwnd, 800, 600))
    {
        MessageBox(nullptr, L"Failed to initialize Swap Chain", L"Error", MB_OK);
        return -1;
    }

    // 렌더링 루프
    MSG msg = {};

    while(true)
    {
        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT) break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Sleep(16);
            // TO-DO
        }

            
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