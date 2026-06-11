#include "FilterBuilder.h"

Filter FilterBuilder::Build() const
{
    return Filter(_world, _componentStorages); 
}

FilterBuilder::FilterBuilder(World& world):
_world(world)
{
    _componentStorages.reserve(4);
}