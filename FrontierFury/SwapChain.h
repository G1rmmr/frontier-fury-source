#pragma once

#include "framework.h"

class SwapChain
{
public:
    SwapChain();
    ~SwapChain();

    bool Init(
        IDXGIFactory4* factory,
        ID3D12CommandQueue* cmdQueue,
        HWND hwnd,
        UINT width,
        UINT height);

    inline IDXGISwapChain4* Get() const
    {
        return mSwapChain.Get();
    }

private:
    Microsoft::WRL::ComPtr<IDXGISwapChain4> mSwapChain;
    UINT mFrameIndex;
};
