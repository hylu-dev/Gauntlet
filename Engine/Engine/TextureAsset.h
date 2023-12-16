#pragma once

#ifndef  _TEXTURE_ASSET_H_
#define _TEXTURE_ASSET_H_

#include "Asset.h"
#include "SDL_image.h"

class TextureAsset final : public Asset {
    SDL_Texture* texture = nullptr;
    int width = 1;
    int height = 1;

	DECLARE_DYNAMIC_DERIVED_CLASS(TextureAsset, Asset)
    TextureAsset() = default;
    ~TextureAsset() override = default;
    void Initialize() override;
    void Destroy() override;
    void Load(json::JSON&) override;
    IVec2 GetDimensions() const;
    SDL_Texture* GetTexture() const
    {
	    return texture;
    }
};

#endif // !_TEXTURE_ASSET_H_
