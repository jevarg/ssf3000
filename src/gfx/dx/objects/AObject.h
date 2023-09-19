//
// Created by Jean on 9/19/2023.
//

#pragma once

#include <d3d11.h>

class AObject {
public:
    [[nodiscard]] virtual ID3D11Buffer *getBuffer() const { return vertexBuffer; }

    [[nodiscard]] virtual UINT getVertexCount() const { return vertexCount; }

    [[nodiscard]] virtual UINT getVertexStride() const { return vertexStride; }

    [[nodiscard]] virtual UINT getVertexOffset() const { return vertexOffset; }

protected:
    AObject(UINT vCount, UINT vStride, UINT vOffset) : vertexCount(vCount),
                                                       vertexStride(vStride),
                                                       vertexOffset(vOffset) {}

    ID3D11Buffer *vertexBuffer = nullptr;

    UINT vertexCount;
    UINT vertexStride;
    UINT vertexOffset;
};