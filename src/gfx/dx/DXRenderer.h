//
// Created by Jean on 9/18/2023.
//

#pragma once

#include <d3d11.h>
#include <memory>
#include <cstdint>
#include <vector>
#include "Shader.h"
#include "RenderTarget.h"
#include "ConstantBuffers.h"
#include "FrameCB.h"
#include "objects/AObject.h"
#include "objects/Head.h"

class DXRenderer {
public:
    explicit DXRenderer(HWND hwnd);
    DXRenderer(const DXRenderer&) = delete;

    void clear();
    void render(HWND hwnd);
    void update();

private:
    IDXGISwapChain *swapChain = nullptr;
    ID3D11Device *device = nullptr;
    ID3D11DeviceContext *deviceCtx = nullptr;
    ID3D11DepthStencilView *depthStencilView = nullptr;

    std::vector<ConstantBuffer> constantBuffers;
    std::shared_ptr<RenderTarget> renderTarget = nullptr;
    std::shared_ptr<Shader> shader = nullptr;
    std::unique_ptr<Head> object;
};
