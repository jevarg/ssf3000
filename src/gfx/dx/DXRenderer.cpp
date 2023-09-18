//
// Created by Jean on 9/18/2023.
//

#include <cassert>
#include "DXRenderer.h"

DXRenderer::DXRenderer(HWND hwnd) {
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.Windowed = true;

    D3D_FEATURE_LEVEL featureLvl;
    UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;

#if defined(DEBUG) || defined(_DEBUG)
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            flags,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &swapChainDesc,
            &swapChain,
            &device,
            &featureLvl,
            &deviceCtx);
    assert(hr == S_OK && swapChain && device && deviceCtx);

    renderTarget = std::make_shared<RenderTarget>(swapChain, device);
    shader = std::make_shared<Shader>(device, L"shaders/default.hlsl", L"shaders/default.hlsl");
    triangle = std::make_unique<Triangle>(device);
}

void DXRenderer::clear() {
    /* clear the back buffer to cornflower blue for the new frame */
    float background_colour[4] = {0x64 / 255.0f, 0x95 / 255.0f, 0xED / 255.0f, 1.0f};
    deviceCtx->ClearRenderTargetView(renderTarget->get(), background_colour);
}

void DXRenderer::render(HWND hwnd) {
    ID3D11RenderTargetView *rt = renderTarget->get();

    UINT vertex_stride = 3 * sizeof(float);
    UINT vertex_offset = 0;
    UINT vertex_count = 3;

    RECT winRect;
    GetClientRect(hwnd, &winRect);
    D3D11_VIEWPORT viewport = {
            0.0f,
            0.0f,
            (FLOAT) (winRect.right - winRect.left),
            (FLOAT) (winRect.bottom - winRect.top),
            0.0f,
            1.0f};
    deviceCtx->RSSetViewports(1, &viewport);
    deviceCtx->OMSetRenderTargets(1, &rt, nullptr);
    deviceCtx->IASetPrimitiveTopology(
            D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceCtx->IASetInputLayout(shader->getInputLayout());
    deviceCtx->IASetVertexBuffers(
            0,
            1,
            &triangle->vertexBuffer,
            &vertex_stride,
            &vertex_offset);

    deviceCtx->VSSetShader(shader->getVertexShader(), nullptr, 0);
    deviceCtx->PSSetShader(shader->getPixelShader(), nullptr, 0);

    deviceCtx->Draw(vertex_count, 0);
    swapChain->Present(1, 0);
}
