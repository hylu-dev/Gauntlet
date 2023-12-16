#pragma once
#ifndef SPAWNER_H
#define SPAWNER_H

#include "GameCore.h"
#include "TextureAsset.h"
#include "Transform.h"

class Spawner : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Spawner, Component)

public:
    void Initialize() override;
    void Update() override;
    void Load(json::JSON&) override;

private:
    Vec2 start_pos;
    BoxCollider* collider = nullptr;
    STRCODE game_over_scene = -1;

    Vec2 direction;

    float spawnCooldown = 2;
    float spawnCounter = 0;
    bool spawned = false;
};

#endif // Spawner_H


