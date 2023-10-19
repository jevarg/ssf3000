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

    constantBuffers.push_back(appCB); // CBType::App [0]
    constantBuffers.push_back(FrameCB(device)); // CBType::FRAME [1]
//    Head head;
//    head.build(device);
}

void DXRenderer::clear() {
    /* clear the back mBuffer to cornflower blue for the new frame */
//    float background_colour[4] = {
//            0x64 / 255.0f,
//            0x95 / 255.0f,
//            0xED / 255.0f,
//            1.0f};

    float background_colour[4] = {
            0, 0, 0,
            1.0f
    };

    deviceCtx->ClearRenderTargetView(renderTarget->get(), background_colour);
}

void DXRenderer::update() {
//    object->
}

void DXRenderer::render(HWND hwnd) {
    ID3D11RenderTargetView *rt = renderTarget->get();
//    ID3D11Buffer *vertexBuffer = object->getBuffer();
    UINT vertexStride = 3 * sizeof(float);//object->getVertexStride();
    UINT vertexOffset = 0;//object->getVertexOffset();

    deviceCtx->OMSetRenderTargets(1, &rt, nullptr);
    deviceCtx->IASetPrimitiveTopology(
            D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceCtx->IASetInputLayout(shader->getInputLayout());
    deviceCtx->IASetVertexBuffers(
            0,
            1,
            &object->buffers["POSITION"],
            &vertexStride,
            &vertexOffset);
    deviceCtx->IASetIndexBuffer(object->indices, DXGI_FORMAT_R16_UINT, 0);

    auto duration = std::chrono::system_clock::now().time_since_epoch();
    XMVECTOR eyePosition = XMVectorSet(0, 3, -5, 1);
    XMVECTOR focusPoint = XMVectorSet(0, 0, 0, 1);
    XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);

    auto viewMatrix = XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);

    object->rotate(0, 0.2f, 0);
    constantBuffers[CBType::FRAME].setData<FrameCB::Data>(deviceCtx, {
            static_cast<int32_t>(duration.count()),
            viewMatrix,
            object->getTransform()
    });

    ID3D11Buffer *cBuffers[] = {
            constantBuffers[CBType::APP].getBuffer(),
            constantBuffers[CBType::FRAME].getBuffer(),
    };

    deviceCtx->VSSetShader(shader->getVertexShader(), nullptr, 0);
    deviceCtx->PSSetShader(shader->getPixelShader(), nullptr, 0);
    deviceCtx->VSSetConstantBuffers(0, 2, cBuffers);

    deviceCtx->DrawIndexed(object->indexCount, 0, 0);
//    deviceCtx->Draw(object->getVertexCount(), 0);
    swapChain->Present(1, 0);
}
