#include "GuiSystem.h"

#include <imgui-SFML.h>
#include <imgui.h>
#include <SFML/Graphics/Texture.hpp>
#include <fstream>
#include <iostream>

#include "../../../Ecs/Filter/FilterBuilder.h"
#include "../Components/TransformComponent.h"
#include "../Components/BBoxComponent.h"
#include "../Components/EditorComponent.h"

namespace Editor {

    GuiSystem::GuiSystem(World& world,
                         sf::RenderWindow& window,
                         const AssetManager& assets,
                         std::shared_ptr<GameObjectFactory> goFactory,
                         std::shared_ptr<CameraService> cameraService)
        : ISystem(world),
          _window(window),
          _assets(assets),
          _goFactory(goFactory),
          _cameraService(cameraService)
    {
        // Инициализируем маппинг текстур → имена объектов для level.txt
        
        // Игрок
        _textureToObjectName["TexRun"] = "Player";
        _textureToObjectName["TexIdle"] = "Player";
        _textureToObjectName["TexJump"] = "Player";
        
        // Тайлы
        _textureToObjectName["Tile"] = "Tile";
        _textureToObjectName["Tile1"] = "Tile";
        _textureToObjectName["Brick_Tile"] = "Brick_Tile";
        _textureToObjectName["Question_Tile"] = "Question_Tile";
        _textureToObjectName["Question_Inactive_Tile"] = "Question_Inactive_Tile";
        
        // Трубы
        _textureToObjectName["Pipe_Up_R"] = "Pipe1";
        _textureToObjectName["Pipe_Up_L"] = "Pipe2";
        _textureToObjectName["Pipe_L"] = "Pipe_L";
        _textureToObjectName["Pipe_R"] = "Pipe_R";
        
        // Финиш
        _textureToObjectName["Finish"] = "Finish";
        
        // Декорации
        _textureToObjectName["BigHill"] = "BigHill";
        _textureToObjectName["SmallHill"] = "SmallHill";
        
        // Враги
        _textureToObjectName["TexGoombaMove"] = "Goomba";
    }

    void GuiSystem::OnInit()
    {
    }

    void GuiSystem::SaveScene()
    {
        auto& transformStorage = world.GetStorage<TransformComponent>();
        auto& editorStorage = world.GetStorage<EditorComponent>();
        
        Filter editorObjects(FilterBuilder(world)
            .With<TransformComponent>()
            .With<EditorComponent>()
            .Build());
        
        std::vector<std::string> lines;
        lines.push_back("# ===== ОБЪЕКТЫ УРОВНЯ =====");
        lines.push_back("# name x y");
        
        int objectCount = 0;
        
        for (auto ent : editorObjects)
        {
            auto& transform = transformStorage.Get(ent);
            auto& editor = editorStorage.Get(ent);
            
            std::string objectName;
            auto it = _textureToObjectName.find(editor.AssetName);
            if (it != _textureToObjectName.end())
            {
                objectName = it->second;
            }
            else
            {
                objectName = editor.AssetName;
            }
            
            int gridX = static_cast<int>(transform.Position.x / 64.0f);
            int gridY = static_cast<int>(transform.Position.y / 64.0f);
            
            lines.push_back(objectName + " " + std::to_string(gridX) + " " + std::to_string(gridY));
            
            std::cout << "[Editor] Save: " << objectName << " at grid(" 
                      << gridX << ", " << gridY << ") world(" 
                      << transform.Position.x << ", " << transform.Position.y << ")\n";
            
            objectCount++;
        }
        
        std::string filePath = "D:/work/Game_engines_LR4/src/Configs/level.txt";
        std::ofstream file(filePath);
        if (!file.is_open())
        {
            std::cerr << "[Editor] ERROR: Failed to open file for writing: " << filePath << "\n";
            return;
        }
        
        for (const auto& line : lines)
        {
            file << line << "\n";
        }
        
        file.close();
        std::cout << "[Editor] Scene saved to " << filePath << " (" << objectCount << " objects)\n";
    }

    void GuiSystem::OnUpdate()
    {
        ImGui::Begin("Editor Assets");

        if (ImGui::CollapsingHeader("Textures", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const auto& textures = _assets.GetTextures();
            
            int columns = 4;
            if (ImGui::BeginTable("TexturesTable", columns, ImGuiTableFlags_None))
            {
                int i = 0;
                for (const auto& [name, texture] : textures)
                {
                    if (i % columns == 0)
                        ImGui::TableNextRow();
                    
                    ImGui::TableSetColumnIndex(i % columns);
                    
                    ImGui::PushID(name.c_str());

                    ImTextureID texId = static_cast<ImTextureID>(texture.getNativeHandle());
                    
                    if (ImGui::ImageButton(name.c_str(), texId, ImVec2(BTN_SIZE.x, BTN_SIZE.y)))
                    {
                        sf::Vector2f viewCenter = _cameraService->GetView().getCenter();
                        _goFactory->CreateFromTexture(name, viewCenter);
                        std::cout << "[Editor] Created object: " << name << " at (" 
                                  << viewCenter.x << ", " << viewCenter.y << ")\n";
                    }

                    if (ImGui::IsItemHovered())
                    {
                        ImGui::BeginTooltip();
                        ImGui::Text("%s", name.c_str());
                        ImGui::Text("%dx%d", texture.getSize().x, texture.getSize().y);
                        ImGui::EndTooltip();
                    }

                    std::string displayName = name;
                    if (displayName.length() > 12)
                        displayName = displayName.substr(0, 10) + "..";
                    ImGui::Text("%s", displayName.c_str());

                    ImGui::PopID();
                    i++;
                }
                
                ImGui::EndTable();
            }
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::Button("Save Scene", ImVec2(200, 40)))
        {
            SaveScene();
        }

        ImGui::End();
    }

}