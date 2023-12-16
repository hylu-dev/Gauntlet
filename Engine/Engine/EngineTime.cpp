#include "EngineCore.h"
#include "EngineTime.h"

Time* Time::instance = nullptr;

void Time::Update()
{
	endTime = std::chrono::system_clock().now();
	deltaTime = endTime - beginTime;
	beginTime = endTime;
	totalTime += deltaTime;
	frameCount++;
}

void Time::Initialize()
{
	beginTime = std::chrono::system_clock().now();
	endTime = std::chrono::system_clock().now();

	deltaTime = std::chrono::duration<float>(0);
	totalTime = std::chrono::duration<float>(0);
	frameCount = 0;
}
