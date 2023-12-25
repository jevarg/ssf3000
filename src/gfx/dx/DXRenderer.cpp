//
// Created by Jean on 9/18/2023.
//

#include <cassert>
#include <chrono>
#include "DXRenderer.h"
#include "objects/Cube.h"
#include "objects/Triangle.h"
#include "FrameCB.h"
#include "ConstantBuffers.h"
#include "AppCB.h"
#include "objects/Head.h"

using namespace DirectX;

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
    object = std::make_unique<Head>();
    object->build(device);

    RECT winRect;
    GetClientRect(hwnd, &winRect);
    D3D11_VIEWPORT viewport = {
            0.0f,
            0.0f,
            static_cast<float>(winRect.right - winRect.left),
            static_cast<float>(winRect.bottom - winRect.top),
            0.0f,
            1.0f};
    deviceCtx->RSSetViewports(1, &viewport);

    AppCB appCB(device);
    appCB.setData(deviceCtx, XMMatrixPerspectiveFovLH(XMConvertToRadians(40.0f),
                                                      viewport.Width / viewport.Height,
                                                      0.1f,
                                                      1000.0f));

    constantBuffers.push_back(appCB);
    constantBuffers.push_back(FrameCB(device));

    ID3D11Texture2D* depthStencilBuffer = nullptr;
    D3D11_TEXTURE2D_DESC descDepth;
    descDepth.Width = static_cast<UINT>(viewport.Width);
    descDepth.Height = static_cast<UINT>(viewport.Height);
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = device->CreateTexture2D(&descDepth, nullptr, &depthStencilBuffer);
    assert(SUCCEEDED(hr));

    hr = device->CreateDepthStencilView(depthStencilBuffer, nullptr, &depthStencilView);
    assert(SUCCEEDED(hr));
}

void DXRenderer::clear() {
    /* clear the back mBuffer to cornflower blue for the new frame */
//    float background_colour[4] = {
//            0x64 / 255.0f,
//            0x95 / 255.0f,
//            0xED / 255.0f,
//            1.0f};

    float background_colour[4] = { 0, 0, 0, 1.0f };

    deviceCtx->ClearRenderTargetView(renderTarget->get(), background_colour);
    deviceCtx->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DXRenderer::update() {
//    object->
}

void DXRenderer::render(HWND hwnd) {
    ID3D11RenderTargetView *rt = renderTarget->get();
//    ID3D11Buffer *vertexBuffer = object->getBuffer();

    deviceCtx->OMSetRenderTargets(1, &rt, depthStencilView);
    deviceCtx->IASetPrimitiveTopology(
            D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceCtx->IASetInputLayout(shader->getInputLayout());
    ID3D11Buffer *buffers[] = {
            object->buffers["POSITION"],
            object->buffers["TEXCOORD_0"]
    };
    constexpr UINT vertexStrides[] = {
            3 * sizeof(float),
            2 * sizeof(float)
    };
    constexpr UINT vertexOffsets[] = {
            0, 0
    };//object->getVertexOffset();
    deviceCtx->IASetVertexBuffers(
            0,
            2,
            buffers,
            vertexStrides,
            vertexOffsets);
    deviceCtx->IASetIndexBuffer(object->indices, DXGI_FORMAT_R16_UINT, 0);

    auto duration = std::chrono::system_clock::now().time_since_epoch();
    const XMVECTOR eyePosition = XMVectorSet(0, 1, -5, 1);
    const XMVECTOR focusPoint = XMVectorSet(0, 1, 0, 1);
    const XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);

    const auto viewMatrix = XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);

    object->rotate(0, 0.2f, 0);
    constantBuffers[FRAME].setData<FrameCB::Data>(deviceCtx, {
        static_cast<int32_t>(duration.count()),
        viewMatrix,
        object->getTransform()
    });

    ID3D11Buffer *cBuffers[] = {
            constantBuffers[APP].getBuffer(),
            constantBuffers[FRAME].getBuffer(),
    };

    deviceCtx->VSSetShader(shader->getVertexShader(), nullptr, 0);
    deviceCtx->PSSetShader(shader->getPixelShader(), nullptr, 0);
    deviceCtx->VSSetConstantBuffers(0, 2, cBuffers);
    deviceCtx->PSSetShaderResources(0, 1, &object->srv);

    deviceCtx->DrawIndexed(object->indexCount, 0, 0);
//    deviceCtx->Draw(object->getVertexCount(), 0);
    swapChain->Present(1, 0);
}
