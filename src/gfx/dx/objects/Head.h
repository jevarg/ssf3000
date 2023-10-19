//
// Created by Jean on 10/16/2023.
//

#pragma once

#include <tiny_gltf.h>
#include "AObject.h"

class Head: public AObject {
public:
    explicit Head();
    Head(const Head&) = delete;

    void build(ID3D11Device *device) override;
    std::map<std::string, ID3D11Buffer *> buffers;
    ID3D11Buffer *indices;
    uint32_t indexCount;

private:
    tinygltf::Model m_Model;
};
