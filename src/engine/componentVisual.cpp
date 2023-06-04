#include "componentVisual.h"

ComponentVisual::ComponentVisual()
{
    fields.insert(fields.begin(), Component::getFields().begin(), Component::getFields().end());
}