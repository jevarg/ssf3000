//
// Created by Jean on 9/19/2023.
//

#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class AObject {
public:
    virtual void build(ID3D11Device *device) = 0;

    [[nodiscard]] virtual ID3D11Buffer *getBuffer() const;

    [[nodiscard]] virtual UINT getVertexCount() const;

    [[nodiscard]] virtual UINT getVertexStride() const;

    [[nodiscard]] virtual UINT getVertexOffset() const;

    [[nodiscard]] virtual XMMATRIX getTransform() const;

    virtual void rotate(float xDeg, float yDeg, float zDeg);

    virtual void translate(float x, float y, float z);

    void scale(float x, float y, float z);

protected:
    AObject(UINT vCount, UINT vStride, UINT vOffset);

    XMFLOAT3 m_Position = {0, 0, 0};
    XMFLOAT3 m_Rotation = {0, 0, 0};
    XMFLOAT3 m_Scale = {1, 1, 1};

    UINT m_VertexCount;
    UINT m_VertexStride;
    UINT m_VertexOffset;
    UINT m_IndexCount = 0;

    ID3D11Buffer *m_VertexBuffer = nullptr;
    ID3D11Buffer *m_IndexBuffer = nullptr;

};