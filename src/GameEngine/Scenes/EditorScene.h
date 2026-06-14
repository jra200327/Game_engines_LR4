#ifndef EDITORSCENE_H
#define EDITORSCENE_H

#include "../Scene.h"
#include <memory>

namespace Editor {
    class CameraService;
    class GameObjectFactory;
    class GameObjectController;
    class RenderSystem;
}

class EditorScene : public Scene
{
    std::shared_ptr<Editor::CameraService> _cameraService;
    std::shared_ptr<Editor::GameObjectFactory> _goFactory;
    std::shared_ptr<Editor::GameObjectController> _goController;
    std::shared_ptr<Editor::RenderSystem> _renderSystem;

public:
    EditorScene(GameEngine& engine);
    
    void Init() override;
    void Update(float delta) override;
    void Render() override;
};

#endif // EDITORSCENE_H