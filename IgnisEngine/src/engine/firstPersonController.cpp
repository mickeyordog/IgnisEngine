#include "firstPersonController.h"
#include "inputHandler.h"
#include "gameObject.cpp"

FirstPersonController::FirstPersonController(float movementSpeed, float rotationSpeed) : movementSpeed(movementSpeed), rotationSpeed(rotationSpeed)
{
    fields.insert(fields.begin(), Component::getFields().begin(), Component::getFields().end());
}

FirstPersonController::~FirstPersonController()
{

}

void FirstPersonController::start()
{ 

}

void FirstPersonController::update(float dt)
{
    Vec3 velocity;
    if (InputHandler::getInstance().keyPressed(SDL_SCANCODE_W)) {
        velocity.z() -= 1.0;
    }
    if (InputHandler::getInstance().keyPressed(SDL_SCANCODE_S))
    {
        velocity.z() += 1.0;
    }
    if (InputHandler::getInstance().keyPressed(SDL_SCANCODE_A))
    {
        velocity.x() -= 1.0;
    }
    if (InputHandler::getInstance().keyPressed(SDL_SCANCODE_D))
    {
        velocity.x() += 1.0;
    }
    if (velocity.x() != 0.0 || velocity.z() != 0.0) {
        velocity.applyRotation(gameObject->transform->getRotation());
        velocity.y() = 0.0;
        velocity.normalize();
        velocity *= dt * this->movementSpeed;
    }
    if (InputHandler::getInstance().keyPressed(SDL_SCANCODE_SPACE))
    {
        velocity.y() += dt * this->movementSpeed;
    }
    if (InputHandler::getInstance().keyPressed(SDL_SCANCODE_LSHIFT))
    {
        velocity.y() -= dt * this->movementSpeed;
    }
    gameObject->transform->translate(velocity);

    float rotationY = 0.0;
    if (InputHandler::getInstance().keyPressed(SDL_SCANCODE_LEFT))
    {
        rotationY += this->rotationSpeed * dt;
    }
    if (InputHandler::getInstance().keyPressed(SDL_SCANCODE_RIGHT))
    {
        rotationY -= this->rotationSpeed * dt;
    }
    rotationY -= InputHandler::getInstance().getMouseDelta().x() * this->rotationSpeed * MOUSE_SENSITIVITY * dt;
    gameObject->transform->rotateAround(Vec3::UP, rotationY);

    float rotationX = 0.0;
    if (InputHandler::getInstance().keyPressed(SDL_SCANCODE_UP))
    {
        rotationX += this->rotationSpeed * dt;
    }
    if (InputHandler::getInstance().keyPressed(SDL_SCANCODE_DOWN))
    {
        rotationX -= this->rotationSpeed * dt;
    }
    rotationX -= InputHandler::getInstance().getMouseDelta().y() * this->rotationSpeed * MOUSE_SENSITIVITY * dt;
    gameObject->transform->rotateAroundLocal(Vec3::RIGHT, rotationX);

}
