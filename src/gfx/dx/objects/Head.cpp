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
        for (const auto &primitive : mesh.primitives) {
            if (primitive.indices > -1) {
                const auto &accessor = m_Model.accessors[primitive.indices];
                const tinygltf::BufferView &bufferView = m_Model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer &buffer = m_Model.buffers[bufferView.buffer];

                D3D11_BUFFER_DESC bufferDesc = {};
                bufferDesc.ByteWidth = accessor.count * tinygltf::GetNumComponentsInType(accessor.type) * tinygltf::GetComponentSizeInBytes(accessor.componentType); // TODO: determine stride w/ posAccessor.componentType
                bufferDesc.Usage = D3D11_USAGE_DEFAULT;
                bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

                D3D11_SUBRESOURCE_DATA data = {};
                data.pSysMem = &buffer.data[bufferView.byteOffset + accessor.byteOffset];

                HRESULT hr = device->CreateBuffer(
                        &bufferDesc,
                        &data,
                        &indices);

                assert(SUCCEEDED(hr));

                indexCount = accessor.count;
            }

            for (const auto &[attributeName, accessorIndex] : primitive.attributes) {
                const tinygltf::Accessor &accessor = m_Model.accessors[accessorIndex];
                if (accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT) {
                    MessageBox(nullptr, "Unsupported glTF accessor type!", "Error", MB_OK | MB_ICONERROR);
                    exit(-1);
                }

                const tinygltf::BufferView &bufferView = m_Model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer &buffer = m_Model.buffers[bufferView.buffer];

                D3D11_BUFFER_DESC bufferDesc = {};
                bufferDesc.ByteWidth = tinygltf::GetComponentSizeInBytes(accessor.componentType) * accessor.count * tinygltf::GetNumComponentsInType(accessor.type);
                bufferDesc.Usage = D3D11_USAGE_DEFAULT;
                bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

                D3D11_SUBRESOURCE_DATA data = {};
                data.pSysMem = &buffer.data[bufferView.byteOffset + accessor.byteOffset];

                ID3D11Buffer *dxBuffer;
                HRESULT hr = device->CreateBuffer(
                        &bufferDesc,
                        &data,
                        &dxBuffer);

                assert(SUCCEEDED(hr));
                buffers[attributeName] = dxBuffer;
            }
        }
    }
}
