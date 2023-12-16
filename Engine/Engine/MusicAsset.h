#pragma once

#ifndef _MUSIC_ASSET_H_
#define _MUSIC_ASSET_H_

#include "Asset.h"
#include "SDL_mixer.h"

class MusicAsset final : public Asset {
	Mix_Music* music;

	DECLARE_DYNAMIC_DERIVED_CLASS(MusicAsset, Asset)
	MusicAsset() = default;
    ~MusicAsset() override = default;
    void Initialize() override;
    void Destroy() override;
    void Load(json::JSON&) override;

    Mix_Music* GetMusic() { return music; }
};

#endif // !_MUSIC_ASSET_H_
