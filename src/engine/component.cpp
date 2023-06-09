#include "component.h"
#include "randomNumberGenerator.h"

Component::Component()
{
    // TODO: where do I want to generate/pass in ID?
    fileID = RandomNumberGenerator::getRandomInteger();
}
