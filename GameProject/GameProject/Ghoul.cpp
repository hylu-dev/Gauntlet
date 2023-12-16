#include "EngineCore.h"
#include "Ghoul.h"

std::string Ghoul::_type = "Ghoul";

void Ghoul::Initialize()
{
}

const std::string& Ghoul::GetComponentType()
{
	return _type;
}
