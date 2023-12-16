#include "EngineCore.h"
#include "Zombie.h"

IMPLEMENT_DYNAMIC_CLASS(Zombie)

void Zombie::Initialize()
{
}

const std::string& Zombie::GetComponentType()
{
	return Zombie::GetClassName();
}
