#pragma once

#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

/*
* @Renderable
*
* Abstract class that any component that needs to be renderered to the RenderSystems
* SDL Window inherits from. Allows the user to pick a color filter to set over any
* Sprites.
*/

#include "Component.h"
#include "SDL.h"

class Renderable : public Component
{
	friend class RenderSystem;

	DECLARE_ABSTRACT_DERIVED_CLASS(Renderable, Component);

private:

/*
* @SetFilterColor
*
* Allows the user to set a color filter that will be applied overtop of their sprite.
* Takes RGB Values along with an Alpha. Nothing clamps the values so numbers bigger then 255
* or less than 0 will loop in their respective directions.
*/
	void SetFilterColor(int r, int g, int b, int a);

	virtual void Render() = 0;

protected:
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	SDL_Color filterColor = { 255, 255, 255 };

	Renderable();

	~Renderable() override;
};

#endif

