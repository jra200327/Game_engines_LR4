#include "FollowXCameraSystem.h"

void FollowXCameraSystem::OnInit()
{
}

void FollowXCameraSystem::OnUpdate()
{
    for (const auto ent : _followCameras)
    {
        for (const auto pl : _player)
        {
            auto& camera = _cameraComponents.Get(ent);
            auto& player = _targets.Get(pl);
            auto& position = _positions.Get(pl);

            auto center = camera.View.getCenter();
            center.x = position.X;
            camera.View.setCenter(center);
        }
    }
}