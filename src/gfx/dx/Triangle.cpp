//
// Created by Jean on 9/18/2023.
//

#include <cassert>
#include "Triangle.h"

Triangle::Triangle(ID3D11Device *device) {
    float vertex_data_array[] = {
            0.0f, 0.5f, 0.0f, // point at top
            0.5f, -0.5f, 0.0f, // point at bottom-right
            -0.5f, -0.5f, 0.0f, // point at bottom-left
    };

    { /*** load mesh data into vertex buffer **/
        D3D11_BUFFER_DESC vertex_buff_descr = {};
        vertex_buff_descr.ByteWidth = sizeof(vertex_data_array);
        vertex_buff_descr.Usage = D3D11_USAGE_DEFAULT;
        vertex_buff_descr.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA sr_data = {};
        sr_data.pSysMem = vertex_data_array;
        HRESULT hr = device->CreateBuffer(
                &vertex_buff_descr,
                &sr_data,
                &vertexBuffer);
        assert(SUCCEEDED(hr));
    }
}