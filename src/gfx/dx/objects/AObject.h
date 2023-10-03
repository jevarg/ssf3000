//
// Created by Jean on 9/19/2023.
//

#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class AObject {
public:
    [[nodiscard]] virtual ID3D11Buffer *getBuffer() const { return vertexBuffer; }

    [[nodiscard]] virtual UINT getVertexCount() const { return vertexCount; }

    [[nodiscard]] virtual UINT getVertexStride() const { return vertexStride; }

    [[nodiscard]] virtual UINT getVertexOffset() const { return vertexOffset; }

    [[nodiscard]] virtual DirectX::XMMATRIX getModelMatrix() const { return modelMatrix; }

protected:
    AObject(UINT vCount, UINT vStride, UINT vOffset) : vertexBuffer(nullptr),
                                                       vertexCount(vCount),
                                                       vertexStride(vStride),
                                                       vertexOffset(vOffset),
                                                       modelMatrix(DirectX::XMMatrixIdentity()) {}

    ID3D11Buffer *vertexBuffer;
    DirectX::XMMATRIX modelMatrix;

    UINT vertexCount;
    UINT vertexStride;
    UINT vertexOffset;
};