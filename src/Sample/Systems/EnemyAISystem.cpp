#include "EnemyAISystem.h"

#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>

void EnemyAISystem::OnInit()
{

}

// -------------------- UTILS --------------------

static float Distance(sf::Vector2f a, sf::Vector2f b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

static int Heuristic(sf::Vector2i a, sf::Vector2i b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

// -------------------- BLOCK --------------------

bool EnemyAISystem::IsBlocked(sf::Vector2i cell)
{
    for (auto ent : _objects)
    {
        auto& pos = _positionComponents.Get(ent);

        sf::Vector2i objCell =
            _grid.WorldToGrid(sf::Vector2f(pos.X, pos.Y));

        if (objCell == cell)
            return true;
    }

    return false;
}

// -------------------- LOS --------------------

bool EnemyAISystem::HasLineOfSight(sf::Vector2i a, sf::Vector2i b)
{
    int dx = std::abs(b.x - a.x);
    int dy = std::abs(b.y - a.y);

    int sx = (a.x < b.x) ? 1 : -1;
    int sy = (a.y < b.y) ? 1 : -1;

    int err = dx - dy;

    while (true)
    {
        if (IsBlocked(a))
            return false;

        if (a == b)
            break;

        int e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            a.x += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            a.y += sy;
        }
    }

    return true;
}

// -------------------- MOVE (ONLY DIR) --------------------

void EnemyAISystem::Move(sf::Vector2f& pos,
                         MovementComponent& movement,
                         EnemyComponent& enemy,
                         float dt)
{
    if (enemy.path.empty())
    {
        movement.Direction = {0.f, 0.f};
        return;
    }

    sf::Vector2f target = _grid.GridToWorld(enemy.path.front());

    sf::Vector2f diff = target - pos;

    sf::Vector2f dir = {0.f, 0.f};

    if (std::abs(diff.x) > std::abs(diff.y))
        dir.x = (diff.x > 0.f) ? 1.f : -1.f;
    else
        dir.y = (diff.y > 0.f) ? 1.f : -1.f;

    movement.Direction = dir;
}

// -------------------- PATHFIND --------------------

std::vector<sf::Vector2i> EnemyAISystem::FindPath(
    sf::Vector2i start,
    sf::Vector2i goal,
    bool allowJump)
{
    struct Node
    {
        sf::Vector2i pos;
        int g;
        int h;
        int f() const { return g + h; }
    };

    auto cmp = [](const Node& a, const Node& b)
    {
        return a.f() > b.f();
    };

    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> open(cmp);

    std::unordered_map<int, sf::Vector2i> cameFrom;
    std::unordered_map<int, int> cost;

    auto hash = [](sf::Vector2i p)
    {
        return p.y * 10000 + p.x;
    };

    open.push({start, 0, Heuristic(start, goal)});
    cost[hash(start)] = 0;

    const std::vector<sf::Vector2i> dirs =
    {
        {1,0}, {-1,0}, {0,1}, {0,-1}
    };

    while (!open.empty())
    {
        Node cur = open.top();
        open.pop();

        if (cur.pos == goal)
        {
            std::vector<sf::Vector2i> path;
            sf::Vector2i p = goal;

            while (p != start)
            {
                path.push_back(p);
                p = cameFrom[hash(p)];
            }

            std::reverse(path.begin(), path.end());
            return path;
        }

        for (auto d : dirs)
        {
            sf::Vector2i next = cur.pos + d;

            if (IsBlocked(next))
                continue;

            if (allowJump && std::abs(next.y - cur.pos.y) > 1)
                continue;

            int newCost = cost[hash(cur.pos)] + 1;

            if (!cost.count(hash(next)) || newCost < cost[hash(next)])
            {
                cost[hash(next)] = newCost;
                cameFrom[hash(next)] = cur.pos;

                open.push({next, newCost, Heuristic(next, goal)});
            }
        }
    }

    return {};
}

// -------------------- UPDATE --------------------

void EnemyAISystem::OnUpdate()
{
    float dt = 1.f / 60.f;

    sf::Vector2f playerPos;

    for (auto ent : _player)
    {
        auto& p = _positionComponents.Get(ent);
        playerPos = {p.X, p.Y};
        break;
    }

    for (auto ent : _enemies)
    {
        auto& enemy = _enemyComponents.Get(ent);
        auto& pos = _positionComponents.Get(ent);
        auto& movement = _movementComponents.Get(ent);
        auto& jump = _jumpComponents.Get(ent);
        auto& grav = _gravityComponents.Get(ent);

        if (!enemy.isInitiated)
            continue;

        sf::Vector2f enemyPos = {pos.X, pos.Y};

        sf::Vector2i enemyCell = _grid.WorldToGrid(enemyPos);
        sf::Vector2i playerCell = _grid.WorldToGrid(playerPos);

        float dist = Distance(enemyPos, playerPos);

        enemy.seesPlayer =
            dist < enemy.sightRadius &&
            HasLineOfSight(enemyCell, playerCell);

        // ---------------- PATROL ----------------
        if (!enemy.seesPlayer)
        {
            enemy.state = EnemyState::Patrol;
            
            // Получаем мировые координаты цели
            sf::Vector2f targetWorldPos = _grid.GridToWorld(enemy.currentPatrolTarget);
            
            // Проверяем достижение цели по расстоянию (допуск 10 пикселей)
            float distToTarget = Distance(enemyPos, targetWorldPos);
            
            if (distToTarget < 40.f) // Достиг цели
            {
                // Меняем цель на другую точку патрулирования
                enemy.currentPatrolTarget = 
                    (enemy.currentPatrolTarget == enemy.patrolA) 
                    ? enemy.patrolB 
                    : enemy.patrolA;
                
                enemy.path.clear();
                
                // В этом кадре не двигаемся
                movement.Direction = {0.f, 0.f};
                continue;
            }

            enemy.path = FindPath(enemyCell, enemy.currentPatrolTarget, true);
            
            Move(enemyPos, movement, enemy, dt);
        }
        else
        {
            enemy.state = EnemyState::Chase;

            enemy.repathTimer -= dt;

            if (enemy.repathTimer <= 0.f || enemy.path.empty())
            {
                enemy.path = FindPath(enemyCell, playerCell, true);
                enemy.repathTimer = 0.1f;
            }

            Move(enemyPos, movement, enemy, dt);
            
            // ---------------- JUMP FLAG ----------------
            if (_jumpComponents.Has(ent) && _gravityComponents.Has(ent))
            {
                if (!enemy.path.empty())
                {
                    sf::Vector2i nextCell = enemy.path.front();

                    if (nextCell.y < enemyCell.y && grav.grounded)
                    {
                        jump.jumpRequested = true;
                    }
                }
            }
        }
        
    }
}