//
// Created by jeang on 10/2/2023.
//

#pragma once

#include <d3d11.h>
#include <cstdio>
#include <cassert>

class ConstantBuffer {
public:
    template<typename T>
    explicit ConstantBuffer(ID3D11Device *device, [[maybe_unused]] T type): mBuffer(nullptr) {
        assert((sizeof(T) % 16) == 0);
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
    };

    template<typename T>
    void setData(ID3D11DeviceContext *deviceCtx, const T &data) {
        deviceCtx->UpdateSubresource(mBuffer, 0, nullptr,
                                     &data, 0, 0);
    }

    ID3D11Buffer *getBuffer() { return mBuffer; }

private:
    ID3D11Buffer *mBuffer;
};