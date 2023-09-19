//
// Created by Jean on 9/19/2023.
//

#include <cassert>
#include "Cube.h"

Cube::Cube(ID3D11Device *device): AObject(6, 3 * sizeof(float), 0) {
    float vertexData[] = {
            -0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
    };

    { /*** load mesh data into vertex buffer **/
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.ByteWidth = sizeof(vertexData);
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA data = {};
        data.pSysMem = vertexData;

        HRESULT hr = device->CreateBuffer(
                &bufferDesc,
                &data,
                &vertexBuffer);

        assert(SUCCEEDED(hr));
    }
}
