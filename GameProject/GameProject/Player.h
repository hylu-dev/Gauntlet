#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "GameCore.h"
#include "TextureAsset.h"

class Player : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Player, Component)

public:
    void Initialize() override;
    void Update() override;
    void Load(json::JSON&) override;
private:
    float speed = 5.0f;
    Vec2 start_pos;
    BoxCollider* collider = nullptr;
    STRCODE game_over_scene = -1;

    TextureAsset* up = nullptr;
    TextureAsset* left = nullptr;
    TextureAsset* down = nullptr;
    TextureAsset* right = nullptr;
};

#endif // PLAYER_H


