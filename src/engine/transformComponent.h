#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "component.h"
#include "vec3.h"
#include "quat.h"

// TODO: rename TransformComponent to match others' convention
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
    void lookAt(const Vec3& target, const Vec3& up = Vec3::UP);

    void addChildTransform(TransformComponent* transform);
    void removeChildTransform(TransformComponent* transform);
    std::vector<TransformComponent*>& getChildTransforms() { return childTransforms; };

    const glm::mat4& getMatrix() const { return globalMatrix; };
    void setParentMatrix(glm::mat4& newParentMatrix);
    void updateChildTransforms();

    Vec3 getPosition() { return Vec3(position.x, position.y, position.z); };
    Quat getRotation() { return Quat(rotation); };

    virtual std::vector<FieldDescription>& getFields() override { return fields; };
    virtual enum ComponentType getType() override { return ComponentType::TRANSFORM; };

    TransformComponent* parentTransform = nullptr;

    void updateMatrix();
private:
    // Could potentially wrap these all in my own type. Would need to make sure it's as performant as POD types first though, but would be nicer
    glm::mat4 globalMatrix = glm::mat4(1.0f);
    
    glm::vec3 position = glm::vec3(0.0f);
    glm::quat rotation;
    glm::vec3 scale = glm::vec3(1.0f);
    glm::mat4 parentMatrix = glm::mat4(1.0f);

    glm::vec3 guiEulerAngles = glm::vec3(0.0f);
    std::vector<FieldDescription> fields = {
        { GET_NAME(position), FieldType::VEC3_FIELD, &position, [&]() { this->updateMatrix(); } },
        { GET_NAME(rotation), FieldType::VEC3_FIELD, &guiEulerAngles, [&]() { this->setEulerRotation(guiEulerAngles); } },
        { GET_NAME(scale), FieldType::VEC3_FIELD, &scale, [&]() { this->updateMatrix(); } },
    };

    std::vector<TransformComponent*> childTransforms;
};