#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "GameCore.h"
#include "TextureAsset.h"
#include "Transform.h"

class Enemy : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Enemy, Component)

public:
    void Initialize() override;
    void Update() override;
    void Load(json::JSON&) override;
private:
    float speed = 5.0f;
    Vec2 start_pos;
    BoxCollider* collider = nullptr;
    STRCODE game_over_scene = -1;
    Transform target;
    
};

#endif // Enemy_H


