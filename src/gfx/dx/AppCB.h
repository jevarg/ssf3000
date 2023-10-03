//
// Created by jeang on 10/2/2023.
//

#pragma once

#include <DirectXMath.h>
#include "../ConstantBuffer.h"

class AppCB: public ConstantBuffer {
public:
    struct Data {
        DirectX::XMMATRIX projectionMatrix;
    };

    explicit AppCB(ID3D11Device *device): ConstantBuffer(device, Data()) {};
};