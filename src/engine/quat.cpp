#include "quat.h"
#include "mat4.h"
#include "vec3.h"

Mat4 Quat::toMat4() const 
{ 
    return Mat4(glm::toMat4(data)); 
}

void Quat::lookAt(const Vec3& position, const Vec3& target, const Vec3& up)
{
    data = glm::quat_cast(glm::lookAt(position.getData(), target.getData(), up.getData()));
}

void Quat::rotateAboutAxisEuler(const Vec3& axis, float angleDegrees)
{ 
    data *= glm::angleAxis(glm::radians(angleDegrees), glm::normalize(axis.getData()));
}

Vec3 Quat::toEuler(const Quat& quat)
{
    return Vec3(glm::degrees(glm::eulerAngles(quat.getData())));
}

Quat Quat::fromEuler(const Vec3& eulerRotationDegrees)
{
    return glm::quat(glm::radians(eulerRotationDegrees.getData()));
}