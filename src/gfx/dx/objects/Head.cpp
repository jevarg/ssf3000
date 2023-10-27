//
// Created by Jean on 10/16/2023.
//

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "Head.h"

Head::Head(): AObject(0, 0, 0) {
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool success = loader.LoadBinaryFromFile(&m_Model, &err, &warn, "assets/sarah.glb");
    if (!warn.empty()) {
        printf("Warn: %s\n", warn.c_str());
    }

    if (!err.empty()) {
        printf("Err: %s\n", err.c_str());
    }

    if (!success) {
        printf("Failed to parse glTF\n");
        exit(-1);
    }
}

void Head::build(ID3D11Device *device) {
    for (const auto &mesh : m_Model.meshes) {
        for (const auto &material : m_Model.materials) {
            const auto &glTFTex = m_Model.textures[material.emissiveTexture.index];
            const auto &glTFImage = m_Model.images[glTFTex.source];

            D3D11_TEXTURE2D_DESC desc = {};
            desc.Width = glTFImage.width;
            desc.Height = glTFImage.height;
            desc.MipLevels = 1;
            desc.ArraySize = 1;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            desc.Usage = D3D11_USAGE_DEFAULT;

            D3D11_SUBRESOURCE_DATA initData = {};
            initData.pSysMem = glTFImage.image.data();
            initData.SysMemPitch = glTFImage.width * glTFImage.component;

//            ID3D11Texture2D *dxTex;
            HRESULT res = device->CreateTexture2D(&desc, &initData, &tex);
            assert(SUCCEEDED(res));

            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            srvDesc.Format = desc.Format;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = 1;

            res = device->CreateShaderResourceView(tex, &srvDesc, &srv);
            assert(SUCCEEDED(res));
        }

        for (const auto &primitive : mesh.primitives) {
            if (primitive.indices > -1) {
                const auto &accessor = m_Model.accessors[primitive.indices];
                indices = m_CreateBuffer(device, D3D11_BIND_INDEX_BUFFER, accessor);
                indexCount = accessor.count;
            }

            for (const auto &[attributeName, accessorIndex] : primitive.attributes) {
                const tinygltf::Accessor &accessor = m_Model.accessors[accessorIndex];
                if (accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT) {
                    MessageBox(nullptr, "Unsupported glTF accessor type!", "Error", MB_OK | MB_ICONERROR);
                    exit(-1);
                }

                buffers[attributeName] = m_CreateBuffer(device, D3D11_BIND_VERTEX_BUFFER, accessor);
            }
        }
    }
}

ID3D11Buffer *Head::m_CreateBuffer(ID3D11Device *device, D3D11_BIND_FLAG bindFlags, const tinygltf::Accessor &accessor) {
    const tinygltf::BufferView &bufferView = m_Model.bufferViews[accessor.bufferView];
    const tinygltf::Buffer &glTFBuffer = m_Model.buffers[bufferView.buffer];

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = accessor.count * tinygltf::GetNumComponentsInType(accessor.type) * tinygltf::GetComponentSizeInBytes(accessor.componentType);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = bindFlags;

    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem = &glTFBuffer.data[bufferView.byteOffset + accessor.byteOffset];

    ID3D11Buffer *dxBuffer;
    HRESULT hr = device->CreateBuffer(
            &bufferDesc,
            &data,
            &dxBuffer);

    assert(SUCCEEDED(hr));
    return dxBuffer;
}
