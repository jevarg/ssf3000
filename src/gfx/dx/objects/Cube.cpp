//
// Created by Jean on 9/19/2023.
//

#include <cassert>
#include "Cube.h"

Cube::Cube(ID3D11Device *device): AObject(36, 6 * sizeof(float), 0) {
    float vertexData[] = {
            // Top
            -0.5f, 0.5f, 0.0f, 1, 0, 0,
            0.5f, 0.5f, 0.0f, 1, 0, 0,
            -0.5f, 0.5f, 1.0f, 1, 0, 0,
            -0.5f, 0.5f, 1.0f, 1, 0, 0,
            0.5f, 0.5f, 0.0f, 1, 0, 0,
            0.5f, 0.5f, 1.0f, 1, 0, 0,

            // Bottom
            -0.5f, -0.5f, 0.0f, 1, 0, 0,
            0.5f, -0.5f, 0.0f, 1, 0, 0,
            -0.5f, -0.5f, 1.0f, 1, 0, 0,
            -0.5f, -0.5f, 1.0f, 1, 0, 0,
            0.5f, -0.5f, 0.0f, 1, 0, 0,
            0.5f, -0.5f, 1.0f, 1, 0, 0,

            // Back
            0.5f, 0.5f, 0.0f, 0, 1, 0,
            -0.5f, 0.5f, 0.0f, 0, 1, 0,
            0.5f, -0.5f, 0.0f, 0, 1, 0,
            0.5f, -0.5f, 0.0f, 0, 1, 0,
            -0.5f, 0.5f, 0.0f, 0, 1, 0,
            -0.5f, -0.5f, 0.0f, 0, 1, 0,

            // Front
            -0.5f, 0.5f, 1.0f, 0, 1, 0,
            0.5f, 0.5f, 1.0f, 0, 1, 0,
            0.5f, -0.5f, 1.0f, 0, 1, 0,
            -0.5f, 0.5f, 1.0f, 0, 1, 0,
            0.5f, -0.5f, 1.0f, 0, 1, 0,
            -0.5f, -0.5f, 1.0f, 0, 1, 0,

            // Left
            -0.5f, 0.5f, 0.0f, 0, 0, 1,
            -0.5f, 0.5f, 1.0f, 0, 0, 1,
            -0.5f, -0.5f, 0.0f, 0, 0, 1,
            -0.5f, 0.5f, 1.0f, 0, 0, 1,
            -0.5f, -0.5f, 1.0f, 0, 0, 1,
            -0.5f, -0.5f, 0.0f, 0, 0, 1,

            // Right
            0.5f, 0.5f, 1.0f, 0, 0, 1,
            0.5f, 0.5f, 0.0f, 0, 0, 1,
            0.5f, -0.5f, 0.0f, 0, 0, 1,
            0.5f, -0.5f, 0.0f, 0, 0, 1,
            0.5f, -0.5f, 1.0f, 0, 0, 1,
            0.5f, 0.5f, 1.0f, 0, 0, 1,
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
                &vertexBuffer);

        assert(SUCCEEDED(hr));
    }
}
