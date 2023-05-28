#include "objectTransform.h"

#include <iostream>

namespace ignis_engine {
    Transform::~Transform()
    {
        
    }

    void Transform::start()
    {
        std::cout << "Transform start" << std::endl;
    }

    void Transform::update(float dt) {
        
    }
}