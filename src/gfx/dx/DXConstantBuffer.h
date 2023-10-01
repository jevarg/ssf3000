//
// Created by Jean on 10/1/2023.
//

#pragma once

#include <cassert>
#include <d3d11.h>

template<typename T>
class DXConstantBuffer {
public:
    explicit DXConstantBuffer(ID3D11Device *device): mBuffer(nullptr) {
        // Fill in a buffer description.
        D3D11_BUFFER_DESC cbDesc = {0};
        cbDesc.ByteWidth = sizeof(T);
        cbDesc.Usage = D3D11_USAGE_DEFAULT;
        cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

        // Create the buffer.
        HRESULT hr = device->CreateBuffer(&cbDesc,
                                          nullptr,
                                          &mBuffer);
        assert(SUCCEEDED(hr));
    }

private:
    ID3D11Buffer *mBuffer;
};
