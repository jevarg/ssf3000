//
// Created by Jean on 9/18/2023.
//

#pragma once

#include <dxgi.h>
#include <d3d11.h>

class RenderTarget {
public:
    RenderTarget(IDXGISwapChain *swapChain, ID3D11Device *device);
    ID3D11RenderTargetView *get();

private:
    ID3D11RenderTargetView *renderTarget = nullptr;
};
