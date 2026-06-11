#include "DefaultCameraSystem.h"

void DefaultCameraSystem::OnInit()
{
}

void DefaultCameraSystem::OnUpdate()
{
    for (const auto ent : _defaultCameras)
    {
        auto& camera = _cameraComponents.Get(ent);
        auto size = camera.View.getSize();
        size.x = size.x / 2;
        size.y = size.y / 2;
        camera.View.setCenter(size);
    }
}