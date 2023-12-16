#include "EngineCore.h"
#include "Engine.h"
#include "EngineTime.h"
#include "RenderSystem.h"
#include "SceneManager.h"
#include "Entity.h"
#include "InputSystem.h"

#include <thread>

#include "AssetManager.h"
#include "AudioSystem.h"
#include "CollisionSystem.h"
#include "Component.h"

Engine* Engine::instance = nullptr;

extern void Engine_Register();

void Engine::Initialize()
{
	Engine_Register();

	// Load the managers
	AudioSystem::Get().Load("../Assets/AudioSystem.json");
	AssetManager::Get().Load("../Assets/AssetManager.json");

	AudioSystem::Get().Initialize();
	AssetManager::Get().Initialize();

	RenderSystem::Instance().Initialize();
	SceneManager::Get().Load();

	// Initialize the managers

	SceneManager::Get().Initialize();

	InputSystem::Instance().registerQuitEventHandler([this] {isRunning = false; });



}

void Engine::Destroy()
{
	Time::Instance().Destroy();
	CollisionSystem::Instance().Destroy();
	SceneManager::Get().Destroy();
	AssetManager::Get().Destroy();
	AudioSystem::Get().Destroy();
	RenderSystem::Instance().Destroy();
	delete instance;
	instance = nullptr;
}

void Engine::GameLoop()
{
	while (isRunning)
	{
		Time::Instance().Update();

		// --------------------- Pre-update Phase ---------------------
		SceneManager::Get().PreUpdate();

		// --------------------- Update Phase ---------------------
		SceneManager::Get().Update();
		RenderSystem::Instance().Update();
		CollisionSystem::Instance().Update();

		// --------------------- Post-update Phase ---------------------
		SceneManager::Get().PostUpdate();


		/*	if (Time::Instance().TotalTime() > 5.0f)
			{
				break;
			}*/


			// --------------------- Input Phase ---------------------
		InputSystem::Instance().Update();
	/*	if (Time::Instance().TotalTime() > 5.0f)
		{
			break;
		}*/
	}
}
