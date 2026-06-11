#include "World.h"

void World::EntityComponentsChanged(const int e, const int storageId, const bool added)
{
   auto& entity = _entities[e];

   if (e < 0 || e >= (int)_entities.size())
        return;

    if (e >= (int)_isRemoving.size())
        return;

    if (added)
    {
        entity.AddComponent(storageId);
    }
    else
    {
        const int newComponentsCount = entity.RemoveComponent(storageId);

        // ❗ ВАЖНО: не удаляем сразу!
        if (newComponentsCount == 0 && !_isRemoving[e])
        {
            _isRemoving[e] = true;
            _toRemove.push_back(e);
        }
    }
}

World::World()
{
    _entities.reserve(DefaultEntitiesCapacity);
    _isRemoving.reserve(DefaultEntitiesCapacity);
}

int World::CreateEntity()
{
    int entityId;
    if (!_freeEntities.empty()) {
        entityId = _freeEntities.back();
        _freeEntities.pop_back();
        _entities[entityId].Recycle();
    } else {
        entityId = _entities.size();
        _entities.emplace_back(entityId, 1);
        _isRemoving.push_back(false);
    }

    if (entityId >= (int)_isRemoving.size())
    {
        _isRemoving.resize(entityId + 1, false);
    }

    return entityId;
}

void World::RemoveEntity(int e)
{
    auto& entity = _entities[e];

    if (entity.IsRemoved())
        return;

    auto components = entity.Components();

    for (int i = components.size() - 1; i >= 0; --i)
    {
        _componentStorages[components[i]]->Remove(e);
    }

    entity.Remove();

    _freeEntities.push_back(entity.Id);
    _isRemoving[e] = false;
}

void World::Flush()
{
    for (int e : _toRemove)
    {
        RemoveEntity(e);
    }

    _toRemove.clear();
}

EntityId World::GetPackedEntity(const int e) const
{
    return _entities[e];
}

bool World::UnpackEntity(const EntityId &eId, int &e) const
{
    e = eId.Id;
    return IsEntityAlive(e) && eId.Gen() == _entities[e].Gen();
}

bool World::IsEntityAlive(const int e) const
{
    return e >= 0 &&
           e < (int)_entities.size() &&
           !_entities[e].IsRemoved();
}

void World::RestartWorld()
{
   for (int i = 0; i < (int)_entities.size(); ++i)
    {
        if (IsEntityAlive(i) && !_isRemoving[i])
        {
            std::cout << "Removing " << i << std::endl;

            _isRemoving[i] = true;
            _toRemove.push_back(i);
        }
    }
}