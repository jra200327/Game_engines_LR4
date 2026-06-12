set(TARGET_NAME main)
set(SOURCES main.cpp
        Sample/Systems/MovementSystem.cpp
        Sample/Systems/MovementSystem.h
        Sample/Systems/InitSystem.cpp
        Sample/Systems/InitSystem.h
        Sample/Components/PositionComponent.h
        Sample/Components/MovementComponent.h
        Sample/Components/BoxColliderComponent.h
        Sample/Components/CircleColliderComponent.h
        Sample/Components/CollisionComponent.h
        Sample/Components/SpriteComponent.h
        Sample/Components/ShooterComponent.h
        Sample/Components/BulletComponent.h
        Sample/Components/GravityComponent.h
        Sample/Components/CameraComponent.h
        Sample/Components/DefaultCameraTag.h
        Sample/Components/FollowXCameraTag.h
        Sample/Components/ExplosionTag.h
        Sample/Components/AnimationComponent.h
        Sample/Components/JumpComponent.h
        Sample/Components/ObjectComponent.h
        Sample/Components/EnemyComponent.h
        Sample/Components/FinishTag.h
        Sample/Systems/InputSystem.cpp
        Sample/Systems/InputSystem.h
        Sample/Systems/CollisionSystem.cpp
        Sample/Systems/CollisionSystem.h
        Sample/Systems/CollisionResolveSystem.cpp
        Sample/Systems/CollisionResolveSystem.h
        Sample/Systems/RenderSystem.cpp
        Sample/Systems/RenderSystem.h
        Sample/Systems/ShootingSystem.cpp
        Sample/Systems/ShootingSystem.h
        Sample/Systems/EntityFactory.cpp
        Sample/Systems/EntityFactory.h
        Sample/Systems/DefaultCameraSystem.h
        Sample/Systems/DefaultCameraSystem.cpp
        Sample/Systems/FollowXCameraSystem.h
        Sample/Systems/FollowXCameraSystem.cpp
        Sample/Systems/AnimationSystem.h
        Sample/Systems/AnimationSystem.cpp
        Sample/Systems/PlayerAnimationSystem.h
        Sample/Systems/PlayerAnimationSystem.cpp
        Sample/Systems/PlayerRespawnSystem.h
        Sample/Systems/PlayerRespawnSystem.cpp
        Sample/Systems/CleanerSystem.cpp
        Sample/Systems/CleanerSystem.h
        Sample/Components/MoveInputEvent.h
        Sample/Components/ShootEvent.h
        GameEngine/GameEngine.h
        GameEngine/GameEngine.cpp
        GameEngine/GameEngineConfiguration.h
        GameEngine/Input/InputManager.cpp
        GameEngine/Input/InputAction.h
        GameEngine/Scene.cpp
        GameEngine/Scenes/MenuScene.h
        GameEngine/Scenes/MenuScene.cpp
        GameEngine/Scenes/GameScene.h
        GameEngine/Scenes/GameScene.cpp
        GameEngine/Assets/AssetManager.cpp
        GameEngine/Assets/Animation.h
        GameEngine/Assets/Animation.cpp
        GameEngine/Grid/Grid.cpp
        GameEngine/ColliderDebug/ColliderDebug.h
        GameEngine/ColliderDebug/ColliderDebug.cpp
        UI/Text.cpp
        UI/Button.cpp
)

add_subdirectory(Ecs)

add_executable(${TARGET_NAME} 
    ${SOURCES})

#target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

target_link_libraries(${TARGET_NAME} PUBLIC easy_ecs)
target_link_libraries(${TARGET_NAME} PUBLIC ImGui-SFML::ImGui-SFML)

target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Ecs
    ${CMAKE_CURRENT_SOURCE_DIR}/Sample
    ${CMAKE_CURRENT_SOURCE_DIR}/Sample/Systems
    ${CMAKE_CURRENT_SOURCE_DIR}/Sample/Components

    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/Input
    ${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/Scenes

    ${CMAKE_CURRENT_SOURCE_DIR}/UI
)

# Copy resources to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Images DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Configs 
     DESTINATION ${CMAKE_BINARY_DIR}
     FILES_MATCHING PATTERN "*")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets DESTINATION ${CMAKE_BINARY_DIR})

# Set working directory for debugging
set_target_properties(${TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
