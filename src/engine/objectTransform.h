#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "component.h"
#include "vec3.h"


class ObjectTransform : public Component {
public:
    ObjectTransform();
    ~ObjectTransform();

    virtual void start() override;
    virtual void update(float dt) override;

    void setPosition(Vec3 position);
    void translate(Vec3 translation);
    void setScale(Vec3 scale);
    void rotateAround(Vec3& axis, float angleDegrees);
    void lookAt(Vec3 target, Vec3 up);

    void addChildTransform(ObjectTransform& transform);
    void removeChildTransform(ObjectTransform& transform);
    std::vector<ObjectTransform*>& getChildTransforms() { return childTransforms; };

    const glm::mat4& getMatrix() const { return globalMatrix; };
    void setParentMatrix(glm::mat4& newParentMatrix);
    void updateChildTransforms();

    Vec3 getPosition() { return Vec3(position.x, position.y, position.z); };

    virtual std::vector<FieldDescription>& getFields() override { return fields; };

    ObjectTransform* parentTransform;

    void updateMatrix();
private:

    glm::mat4 globalMatrix = glm::mat4(1.0f);
    
    glm::vec3 position = glm::vec3(0.0f);
    glm::quat rotation;
    glm::vec3 scale = glm::vec3(1.0f);
    glm::mat4 parentMatrix = glm::mat4(1.0f);

    std::vector<FieldDescription> fields = {
        { GET_NAME(ObjectTransform), FieldType::ComponentType, this },
        { GET_NAME(position), FieldType::vec3, &position, this }, // TODO: this is wrong for camera, I think should be inverse b/c goes in wrong dir
    };

    std::vector<ObjectTransform*> childTransforms;
};