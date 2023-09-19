//
// Created by Jean on 9/18/2023.
//

#pragma once

#include <d3d11.h>
#include <memory>
#include "Shader.h"
#include "RenderTarget.h"
#include "objects/AObject.h"

class DXRenderer {
public:
    explicit DXRenderer(HWND hwnd);
    DXRenderer(const DXRenderer&) = delete;

    void clear();
    void render(HWND hwnd);

private:
    IDXGISwapChain *swapChain = nullptr;

    ID3D11Device *device = nullptr;
    ID3D11DeviceContext *deviceCtx = nullptr;
    std::shared_ptr<RenderTarget> renderTarget = nullptr;
    std::shared_ptr<Shader> shader = nullptr;

    std::unique_ptr<AObject> object = nullptr;
};
