#include "SwapChain.h"

SwapChain::SwapChain() : mFrameIndex(0)
{

}

SwapChain::~SwapChain()
{

}

bool SwapChain::Init(
    IDXGIFactory4* factory,
    ID3D12CommandQueue* cmdQueue,
    HWND hwnd,
    UINT width,
    UINT height)
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Width = width;
    swapChainDesc.Height = height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
    if(FAILED(factory->CreateSwapChainForHwnd(
        cmdQueue, hwnd, &swapChainDesc, nullptr, nullptr, &swapChain)))
    {
        return false;
    }

    swapChain.As(&mSwapChain);
    mFrameIndex = mSwapChain->GetCurrentBackBufferIndex();
    return true;
}