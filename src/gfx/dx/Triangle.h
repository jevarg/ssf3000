//
// Created by Jean on 9/18/2023.
//

#pragma once

#include <d3d11.h>

class Triangle {
public:
    explicit Triangle(ID3D11Device *device);

    ID3D11Buffer *vertexBuffer = nullptr;
};
