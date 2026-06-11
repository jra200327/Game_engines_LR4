#ifndef FOLLOWXCAMERASYSTEM_H
#define FOLLOWXCAMERASYSTEM_H

#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/FilterBuilder.h"

#include "../Components/CameraComponent.h"
#include "../Components/FollowXCameraTag.h"
#include "../Components/ShooterComponent.h"
#include "../Components/PositionComponent.h"

class FollowXCameraSystem final : public ISystem
{
    ComponentStorage<CameraComponent>& _cameraComponents;

    ComponentStorage<ShooterComponent>& _targets;

    ComponentStorage<PositionComponent>& _positions;

    Filter _followCameras;
    Filter _player;

public:
    FollowXCameraSystem(World& world): 
        ISystem(world),

        _cameraComponents(world.GetStorage<CameraComponent>()),
        _targets(world.GetStorage<ShooterComponent>()),
        _positions(world.GetStorage<PositionComponent>()),
        _followCameras(
            FilterBuilder(world)
                .With<CameraComponent>()
                .With<FollowXCameraTag>()
                .Build()),
        _player(
            FilterBuilder(world)
                .With<PositionComponent>()
                .With<ShooterComponent>()
                .Build())
    {
    }

    void OnUpdate() override;
    void OnInit() override;
};

#endif