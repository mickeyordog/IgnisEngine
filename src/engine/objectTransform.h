#pragma once

#include "component.h"
#include "vec3.h"

#include <stdio.h>

class ObjectTransform : public Component {
public:
    ObjectTransform();
    ~ObjectTransform();

    virtual void start() override;
    virtual void update(float dt) override;

    // Vec3 position;
    // Vec3 rotation;
    // Vec3 scale;

    void translate(Vec3 translation);
    void scale(Vec3& scale);
    void rotateAround(Vec3& axis, float angleDegrees);
    void lookAt(Vec3 target, Vec3 up);

    void addChildTransform(ObjectTransform& transform);
    void removeChildTransform(ObjectTransform& transform);
    std::vector<ObjectTransform*>& getChildTransforms() { return childTransforms; };

    const glm::mat4& getData() { return matrix; }

    virtual std::vector<FieldDescription>& getFields() override { return fields; };

    ObjectTransform* parentTransform;

private:
    glm::mat4 matrix = glm::mat4(1.0f);
    std::vector<FieldDescription> fields = {
        { GET_NAME(ObjectTransform), FieldType::ComponentType, this },
        { "x", FieldType::Float, &matrix[3][0]}, // TODO: this is wrong, I think should be inverse b/c goes in wrong dir
        { "y", FieldType::Float, &matrix[3][1]},
        { "z", FieldType::Float, &matrix[3][2]}
    };

    std::vector<ObjectTransform*> childTransforms;
};