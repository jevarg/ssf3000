//
// Created by Jean on 9/19/2023.
//

#pragma once

#include "AObject.h"

class Cube: public AObject {
public:
    explicit Cube(ID3D11Device *device);
    Cube(const Cube&) = delete;
};
