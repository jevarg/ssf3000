//
// Created by Jean on 10/20/2023.
//

#include "AObject.h"

ID3D11Buffer *AObject::getBuffer() const {
    return m_VertexBuffer;
}

UINT AObject::getVertexCount() const {
    return m_VertexCount;
}

UINT AObject::getVertexStride() const {
    return m_VertexStride;
}

UINT AObject::getVertexOffset() const {
    return m_VertexOffset;
}

XMMATRIX AObject::getTransform() const {
    XMMATRIX transform = XMMatrixIdentity();

    transform *= XMMatrixScalingFromVector(XMLoadFloat3(&m_Scale));
    transform *= XMMatrixRotationX(XMConvertToRadians(m_Rotation.x));
    transform *= XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
    transform *= XMMatrixRotationZ(XMConvertToRadians(m_Rotation.z));
    transform *= XMMatrixTranslationFromVector(XMLoadFloat3(&m_Position));

    return transform;
}

void AObject::rotate(float xDeg, float yDeg, float zDeg) {
    m_Rotation.x += xDeg;
    m_Rotation.y += yDeg;
    m_Rotation.z += zDeg;
}

void AObject::translate(float x, float y, float z) {
    m_Position.x += x;
    m_Position.y += y;
    m_Position.z += z;
}

void AObject::scale(float x, float y, float z) {
    m_Scale.x += x;
    m_Scale.y += y;
    m_Scale.z += z;
}

AObject::AObject(UINT vCount, UINT vStride, UINT vOffset) : m_VertexCount(vCount),
                                                            m_VertexStride(vStride),
                                                            m_VertexOffset(vOffset) {}