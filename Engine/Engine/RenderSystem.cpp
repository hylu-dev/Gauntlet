/*
* @RenderSystem
*
* Controls the SDL window along with its properties. Also responsible for
* calling render on anything renderable.
*/

#include "EngineCore.h"
#include "RenderSystem.h"
#include "Renderable.h"
#include "Scene.h"

RenderSystem* RenderSystem::_instance = nullptr;

RenderSystem& RenderSystem::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new RenderSystem();
	}
	return *_instance;
}

RenderSystem::RenderSystem()
{

}

RenderSystem::~RenderSystem()
{

}

void RenderSystem::Initialize()
{
	//Pulls the window information from the RenderSettings file located in Assets
	std::ifstream inputStream("../Assets/RenderSettings.json");
	std::string str((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
	json::JSON document = json::JSON::Load(str);

	//Checks for name in RenderSettings
	if (document.hasKey("name"))
	{
		_name = document["name"].ToString();
	}
	else
	{
		std::cout << "Name wasn't found in RenderSettings. Going with the Default instead.";
	}

	//Checks for width in RenderSettings
	if (document.hasKey("width"))
	{
		_width = document["width"].ToInt();
	}
	else
	{
		std::cout << "Width wasn't found in RenderSettings. Going with the Default instead.";
	}

	//Checks for height in RenderSettings
	if (document.hasKey("height"))
	{
		_height = document["height"].ToInt();
	}
	else
	{
		std::cout << "Height wasn't found in RenderSettings. Going with the Default instead.";
	}

	//Checks for fullscreen in RenderSettings
	if (document.hasKey("fullscreen"))
	{
		_fullScreen = document["fullScreen"].ToBool();
	}
	else
	{
		std::cout << "Fullscreen wasn't found in RenderSettings. Going with the Default instead.";
	}

	_window = SDL_CreateWindow(_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _width, _height, _fullScreen);
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
}

void RenderSystem::Destroy()
{
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);

	delete _instance;
}

void RenderSystem::Update()
{
	SDL_RenderClear(_renderer);
	SDL_SetRenderDrawColor(_renderer, _backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);

	for (Renderable* renderable : _renderables)
	{
		if (!renderable->ownerEntity->GetParentScene()->isEnabled)
		{
			continue;
		}
		renderable->Render();
	}

	SDL_RenderPresent(_renderer);
}

void RenderSystem::Load()
{

}

SDL_Window& RenderSystem::GetWindow()
{
	return *_window;
}

SDL_Renderer& RenderSystem::GetRenderer()
{
	return *_renderer;
}

/*
* @AddRenderable
*
* When something renderable is created it calls this method and inserts
* itself into the list of renderables so RenderSystem can call its Render method
*/
void RenderSystem::AddRenderable(Renderable* renderable)
{
	_renderables.push_back(renderable);
}

/*
* @RemoveRenderable
*
* When a renderable is destroyed it removes itself from the renderables list using
* this method so RenderSystem no longer will try to call render on it
*/
void RenderSystem::RemoveRenderable(Renderable* renderable)
{
	_renderables.remove(renderable);
}

/*
* @WindowBackgroundColor
*
* Allows the user to set a new background color for the SDL Window by providing
* RGB Values and an Alpha
*/
void RenderSystem::WindowBackgroundColor(int r, int g, int b, int a)
{
	_backgroundColor.r = r;
	_backgroundColor.g = g;
	_backgroundColor.b = b;
	_backgroundColor.a = a;
}

/*
* @WindowSize
*
* Allows the user to set a new width and height for the SDL Window. Won't apply if
* the Window is in fullscreen mode.
*/
void RenderSystem::WindowSize(int width, int height)
{
	if (!_fullScreen)
	{
		_width = width;
		_height = height;
		SDL_SetWindowSize(_window, _width, _height);
	}
	else
	{
		std::cout << "Can't resize a fullscreen window. Change the RenderSettings if you want to use this method." << std::endl;
	}
}

IVec2 RenderSystem::GetWindowSize() const
{
	return {static_cast<int>(_width), static_cast<int>(_height)};
}
