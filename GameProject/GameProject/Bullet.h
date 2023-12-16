#pragma once
#ifndef BULLET_H
#define BULLET_H

#include "GameCore.h"
#include "TextureAsset.h"
#include "Transform.h"

class Bullet : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Bullet, Component)

public:
    void Initialize() override;
    void Update() override;
    void Load(json::JSON&) override;
    void SetTarget(Vec2 dir);

private:
    float speed = 500.0f;
    Vec2 start_pos;
    BoxCollider* collider = nullptr;
    STRCODE game_over_scene = -1;
    
    Vec2 direction;
};

#endif // Bullet_H


