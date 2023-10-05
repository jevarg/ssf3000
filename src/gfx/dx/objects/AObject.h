//
// Created by Jean on 9/19/2023.
//

#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class AObject {
public:
    [[nodiscard]] virtual ID3D11Buffer *getBuffer() const { return m_VertexBuffer; }

    [[nodiscard]] virtual UINT getVertexCount() const { return m_VertexCount; }

    [[nodiscard]] virtual UINT getVertexStride() const { return m_VertexStride; }

    [[nodiscard]] virtual UINT getVertexOffset() const { return m_VertexOffset; }

    [[nodiscard]] virtual XMMATRIX getTransform() const {
        XMMATRIX transform = XMMatrixIdentity();

        transform *= XMMatrixScalingFromVector(XMLoadFloat3(&m_Scale));
        transform *= XMMatrixRotationX(XMConvertToRadians(m_Rotation.x));
        transform *= XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
        transform *= XMMatrixRotationZ(XMConvertToRadians(m_Rotation.z));
        transform *= XMMatrixTranslationFromVector(XMLoadFloat3(&m_Position));

        return transform;
    }

    virtual void rotate(float xDeg, float yDeg, float zDeg) {
        m_Rotation.x += xDeg;
        m_Rotation.y += yDeg;
        m_Rotation.z += zDeg;
    }

    virtual void translate(float x, float y, float z) {
        m_Position.x += x;
        m_Position.y += y;
        m_Position.z += z;
    }

    void scale(float x, float y, float z) {
        m_Scale.x += x;
        m_Scale.y += y;
        m_Scale.z += z;
    }

protected:
    AObject(UINT vCount, UINT vStride, UINT vOffset) : m_VertexBuffer(nullptr),
                                                       m_VertexCount(vCount),
                                                       m_VertexStride(vStride),
                                                       m_VertexOffset(vOffset),
                                                       m_Position(0, 0, 0),
                                                       m_Rotation(0, 0, 0),
                                                       m_Scale(1, 1, 1) {}

//    XMMATRIX transform;

    XMFLOAT3 m_Position;
    XMFLOAT3 m_Rotation;
    XMFLOAT3 m_Scale;

    ID3D11Buffer *m_VertexBuffer;

    UINT m_VertexCount;
    UINT m_VertexStride;
    UINT m_VertexOffset;
};