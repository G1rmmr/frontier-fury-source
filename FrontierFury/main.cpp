// main.cpp

#include <windows.h>
#include <d3d11.h>
#include <wrl/client.h> // Microsoft::WRL::ComPtr
#include <dxgi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <string>

// 라이브러리 링크
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using Microsoft::WRL::ComPtr;

// 전역 변수
HINSTANCE hInst;
HWND hWnd = nullptr;

// 화면 크기
const int WIDTH = 800;
const int HEIGHT = 600;

// DirectX 11 객체
ComPtr<ID3D11Device> gDevice;
ComPtr<ID3D11DeviceContext> gDeviceContext;
ComPtr<IDXGISwapChain> gSwapChain;
ComPtr<ID3D11RenderTargetView> gRenderTargetView;

// 함수 선언
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool InitializeWindow(HINSTANCE hInstance, int nCmdShow);
bool InitializeDirectX();
void Cleanup();
void Render();

// 진입점
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    // 윈도우 초기화
    if(!InitializeWindow(hInstance, nCmdShow))
    {
        MessageBox(nullptr, L"윈도우 초기화 실패!", L"오류", MB_ICONERROR);
        return -1;
    }

    // DirectX 11 초기화
    if(!InitializeDirectX())
    {
        MessageBox(nullptr, L"DirectX 초기화 실패!", L"오류", MB_ICONERROR);
        return -1;
    }

    // 메시지 루프
    MSG msg = {0};
    while(msg.message != WM_QUIT)
    {
        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // 렌더링
            Render();
        }
    }

    // 정리
    Cleanup();

    return static_cast<int>(msg.wParam);
}

// 윈도우 프로시저
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        ValidateRect(hwnd, NULL);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

// 윈도우 초기화 함수
bool InitializeWindow(HINSTANCE hInstance, int nCmdShow)
{
    // 윈도우 클래스 등록
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"DirectXWindowClass";

    if(!RegisterClassEx(&wc))
    {
        MessageBox(nullptr, L"윈도우 클래스 등록 실패!", L"오류", MB_ICONERROR);
        return false;
    }

    // 윈도우 생성
    RECT windowRect = {0, 0, WIDTH, HEIGHT};
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        L"DirectX 11 초기화 예제",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if(!hWnd)
    {
        MessageBox(nullptr, L"윈도우 생성 실패!", L"오류", MB_ICONERROR);
        return false;
    }

    ShowWindow(hWnd, nCmdShow);
    return true;
}

// DirectX 11 초기화 함수
bool InitializeDirectX()
{
    HRESULT hr;

    // 스왑 체인 설명 설정
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = WIDTH;
    scd.BufferDesc.Height = HEIGHT;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60; // 60Hz
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 1; // 멀티샘플링 비활성화
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // 디바이스, 디바이스 컨텍스트, 스왑 체인 생성
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0
    };
    D3D_FEATURE_LEVEL featureLevel;

    hr = D3D11CreateDeviceAndSwapChain(
        nullptr,                    // 기본 어댑터 사용
        D3D_DRIVER_TYPE_HARDWARE,   // 하드웨어 가속 사용
        nullptr,                    // 소프트웨어 래스터라이저 사용 안함
        createDeviceFlags,          // 디바이스 생성 플래그
        featureLevels,              // 지원할 피처 레벨
        1,                          // 피처 레벨 개수
        D3D11_SDK_VERSION,          // SDK 버전
        &scd,                       // 스왑 체인 설명
        &gSwapChain,                // 스왑 체인
        &gDevice,                   // 디바이스
        &featureLevel,              // 실제 생성된 피처 레벨
        &gDeviceContext             // 디바이스 컨텍스트
    );

    if(FAILED(hr))
    {
        MessageBox(nullptr, L"D3D11CreateDeviceAndSwapChain 실패!", L"오류", MB_ICONERROR);
        return false;
    }

    // 스왑 체인으로부터 백 버퍼 가져오기
    ComPtr<ID3D11Texture2D> backBuffer;
    hr = gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);
    if(FAILED(hr))
    {
        MessageBox(nullptr, L"GetBuffer 실패!", L"오류", MB_ICONERROR);
        return false;
    }

    // 렌더 타겟 뷰 생성
    hr = gDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &gRenderTargetView);
    if(FAILED(hr))
    {
        MessageBox(nullptr, L"CreateRenderTargetView 실패!", L"오류", MB_ICONERROR);
        return false;
    }

    // 렌더 타겟을 설정
    gDeviceContext->OMSetRenderTargets(1, gRenderTargetView.GetAddressOf(), nullptr);

    // 뷰포트 설정
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<float>(WIDTH);
    viewport.Height = static_cast<float>(HEIGHT);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    gDeviceContext->RSSetViewports(1, &viewport);

    return true;
}

// 렌더링 함수
void Render()
{
    // 화면 클리어 색 (예: 어두운 파란색)
    float clearColor[4] = {0.0f, 0.2f, 0.4f, 1.0f};
    gDeviceContext->ClearRenderTargetView(gRenderTargetView.Get(), clearColor);

    // 여기에 추가적인 렌더링 명령어를 추가할 수 있습니다.

    // 스왑 체인 프레젠트 (버퍼를 화면에 표시)
    gSwapChain->Present(1, 0); // VSync 사용
}

// 정리 함수
void Cleanup()
{
    // ComPtr을 사용하면 자동으로 리소스가 해제됩니다.
    // 명시적으로 Release를 호출할 필요가 없습니다.
}
