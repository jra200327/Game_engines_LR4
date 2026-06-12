#include "EnemyAISystem.h"
#include <cmath>

void EnemyAISystem::OnInit()
{
}

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

std::vector<sf::Vector2i> EnemyAISystem::FindPath(sf::Vector2i start, sf::Vector2i goal)
{
    auto hash = [](sf::Vector2i p)
    {
        return p.x * 10000 + p.y;
    };

    auto h = [](sf::Vector2i a, sf::Vector2i b)
    {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    };

    struct Node
    {
        sf::Vector2i pos;
        float g;
    };

    std::vector<Node> open;
    std::unordered_map<int, sf::Vector2i> cameFrom;
    std::unordered_map<int, float> gScore;
    std::unordered_map<int, bool> closed;

    open.push_back({start, 0});
    gScore[hash(start)] = 0;

    while (!open.empty())
    {
        int bestIndex = 0;
        float bestF = 1e9f;

        for (int i = 0; i < (int)open.size(); i++)
        {
            float f = open[i].g + h(open[i].pos, goal);
            if (f < bestF)
            {
                bestF = f;
                bestIndex = i;
            }
        }

        Node current = open[bestIndex];
        open.erase(open.begin() + bestIndex);

        if (closed[hash(current.pos)])
            continue;

        closed[hash(current.pos)] = true;

        if (current.pos == goal)
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

        std::vector<sf::Vector2i> neighbors =
        {
            {1,0},{-1,0},{0,1},{0,-1}
        };

        for (auto n : neighbors)
        {
            sf::Vector2i next = current.pos + n;

            if (IsBlocked(next))
                continue;

            if (closed[hash(next)])
                continue;

            float tentative_g = gScore[hash(current.pos)] + 1;

            if (!gScore.count(hash(next)) || tentative_g < gScore[hash(next)])
            {
                gScore[hash(next)] = tentative_g;
                cameFrom[hash(next)] = current.pos;
                open.push_back({next, tentative_g});
            }
        }
    }

    return {};
}

void EnemyAISystem::OnUpdate()
{
    float dt = 1.0f / 60.0f;

    sf::Vector2f playerPos;
    int playerEnt = -1;

    for (auto ent : _player)
    {
        auto& pos = _positionComponents.Get(ent);
        playerPos = {pos.X, pos.Y};
        playerEnt = ent;
        break;
    }

    for (auto ent : _enemies)
    {
        auto& enemy = _enemyComponents.Get(ent);
        auto& pos   = _positionComponents.Get(ent);
        auto& move  = _movementComponents.Get(ent);

        sf::Vector2f enemyPos(pos.X, pos.Y);
        sf::Vector2i currentCell = _grid.WorldToGrid(enemyPos);

        bool seesPlayer = false;

        if (playerEnt != -1)
        {
            sf::Vector2f d = playerPos - enemyPos;
            float dist2 = d.x * d.x + d.y * d.y;

            seesPlayer = dist2 < (200.f * 200.f);
        }

        if (seesPlayer)
        {
            enemy.state = EnemyState::Chase;

            enemy.repathTimer += dt;

            if (enemy.repathTimer > 0.4f || enemy.path.empty())
            {
                enemy.repathTimer = 0.f;

                enemy.path = FindPath(
                    currentCell,
                    _grid.WorldToGrid(playerPos)
                );
            }
        }
        else
        {
            enemy.state = EnemyState::Patrol;

            if (enemy.path.empty())
            {
                enemy.path = FindPath(
                    currentCell,
                    enemy.currentPatrolTarget
                );

                if (currentCell == enemy.patrolA)
                    enemy.currentPatrolTarget = enemy.patrolB;
                else if (currentCell == enemy.patrolB)
                    enemy.currentPatrolTarget = enemy.patrolA;
            }
        }
        if (enemy.path.empty())
        {
            move.Direction.x = 0;
            move.Direction.y = 0;
            continue;
        }

        sf::Vector2i target = enemy.path.front();
        sf::Vector2i delta = target - currentCell;

        move.Direction.x = (delta.x > 0) ? 1 : (delta.x < 0 ? -1 : 0);
        move.Direction.y = (delta.y > 0) ? 1 : (delta.y < 0 ? -1 : 0);

        sf::Vector2i nextCell;
        nextCell.x = currentCell.x + move.Direction.x;
        nextCell.y = currentCell.y + move.Direction.y;

        if (IsBlocked(nextCell))
        {
            move.Direction.x = -move.Direction.x;
            move.Direction.y = -move.Direction.y;

            enemy.path.clear();
            continue;
        }


        if (currentCell == target)
            enemy.path.erase(enemy.path.begin());
    }
}