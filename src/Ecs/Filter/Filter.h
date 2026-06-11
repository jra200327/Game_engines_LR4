#ifndef FILTER_H
#define FILTER_H

#include <memory>
#include <utility>
#include <vector>

#include "../Components/BaseComponentStorage.h"
#include "../World/World.h"

class Filter {
    World& _world;
    std::vector<std::shared_ptr<BaseComponentStorage>> _componentStorages;
    size_t _minStorageIndex = -1;

    size_t FindMinComponentStorage() const;

public:
    Filter(World& world,
        const std::vector<std::shared_ptr<BaseComponentStorage>>& componentStorages);
    bool Empty();

    class Iterator
    {
        World& _world;
        const std::span<const int> _minStorageEntities;
        const std::vector<std::shared_ptr<BaseComponentStorage>>& _storages;
        const size_t _minStorageIndex;
        int _currentEntity;

        size_t _current;

        bool HasAllComponents() const;

        Iterator& Increment();

    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = int;
        using difference_type = std::ptrdiff_t;

        Iterator(World& world,
            const std::vector<std::shared_ptr<BaseComponentStorage>>& storages,
            const std::span<const int>& minStorageEntities, const size_t minStorageIndex,
            const size_t current);

        value_type operator*() const;

        Iterator& operator++();

        bool operator!=(const Iterator& other) const;
        bool operator==(const Iterator& other) const;
    };

    Iterator begin();

    Iterator end();
};

#endif //FILTER_H
