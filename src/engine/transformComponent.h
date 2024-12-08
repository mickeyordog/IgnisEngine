#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "component.h"
#include "vec3.h"
#include "quat.h"
#include "mat4.h"

class TransformComponent : public Component {
public:
    TransformComponent();
    ~TransformComponent();

    virtual void start() override;
    virtual void update(float dt) override;

    void setPosition(const Vec3& position);
    void translate(const Vec3& translation);
    void translateLocal(const Vec3& translation);
    void setScale(const Vec3& scale);
    void setEulerRotation(const Vec3& eulerRotation);
    void rotateAround(const Vec3& axis, float angleDegrees);
    void rotateAroundLocal(const Vec3& axis, float angleDegrees);
    void lookAt(const Vec3& target, const Vec3& up = Vec3::UP);

    void addChildTransform(TransformComponent* transform);
    void removeChildTransform(TransformComponent* transform);
    std::vector<TransformComponent*>& getChildTransforms() { return childTransforms; };

    const Mat4& getMatrix() const { return globalMatrix; };
    void setParentMatrix(const Mat4& newParentMatrix);
    void updateChildTransforms();

    const Vec3& getPosition() { return position; };
    // These 3 methods could potentially be faster by just getting the columns
    Vec3 getLocalForward() { return globalMatrix.getLocalForward(); };
    Vec3 getLocalRight() { return globalMatrix.getLocalRight(); };
    Vec3 getLocalUp() { return globalMatrix.getLocalUp(); };
    const Quat& getRotation() { return rotation; };

    virtual std::vector<FieldDescription>& getFields() override { return fields; };
    virtual enum ComponentType getType() override { return ComponentType::TRANSFORM; };

    TransformComponent* parentTransform = nullptr;

    void updateMatrix();
private:
    Mat4 globalMatrix = Mat4(1.0f);
    
    Vec3 position = Vec3(0.0f);
    Quat rotation;
    Vec3 scale = Vec3(1.0f);
    Mat4 parentMatrix = Mat4(1.0f);

    Vec3 guiEulerAngles = Vec3(0.0f);
    std::vector<FieldDescription> fields = {
        { GET_NAME(position), FieldType::VEC3_FIELD, &position, [&]() { this->updateMatrix(); } },
        { GET_NAME(rotation), FieldType::VEC3_FIELD, &guiEulerAngles, [&]() { this->setEulerRotation(guiEulerAngles); } },
        { GET_NAME(scale), FieldType::VEC3_FIELD, &scale, [&]() { this->updateMatrix(); } },
    };

    std::vector<TransformComponent*> childTransforms;
};