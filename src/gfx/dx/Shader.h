//
// Created by Jean on 9/18/2023.
//

#pragma once

#include <d3d11.h>

class Shader {
public:
    Shader(ID3D11Device *device, LPCWSTR vertexFile, LPCWSTR pixelFile);

    ID3D11VertexShader *getVertexShader();
    ID3D11PixelShader *getPixelShader();
    ID3D11InputLayout *getInputLayout();

private:
    ID3D11VertexShader *vertexShader = nullptr;
    ID3D11PixelShader *pixelShader = nullptr;

    ID3D11InputLayout *inputLayout = nullptr;
};
