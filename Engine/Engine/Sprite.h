#pragma once

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "SDL_rect.h"
#include "Renderable.h"
#include "TextureAsset.h"

/**
 * @class Sprite
 *
 * Sprite is a derived class of Renderable and serves to render a single texture on screen
 */
class Sprite : public Renderable
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Sprite, Renderable);

public:
	SDL_Rect targetRect = { 0,0,0,0 };
	TextureAsset* texture = nullptr;

protected:
	
	IVec2 size = {0, 0};
	SDL_Rect sourceRect = { 0,0,0,0 };

	/**
	 * @brief Initializes any variables as needed
	 *
	 */
	void Initialize() override;

	/**
	 * @brief Cleans up any memory and pointers as needed
	 *
	 */
	void Destroy() override;

	/**
	 * @brief Updates the sprite class rendering to match transforms
	 *
	 */
	void Update() override;

	/**
	 * @brief Copies the sprite onto a renderer to be displayed for that frame
	 *
	 */
	void Render() override;

public:
	/**
	 * @brief Sprite constructor
	 *
	 */
	Sprite() = default;

	/**
	 * @brief Sprite destructor
	 *
	 */
	~Sprite() override = default;

	/**
	 * @brief Setter for the texture area to render
	 *
	 * @param rect A rect of four values describing the area of the texture to render
	 */
	void SetSourceRect(SDL_Rect rect);

	/**
	 * @brief Initializes the Sprite class
	 *
	 * @param texAsset A pointer to a Texture Asset
	 */
	void SetTextureAsset(TextureAsset* texAsset);


	/**
	 * @brief Loads the Component and TextureAsset information from JSON
	 *
	 * @param node The JSON node containing component information
	 */
	void Load(json::JSON& node) override;
	
};

#endif