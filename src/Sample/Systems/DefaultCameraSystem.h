#ifndef DEFAULTCAMERASYSTEM_H
#define DEFAULTCAMERASYSTEM_H

#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/FilterBuilder.h"

#include "../Components/CameraComponent.h"
#include "../Components/DefaultCameraTag.h"

class DefaultCameraSystem final : public ISystem
{
    ComponentStorage<CameraComponent>& _cameraComponents;

    Filter _defaultCameras;

public:
    DefaultCameraSystem(World& world)
        : ISystem(world),
          _cameraComponents(
              world.GetStorage<CameraComponent>()),

          _defaultCameras(
              FilterBuilder(world)
                  .With<CameraComponent>()
                  .With<DefaultCameraTag>()
                  .Build())
    {
    }

    void OnUpdate() override;
    void OnInit() override;
};

#endif