#pragma once

#include <stdio.h>
#include <vector>
#include <glm/gtx/quaternion.hpp>
#include "serialization.h"

struct Quat {
public:
    // TODO: could potentially have fromEuler constructor
    Quat(glm::quat data) : data(data) { };

    const glm::quat& getData() const { return this->data; }

private:
    glm::quat data;
};