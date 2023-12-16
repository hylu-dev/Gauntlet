#pragma once
#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "GameCore.h"

class SoundAsset;

class GameOver final : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(GameOver, Component)

public:
    void Initialize() override;
    void Update() override;
    void Load(json::JSON&) override;
private:
    float sound_delay = 5.0f;
    float sound_timer = 0;
    float scene_load_delay = 10;
    STRCODE scene_to_load = -1;
    SoundAsset* sound_fx = nullptr;
    bool is_playing_audio = false;
};

#endif // PLAYER_H


