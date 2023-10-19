//
// Created by Jean on 9/18/2023.
//

#pragma once

#include "AObject.h"

class Triangle: public AObject {
public:
    explicit Triangle(ID3D11Device *device);
    Triangle(const Triangle&) = delete;

    void build(ID3D11Device *device) override;
};
