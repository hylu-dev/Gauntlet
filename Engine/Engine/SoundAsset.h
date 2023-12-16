#pragma once

#ifndef _SOUND_ASSET_H_
#define _SOUND_ASSET_H_

#include "Asset.h"
#include "SDL_mixer.h"

class SoundAsset final : public Asset
{
    Mix_Chunk* sound = nullptr;

	DECLARE_DYNAMIC_DERIVED_CLASS(SoundAsset, Asset)
    SoundAsset() = default;
    ~SoundAsset() override = default;
    void Initialize() override;
    void Destroy() override;
    void Load(json::JSON&) override;

    Mix_Chunk* GetSound() const;
};

#endif // !_SOUND_ASSET_H_
