#include "EngineCore.h"
#include "Asset.h"

IMPLEMENT_ABSTRACT_CLASS(Asset)

void Asset::Initialize()
{
}

void Asset::Destroy()
{
	Object::Destroy();
}

void Asset::Load(json::JSON& json)
{
	Object::Load(json);
	LOG("Loading Asset: " << filepath)
}
