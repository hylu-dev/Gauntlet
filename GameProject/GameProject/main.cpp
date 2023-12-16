#include "GameCore.h"

extern void GameRegisterClasses();

int main(int argc, char** argv)
{
	GameRegisterClasses();

	Engine::Instance().Initialize();
	Engine::Instance().GameLoop();
	Engine::Instance().Destroy();

	return 0;
}
