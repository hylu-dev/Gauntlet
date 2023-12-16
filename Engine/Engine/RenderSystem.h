#pragma once

#ifndef _RENDERSYSTEM_H_
#define _RENDERSYSTEM_H_

/*
* @RenderSystem
*
* Controls the SDL window along with its properties. Also responsible for
* calling render on anything renderable.
*/

#include <iostream>
#include <list>
#include "SDL.h"

class Renderable;

class RenderSystem
{
	friend class Engine;

	std::string _name = "DEFAULT NAME";

	int _width = 1280;
	unsigned int _height = 720;

	bool _fullScreen = false;

	SDL_Color _backgroundColor = { 0,0,0,255 };

	std::list<Renderable*> _renderables;

	static RenderSystem* _instance;

	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;

	RenderSystem();

	~RenderSystem();

	inline explicit RenderSystem(RenderSystem const&) = delete;
	inline RenderSystem& operator=(RenderSystem const&) = delete;

protected:

	void Initialize();

	void Update();

	void Destroy();

	void Load();

public:

	static RenderSystem& Instance();

	SDL_Window& GetWindow();

	SDL_Renderer& GetRenderer();

	/*
	* @AddRenderable
	*
	* When something renderable is created it calls this method and inserts
	* itself into the list of renderables so RenderSystem can call its Render method
	*/
	void AddRenderable(Renderable* renderable);

	/*
	* @RemoveRenderable
	*
	* When a renderable is destroyed it removes itself from the renderables list using
	* this method so RenderSystem no longer will try to call render on it
	*/
	void RemoveRenderable(Renderable* renderable);

	/*
	* @WindowBackgroundColor
	*
	* Allows the user to set a new background color for the SDL Window by providing
	* RGB Values and an Alpha
	*/
	void WindowBackgroundColor(int r, int g, int b, int a);

	/*
	* @WindowSize
	*
	* Allows the user to set a new width and height for the SDL Window. Won't apply if
	* the Window is in fullscreen mode.
	*/
	void WindowSize(int width, int height);

	IVec2 GetWindowSize() const;
};

#endif