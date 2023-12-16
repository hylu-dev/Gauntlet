#pragma once
#ifndef _FONT_SPRITE_
#define _FONT_SPRITE_

/*
* @FontSprite
*
* Allows the user to display text onto the SDL Window. They can control the text content,
* font used by providing a new asset to use, and the texts display color.
*/

#include <iostream>
#include "Renderable.h"
#include "SDL.h"

class FontAsset;

class FontSprite : public Renderable
{
	DECLARE_DYNAMIC_DERIVED_CLASS(FontSprite, Renderable);

private:

	IVec2 outputSizing = { 0, 0 };

	std::string text = "DEFAULT TEXT";

	FontAsset* font = nullptr;

	SDL_Rect fontRect = { 0,0,250,250 };

	SDL_Texture* output = nullptr;

	SDL_Color fontColor = { 255,255,255,255 };

	/*
	* @RegenerateOutput
	*
	* Used by other methods to regenerate the SDL_Texture after things like text, font or color
	* have changed.
	*/
	void RegenerateOutput();

protected:

	FontSprite() = default;

	~FontSprite() override = default;

	/*
	* @Initialize
	* 
	* There is no default font. You will see nothing until you provide a font to the FontSprite
	*/
	void Initialize() override;

	void Destroy() override;

	void Render() override;

	void Save(json::JSON&) const;

	void Load(json::JSON&) override;

	void Update() override;

public:

	/*
	* @SetText
	*
	* Takes a string for the display text and regenerates the font texture.
	*/
	void SetText(std::string text);

	/*
	* @SetFont
	*
	* Takes a TextureAsset for the new font and regenerates the font texture.
	*/
	void SetFont(FontAsset* font);

	/*
	* @SetFontColor
	*
	* Takes RGB values along with an Alpha and regenerates the font texture.
	*/
	void SetFontColor(int r, int g, int b, int a);
};

#endif