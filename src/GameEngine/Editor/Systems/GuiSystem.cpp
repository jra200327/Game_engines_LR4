#include "GuiSystem.h"

#include <imgui-SFML.h>
#include <imgui.h>
#include <SFML/Graphics/Texture.hpp>
#include <fstream>
#include <iostream>

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
        _textureToObjectName["Player"] = "Player";
        
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
        _textureToObjectName["Goomba"] = "Goomba";
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
        
        std::string filePath = "../../Configs/level.txt";
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

        std::string patrolsfilePath = "../../Configs/enemyPatrol.txt";
        std::ofstream patrolsfile(patrolsfilePath);

        if (!patrolsfile.is_open())
        {
            std::cerr << "[Editor] ERROR: cannot open file: " << patrolsfilePath << "\n";
            return;
        }

        for (const auto& p : _enemyPatrols)
        {
            patrolsfile
                << p.patrolA.x << " "
                << p.patrolA.y << " "
                << p.patrolB.x << " "
                << p.patrolB.y
                << "\n";
        }

        patrolsfile.close();

        std::cout << "[Editor] Patrols saved to " << patrolsfilePath
                << " (" << _enemyPatrols.size() << " entries)\n";
                
    }

    void GuiSystem::OnUpdate()
    {   
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(400, 600), ImGuiCond_Always);
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

                        if(name == "Goomba")
                        {
                            EnemyPatrolData patrol;

                            auto gridPos = sf::Vector2i(
                                static_cast<int>(viewCenter.x / 64.f),
                                static_cast<int>(viewCenter.y / 64.f));

                            patrol.patrolA = gridPos;
                            patrol.patrolB = gridPos;

                            _enemyPatrols.push_back(patrol);
                        }
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

        ImGui::SetNextWindowPos(ImVec2(420, 10), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Always);
        ImGui::Begin("Enemy Patrols");
        
            for (size_t i = 0; i < _enemyPatrols.size(); ++i)
            {
                auto& patrol = _enemyPatrols[i];

                ImGui::PushID(static_cast<int>(i));

                if (ImGui::TreeNode(
                    ("Goomba #" + std::to_string(i)).c_str()))
                {
                    ImGui::InputInt("A X", &patrol.patrolA.x);
                    ImGui::InputInt("A Y", &patrol.patrolA.y);

                    ImGui::InputInt("B X", &patrol.patrolB.x);
                    ImGui::InputInt("B Y", &patrol.patrolB.y);

                    ImGui::TreePop();
                }

                ImGui::PopID();
            }
            ImGui::End();
        
    }

}