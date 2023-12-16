#pragma once
#ifndef _FONT_ASSET_H_
#define _FONT_ASSET_H_

#include "Asset.h"
#include "SDL_ttf.h"


class FontAsset final : public Asset
{
    TTF_Font* font = nullptr;
    int font_size = 12;

	DECLARE_DYNAMIC_DERIVED_CLASS(FontAsset, Asset)
    FontAsset() = default;
    ~FontAsset() override = default;
    void Initialize() override;
    void Destroy() override;
    void Load(json::JSON&) override;
    TTF_Font* GetFont() const;
    void SetFontSize(int size) const;
};

#endif // !_FONT_ASSET_H_
