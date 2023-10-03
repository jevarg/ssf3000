//
// Created by Jean on 9/18/2023.
//

#include <cassert>
#include "Triangle.h"

Triangle::Triangle(ID3D11Device *device) : AObject(3, 6 * sizeof(float), 0) {

    float vertexData[] = {
            0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // point at top
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // point at bottom-right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // point at bottom-left
    };

    { /*** load mesh data into vertex mBuffer **/
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.ByteWidth = sizeof(vertexData);
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA data = {};
        data.pSysMem = vertexData;

        HRESULT hr = device->CreateBuffer(
                &bufferDesc,
                &data,
                &m_VertexBuffer);

        assert(SUCCEEDED(hr));
    }
}
