#ifndef COMPONENTSTORAGE_H
#define COMPONENTSTORAGE_H

#include <vector>

#include "BaseComponentStorage.h"
#include "../World/IWorldInternal.h"

template <typename T>
class ComponentStorage final : public BaseComponentStorage {
    internal::IWorldInternal& _world;

    std::vector<T> _data;      // Для плотного хранения данных
    std::vector<int> _sparse;  // Для разреженного хранения индексов элементов (сущностей)
    std::vector<int> _dense;   // Для плотного хранения реально существующих элементов (сущностей, на которых есть компоненты)
    
    int _count = 0;            // Текущее число элементов

    int _id;

    void Resize(const int sparseSize, const int dataSize)
    {
        int oldSparseSize = _sparse.size();
        if (oldSparseSize < sparseSize) 
        {
            _sparse.resize(sparseSize);
            std::fill_n(_sparse.data() + oldSparseSize,
            sparseSize - oldSparseSize, -1);
        }
        int oldDataSize = _dense.size();
        if (oldDataSize < dataSize) 
        {
            _dense.resize(dataSize);
            _data.resize(dataSize);
            std::fill_n(_dense.data() + oldDataSize,
            dataSize - oldDataSize, -1);
        }
    }

public:
    // ToDo:
    ComponentStorage(internal::IWorldInternal& w, const int id): 
    _world(w), _id(id), _count(0)
    {
        _sparse.resize(50, -1);
        _dense.resize(50, -1);
        _data.resize(50);
    }

    bool Has(const int e) const override
    {
        return e < _sparse.size() && _sparse[e]!= -1; 
    }

    T& Get(const int e)
    {
        return _data[_sparse[e]];
    }

    T& Add(const int e, const T& value)
    {
        Resize((e / 64 + 1) * 64, _data.size() == _count + 1 ? _data.size() + 64 :
        _data.size());
        _data[_count] = value;
        _dense[_count] = e;
        _sparse[e] = _count;
        _count++;
        _world.EntityComponentsChanged(e, _id, true);
        return _data[_sparse[e]];
    }

    void Remove(const int e) override
    {
        if (e >= _sparse.size() || _sparse[e] == -1)
            return;

        int removeIndex = _sparse[e];
        int lastIndex = --_count;

        if (removeIndex != lastIndex)
        {
            _data[removeIndex] = _data[lastIndex];

            int lastEntity = _dense[lastIndex];
            _dense[removeIndex] = lastEntity;
            _sparse[lastEntity] = removeIndex;
        }

        _sparse[e] = -1;

        _world.EntityComponentsChanged(e, _id, false);
    }

    // ToDo: возврат всех компонентов данного типа
    std::span<const T> All() const
    {
        return std::span(_data.begin(), _count);
    }
    // ToDo: возврат всех сущностей с компонентом данного типа
    std::span<const int> Entities() const override
    {
         return std::span(_dense.begin(), _count);
    }

    // ToDo:
    int Count() const override
    {   
        return _count;
    }

    int Id() const override
    {
        return _id;
    }
};

#endif //COMPONENTSTORAGE_H
