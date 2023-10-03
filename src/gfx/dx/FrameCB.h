//
// Created by Jean on 10/1/2023.
//

#pragma once

#include <cassert>
#include <d3d11.h>
#include "../ConstantBuffer.h"

class FrameCB: public ConstantBuffer {
public:
    struct Data {
        int32_t time;
        DirectX::XMMATRIX viewMatrix;
        [[maybe_unused]] int32_t _padding[3];
    };

    explicit FrameCB(ID3D11Device *device) : ConstantBuffer(device, Data()) {}
};
