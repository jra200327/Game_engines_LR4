#include "EntityFactory.h"

#include "../Components/PositionComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CircleColliderComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/ShooterComponent.h"
#include "../Components/BulletComponent.h"
#include "../Components/AsteroidComponent.h"
#include "../Components/GravityComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/DefaultCameraTag.h"
#include "../Components/FollowXCameraTag.h"
#include "../Components/AnimationComponent.h"
#include "../Components/JumpComponent.h"
#include "../Components/ObjectComponent.h"
#include "../Components/ExplosionTag.h"
#include "../Components/FinishTag.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include <random>

void EntityFactory::CreateEntity(std::string name, sf::Vector2f pos)
{
    if (name == "Player")
    {
        const int player1 = _world.CreateEntity();
        auto& positionsStorage = _world.GetStorage<PositionComponent>();
        auto& movementsStorage = _world.GetStorage<MovementComponent>();
        auto& boxColliderStorage = _world.GetStorage<BoxColliderComponent>();
        auto& collisionStorage = _world.GetStorage<CollisionComponent>();
        auto& spriteStorage = _world.GetStorage<SpriteComponent>();
        auto& shooterStorage = _world.GetStorage<ShooterComponent>();
        auto& gravityStorage = _world.GetStorage<GravityComponent>();
        auto& animationStorage = _world.GetStorage<AnimationComponent>();
        auto& jumpStorage = _world.GetStorage<JumpComponent>();

        positionsStorage.Add(player1, PositionComponent(pos.x, pos.y));
        movementsStorage.Add(player1, MovementComponent(10, sf::Vector2f(0, 0)));
        boxColliderStorage.Add(player1, BoxColliderComponent(48, 56));
        collisionStorage.Add(player1, CollisionComponent());
        spriteStorage.Add(player1, SpriteComponent({24, 24}, {0, 0}, _assets.GetTexture(AssetNames::TexRun), 0.f, 2.f));
        shooterStorage.Add(player1, ShooterComponent(60));
        gravityStorage.Add(player1, GravityComponent());
        jumpStorage.Add(player1, JumpComponent(20));

        AnimationComponent anims;
        std::cout << "[DEBUG] idle tex addr = "
          << &_assets.GetTexture(AssetNames::TexIdle) << std::endl;

        Animation idleAnim = _assets.GetAnimation(AssetNames::IdleAnim);
        Animation runAnim = _assets.GetAnimation(AssetNames::RunAnim);
        Animation jumpAnim = _assets.GetAnimation(AssetNames::JumpAnim);
        Animation shootIdleAnim = _assets.GetAnimation(AssetNames::ShootIdleAnim);
        Animation shootRunAnim = _assets.GetAnimation(AssetNames::ShootRunAnim);
        Animation shootJumpAnim = _assets.GetAnimation(AssetNames::ShootJumpAnim);

        anims.Animations.emplace(AssetNames::IdleAnim, idleAnim);
        anims.Animations.emplace(AssetNames::RunAnim, runAnim);
        anims.Animations.emplace(AssetNames::JumpAnim, jumpAnim);
        anims.Animations.emplace(AssetNames::ShootIdleAnim, shootIdleAnim);
        anims.Animations.emplace(AssetNames::ShootRunAnim, shootRunAnim);
        anims.Animations.emplace(AssetNames::ShootJumpAnim, shootJumpAnim);

        anims.CurrentAnimation = AssetNames::IdleAnim;

        animationStorage.Add(player1, anims);

    }
    else if(name == "Bullet")
    {
        const int bullet = _world.CreateEntity();
        auto& positionsStorage = _world.GetStorage<PositionComponent>();
        auto& movementsStorage = _world.GetStorage<MovementComponent>();
        auto& boxColliderStorage = _world.GetStorage<BoxColliderComponent>();
        auto& collisionStorage = _world.GetStorage<CollisionComponent>();
        auto& spriteStorage = _world.GetStorage<SpriteComponent>();
        auto& bulletStorage = _world.GetStorage<BulletComponent>();

        auto& shooterStorage = _world.GetStorage<ShooterComponent>();
        Filter shooter(FilterBuilder(_world)
                .With<ShooterComponent>()
                .With<PositionComponent>()
                .Build());

        float xdir = 0;
        
        for (int ent : shooter)
        {
            auto& spr = spriteStorage.Get(ent);
            if(spr.flipped)
                xdir = -1;
            else
                xdir = 1;
        }
        
        positionsStorage.Add(bullet, PositionComponent(pos.x, pos.y));
        movementsStorage.Add(bullet, MovementComponent(20, sf::Vector2f(xdir, 0)));
        boxColliderStorage.Add(bullet, BoxColliderComponent(16, 16));
        collisionStorage.Add(bullet, CollisionComponent());
        bulletStorage.Add(bullet, BulletComponent());
        spriteStorage.Add(bullet, SpriteComponent({8, 8}, {0, 0}, _assets.GetTexture(AssetNames::Bullet), 0.f, 2.f));
    }
     else if (name == AssetNames::Tile)
    {
        int e = _world.CreateEntity();
        auto& p = _world.GetStorage<PositionComponent>();
        auto& s = _world.GetStorage<SpriteComponent>();
        auto& collisionStorage = _world.GetStorage<CollisionComponent>();
        auto& boxColliderStorage = _world.GetStorage<BoxColliderComponent>();
        auto& objectStorage = _world.GetStorage<ObjectComponent>();

        p.Add(e, PositionComponent(pos.x, pos.y));
        s.Add(e, SpriteComponent({64, 64}, {0, 0}, _assets.GetTexture(AssetNames::Tile), 0.f, 1.f));
        collisionStorage.Add(e, CollisionComponent());
        boxColliderStorage.Add(e, BoxColliderComponent(64, 64));
        objectStorage.Add(e, ObjectComponent(false));
    }
    else if (name == AssetNames::Tile1)
    {
        int e = _world.CreateEntity();
        auto& p = _world.GetStorage<PositionComponent>();
        auto& s = _world.GetStorage<SpriteComponent>();
        auto& collisionStorage = _world.GetStorage<CollisionComponent>();
        auto& boxColliderStorage = _world.GetStorage<BoxColliderComponent>();
        auto& objectStorage = _world.GetStorage<ObjectComponent>();

        p.Add(e, PositionComponent(pos.x, pos.y));
        s.Add(e, SpriteComponent({64, 64}, {0, 0}, _assets.GetTexture(AssetNames::Tile1), 0.f, 1.f));
        collisionStorage.Add(e, CollisionComponent());
        boxColliderStorage.Add(e, BoxColliderComponent(64, 64));
        objectStorage.Add(e, ObjectComponent(false));
    }
    else if (name == AssetNames::Brick_Tile)
    {
        int e = _world.CreateEntity();
        auto& p = _world.GetStorage<PositionComponent>();
        auto& s = _world.GetStorage<SpriteComponent>();
        auto& collisionStorage = _world.GetStorage<CollisionComponent>();
        auto& boxColliderStorage = _world.GetStorage<BoxColliderComponent>();
        auto& objectStorage = _world.GetStorage<ObjectComponent>();

        p.Add(e, PositionComponent(pos.x, pos.y));
        s.Add(e, SpriteComponent({64, 64}, {0, 0}, _assets.GetTexture(AssetNames::Brick_Tile), 0.f, 1.f));
        collisionStorage.Add(e, CollisionComponent());
        boxColliderStorage.Add(e, BoxColliderComponent(64, 64));
        objectStorage.Add(e, ObjectComponent(true));
    }
    else if (name == AssetNames::Question_Tile)
    {
        int e = _world.CreateEntity();
        auto& p = _world.GetStorage<PositionComponent>();
        auto& s = _world.GetStorage<SpriteComponent>();
        auto& collisionStorage = _world.GetStorage<CollisionComponent>();
        auto& boxColliderStorage = _world.GetStorage<BoxColliderComponent>();
        auto& objectStorage = _world.GetStorage<ObjectComponent>();

        p.Add(e, PositionComponent(pos.x, pos.y));
        s.Add(e, SpriteComponent({64, 64}, {0, 0}, _assets.GetTexture(AssetNames::Question_Tile), 0.f, 1.f));
        collisionStorage.Add(e, CollisionComponent());
        boxColliderStorage.Add(e, BoxColliderComponent(64, 64));
        objectStorage.Add(e, ObjectComponent(false));
    }
    else if (name == AssetNames::Question_Inactive_Tile)
    {
        int e = _world.CreateEntity();
        auto& p = _world.GetStorage<PositionComponent>();
        auto& s = _world.GetStorage<SpriteComponent>();
        auto& collisionStorage = _world.GetStorage<CollisionComponent>();
        auto& boxColliderStorage = _world.GetStorage<BoxColliderComponent>();
        auto& objectStorage = _world.GetStorage<ObjectComponent>();

        p.Add(e, PositionComponent(pos.x, pos.y));
        s.Add(e, SpriteComponent({64, 64}, {0, 0}, _assets.GetTexture(AssetNames::Question_Inactive_Tile), 0.f, 1.f));
        collisionStorage.Add(e, CollisionComponent());
        boxColliderStorage.Add(e, BoxColliderComponent(64, 64));
        objectStorage.Add(e, ObjectComponent(false));
    }
    else if (name == AssetNames::Pipe_Up_L)
    {
        int e = _world.CreateEntity();
        auto& p = _world.GetStorage<PositionComponent>();
        auto& s = _world.GetStorage<SpriteComponent>();
        auto& collisionStorage = _world.GetStorage<CollisionComponent>();
        auto& boxColliderStorage = _world.GetStorage<BoxColliderComponent>();
        auto& objectStorage = _world.GetStorage<ObjectComponent>();
        objectStorage.Add(e, ObjectComponent(false));

        p.Add(e, PositionComponent(pos.x, pos.y));
        s.Add(e, SpriteComponent({64, 64}, {0, 0}, _assets.GetTexture(AssetNames::Pipe_Up_L), 0.f, 1.f));
        collisionStorage.Add(e, CollisionComponent());
        boxColliderStorage.Add(e, BoxColliderComponent(64, 64));
        objectStorage.Add(e, ObjectComponent(false));
    }
    else if (name == AssetNames::Pipe_Up_R)
    {
        int e = _world.CreateEntity();
        auto& p = _world.GetStorage<PositionComponent>();
        auto& s = _world.GetStorage<SpriteComponent>();
        auto& collisionStorage = _world.GetStorage<CollisionComponent>();
        auto& boxColliderStorage = _world.GetStorage<BoxColliderComponent>();
        auto& objectStorage = _world.GetStorage<ObjectComponent>();

        p.Add(e, PositionComponent(pos.x, pos.y));
        s.Add(e, SpriteComponent({64, 64}, {0, 0}, _assets.GetTexture(AssetNames::Pipe_Up_R), 0.f, 1.f));
        collisionStorage.Add(e, CollisionComponent());
        boxColliderStorage.Add(e, BoxColliderComponent(64, 64));
        objectStorage.Add(e, ObjectComponent(false));
    }
    else if (name == AssetNames::Pipe_L)
    {
        int e = _world.CreateEntity();
        auto& p = _world.GetStorage<PositionComponent>();
        auto& s = _world.GetStorage<SpriteComponent>();
        auto& collisionStorage = _world.GetStorage<CollisionComponent>();
        auto& boxColliderStorage = _world.GetStorage<BoxColliderComponent>();
        auto& objectStorage = _world.GetStorage<ObjectComponent>();

        p.Add(e, PositionComponent(pos.x, pos.y));
        s.Add(e, SpriteComponent({64, 64}, {0, 0}, _assets.GetTexture(AssetNames::Pipe_L), 0.f, 1.f));
        collisionStorage.Add(e, CollisionComponent());
        boxColliderStorage.Add(e, BoxColliderComponent(64, 64));
        objectStorage.Add(e, ObjectComponent(false));
    }
    else if (name == AssetNames::Pipe_R)
    {
        int e = _world.CreateEntity();
        auto& p = _world.GetStorage<PositionComponent>();
        auto& s = _world.GetStorage<SpriteComponent>();
        auto& collisionStorage = _world.GetStorage<CollisionComponent>();
        auto& boxColliderStorage = _world.GetStorage<BoxColliderComponent>();
        auto& objectStorage = _world.GetStorage<ObjectComponent>();

        p.Add(e, PositionComponent(pos.x, pos.y));
        s.Add(e, SpriteComponent({64, 64}, {0, 0}, _assets.GetTexture(AssetNames::Pipe_R), 0.f, 1.f));
        collisionStorage.Add(e, CollisionComponent());
        boxColliderStorage.Add(e, BoxColliderComponent(64, 64));
        objectStorage.Add(e, ObjectComponent(false));
    }
    else if (name == AssetNames::BigHill)
    {
        int e = _world.CreateEntity();
        auto& p = _world.GetStorage<PositionComponent>();
        auto& s = _world.GetStorage<SpriteComponent>();

        p.Add(e, PositionComponent(pos.x, pos.y));
        s.Add(e, SpriteComponent({320, 192}, {0, 0}, _assets.GetTexture(AssetNames::BigHill), 0.f, 1.f));
    }
    else if (name == AssetNames::Finish)
    {
        int e = _world.CreateEntity();
        auto& p = _world.GetStorage<PositionComponent>();
        auto& s = _world.GetStorage<SpriteComponent>();
        auto& collisionStorage = _world.GetStorage<CollisionComponent>();
        auto& boxColliderStorage = _world.GetStorage<BoxColliderComponent>();
        auto& objectStorage = _world.GetStorage<ObjectComponent>();
        auto& fin = _world.GetStorage<FinishTag>();

        p.Add(e, PositionComponent(pos.x, pos.y));
        s.Add(e, SpriteComponent({64, 512}, {0, 0}, _assets.GetTexture(AssetNames::Finish), 0.f, 1.f));
        collisionStorage.Add(e, CollisionComponent());
        boxColliderStorage.Add(e, BoxColliderComponent(64, 512));
        objectStorage.Add(e, ObjectComponent(false));
        fin.Add(e, FinishTag());
    }
    else if (name == "DefautlCamera")
    {
        int e = _world.CreateEntity();
        auto& cam = _world.GetStorage<CameraComponent>();
        auto& tag = _world.GetStorage<DefaultCameraTag>();

        cam.Add(e, CameraComponent(sf::View(sf::FloatRect({0.f, 0.f}, {pos.x, pos.y}))));
        tag.Add(e, DefaultCameraTag());
    }
    else if (name == "FollowXCamera")
    {
        int e = _world.CreateEntity();
        auto& cam = _world.GetStorage<CameraComponent>();
        auto& tag = _world.GetStorage<FollowXCameraTag>();

        cam.Add(e, CameraComponent(sf::View(sf::FloatRect({0.f, 0.f}, {pos.x, pos.y}))));
        tag.Add(e, FollowXCameraTag());
    }
    else if (name == AssetNames::ExplosionAnim)
    {
        int e = _world.CreateEntity();
        auto& p = _world.GetStorage<PositionComponent>();
        auto& s = _world.GetStorage<SpriteComponent>();
        auto& a = _world.GetStorage<AnimationComponent>();
        auto& expl = _world.GetStorage<ExplosionTag>();

        p.Add(e, PositionComponent(pos.x, pos.y));
        s.Add(e, SpriteComponent({64, 64}, {0, 0}, _assets.GetTexture(AssetNames::TexExplosion), 0.f, 2.f));
        expl.Add(e, ExplosionTag());

        AnimationComponent anims;
        std::cout << "[DEBUG] idle tex addr = "
          << &_assets.GetTexture(AssetNames::TexIdle) << std::endl;

        Animation explosionAnim = _assets.GetAnimation(AssetNames::ExplosionAnim);

        anims.Animations.emplace(AssetNames::ExplosionAnim, explosionAnim);

        anims.CurrentAnimation = AssetNames::ExplosionAnim;

        a.Add(e, anims);

    }
}