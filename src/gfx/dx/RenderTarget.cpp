//
// Created by Jean on 9/18/2023.
//

#include "RenderTarget.h"
#include <cassert>

RenderTarget::RenderTarget(IDXGISwapChain *swapChain, ID3D11Device *device) {
    ID3D11Texture2D *framebuffer;
    HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **) &framebuffer);
    assert(SUCCEEDED(hr));

    hr = device->CreateRenderTargetView(
            framebuffer,
            nullptr,
            &renderTarget);
    assert(SUCCEEDED(hr));

    framebuffer->Release();
}

ID3D11RenderTargetView *RenderTarget::get() {
    return renderTarget;
}
