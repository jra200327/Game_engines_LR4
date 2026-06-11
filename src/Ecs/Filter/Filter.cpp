#include "Filter.h"

Filter::Filter(World& world, const std::vector<std::shared_ptr<BaseComponentStorage>>& componentStorages):
_world(world), _componentStorages(componentStorages)
{
    _minStorageIndex = FindMinComponentStorage();
}

size_t Filter::FindMinComponentStorage() const 
{
    size_t minIndex = 0;
    size_t minSize = std::numeric_limits<size_t>::max();

    for (size_t i = 0; i < _componentStorages.size(); ++i)
    {
        size_t size = _componentStorages[i]->Count();

        if (size < minSize)
        {
            minSize = size;
            minIndex = i;
        }
    }

    return minIndex;
}


Filter::Iterator::Iterator(World& world,
    const std::vector<std::shared_ptr<BaseComponentStorage>>& storages,
    const std::span<const int>& minStorageEntities,
    const size_t minStorageIndex,
    const size_t current)
    : _world(world),
      _storages(storages),
      _minStorageEntities(minStorageEntities),
      _minStorageIndex(minStorageIndex),
      _current(current)
{
    if (_current < _minStorageEntities.size())
        _currentEntity = _minStorageEntities[_current];

    if (_current < _minStorageEntities.size() && !HasAllComponents())
        Increment();
}

bool Filter::Iterator::HasAllComponents() const
{
    for (size_t i = 0; i < _storages.size(); ++i)
    {
        if (i == _minStorageIndex)
            continue;

        if (!_storages[i]->Has(_currentEntity))
            return false;
    }

    return true;
}

Filter::Iterator& Filter::Iterator::Increment()
{
    while (true)
    {
        ++_current;

        if (_current >= _minStorageEntities.size())
            break;

        _currentEntity = _minStorageEntities[_current];

        if (HasAllComponents())
            break;
    }

    return *this;
}

Filter::Iterator::value_type Filter::Iterator::operator*() const
{
    return _currentEntity;
}

Filter::Iterator& Filter::Iterator::operator++()
{
    return Increment();
}

bool Filter::Iterator::operator!=(const Iterator& other) const
{
    return _current != other._current;
}

bool Filter::Iterator::operator==(const Iterator& other) const
{
    return _current == other._current;
}

Filter::Iterator Filter::begin()
{
    const auto& minStorage = _componentStorages[_minStorageIndex];
    const auto& entities = minStorage->Entities();

    return Iterator(_world,
        _componentStorages,
        entities,
        _minStorageIndex,
        0);
}

Filter::Iterator Filter::end()
{
    const auto& minStorage = _componentStorages[_minStorageIndex];
    const auto& entities = minStorage->Entities();

    return Iterator(_world,
        _componentStorages,
        entities,
        _minStorageIndex,
        entities.size());
}