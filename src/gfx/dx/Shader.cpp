//
// Created by Jean on 9/18/2023.
//

#include <d3dcompiler.h>
#include <cassert>
#include "Shader.h"

Shader::Shader(ID3D11Device *device, LPCWSTR vertexFile, LPCWSTR pixelFile) {
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3DCOMPILE_DEBUG; // add more debug output
#endif

    ID3DBlob *vs_blob_ptr = nullptr;
    ID3DBlob *ps_blob_ptr = nullptr;
    ID3DBlob *error_blob = nullptr;

    // COMPILE FRAME SHADER
    HRESULT hr = D3DCompileFromFile(
            vertexFile,
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "vs_main",
            "vs_5_0",
            flags,
            0,
            &vs_blob_ptr,
            &error_blob);
    if (FAILED(hr)) {
        if (error_blob) {
            OutputDebugStringA((char *) error_blob->GetBufferPointer());
            error_blob->Release();
        }
        if (vs_blob_ptr) { vs_blob_ptr->Release(); }
        assert(false);
    }

    // COMPILE PIXEL SHADER
    hr = D3DCompileFromFile(
            pixelFile,//L"shaders/default.hlsl",
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "ps_main",
            "ps_5_0",
            flags,
            0,
            &ps_blob_ptr,
            &error_blob);
    if (FAILED(hr)) {
        if (error_blob) {
            OutputDebugStringA((char *) error_blob->GetBufferPointer());
            error_blob->Release();
        }
        if (ps_blob_ptr) { ps_blob_ptr->Release(); }
        assert(false);
    }

    hr = device->CreateVertexShader(
            vs_blob_ptr->GetBufferPointer(),
            vs_blob_ptr->GetBufferSize(),
            nullptr,
            &vertexShader);
    assert(SUCCEEDED(hr));

    hr = device->CreatePixelShader(
            ps_blob_ptr->GetBufferPointer(),
            ps_blob_ptr->GetBufferSize(),
            nullptr,
            &pixelShader);
    assert(SUCCEEDED(hr));

    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//            { "COL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            /*
            { "COL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            */
    };
    hr = device->CreateInputLayout(
            inputElementDesc,
            ARRAYSIZE(inputElementDesc),
            vs_blob_ptr->GetBufferPointer(),
            vs_blob_ptr->GetBufferSize(),
            &inputLayout);
    assert(SUCCEEDED(hr));
}

ID3D11InputLayout *Shader::getInputLayout() {
    return inputLayout;
}

ID3D11VertexShader *Shader::getVertexShader() {
    return vertexShader;
}

ID3D11PixelShader *Shader::getPixelShader() {
    return pixelShader;
}
