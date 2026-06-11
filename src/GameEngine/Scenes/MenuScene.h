#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "../Scene.h"

#include "../../UI/Button.h"
#include "../../UI/Text.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Sample/Components/CameraComponent.h"
#include "../../Sample/Components/DefaultCameraTag.h"


class MenuScene : public Scene
{
    std::shared_ptr<Button> _playButton;
    std::shared_ptr<Button> _exitButton;

    std::shared_ptr<Text> _title;
    std::shared_ptr<InputAction> _mouseClickAction;
    std::shared_ptr<InputAction> _mouseMoveAction;

    Filter _camera;
    ComponentStorage<CameraComponent>& _cameraComponents;

public:
    MenuScene(GameEngine& engine): 
        Scene(engine),
        _cameraComponents(world.GetStorage<CameraComponent>()),
        _camera(FilterBuilder(world).With<CameraComponent>().With<DefaultCameraTag>().Build())
    {}

    void Init() override;
    void Update(float delta) override;
    void Render() override;
};

#endif // MENUSCENE_H
