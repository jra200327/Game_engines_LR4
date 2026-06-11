#include "SystemsManager.h"

SystemsManager::SystemsManager(World& world) : _world(world) 
{
    
}

SystemsManager& SystemsManager::AddInitializer(std::shared_ptr<IInitializer> initializer)
{
    _newInitializers.push_back(initializer);
    return *this;
}

SystemsManager& SystemsManager::AddSystem(std::shared_ptr<ISystem> system)
{
    _newInitializers.push_back(system);
    _systems.push_back(system);
    return *this;
}

void SystemsManager::Initialize()
{
    if (_newInitializers.empty())
        return;
    for(int i = 0; i < _newInitializers.size(); i++)
    {
        _newInitializers[i]->OnInit();
        _initializers.push_back(_newInitializers[i]);
    }
    _newInitializers.clear();
}

void SystemsManager::Update()
{
    Initialize();
    for(int i = 0; i < _systems.size(); i++)
    {
        _systems[i]->OnUpdate();
    }
}

void SystemsManager::RestartSystems()
{
    if (_initializers.empty())
        return;
    for(int i = 0; i < _initializers.size(); i++)
    {
        _newInitializers.push_back(_initializers[i]);
    }
    _initializers.clear();
    Initialize();
}