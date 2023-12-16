#pragma once

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <list>

class Engine
{
public:

	static Engine& Instance()
	{
		if (instance == nullptr)
		{
			instance = new Engine();
		}
		return *instance;
	}

	void Initialize();
	void Destroy();
	void GameLoop();
	bool isRunning = true;

private:
	static Engine* instance;
	
private:
	inline explicit Engine() = default;
	inline ~Engine() = default;
	inline explicit Engine(Engine const&) = delete;
	inline Engine& operator=(Engine const&) = delete;
};

#endif