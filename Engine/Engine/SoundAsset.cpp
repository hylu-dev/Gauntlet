#include "EngineCore.h"
#include "SoundAsset.h"

IMPLEMENT_DYNAMIC_CLASS(SoundAsset)

void SoundAsset::Initialize()
{
	Asset::Initialize();
}

void SoundAsset::Destroy()
{
	Mix_FreeChunk(sound);
	sound = nullptr;
	Asset::Destroy();
}

void SoundAsset::Load(json::JSON& json)
{
	Asset::Load(json);
	sound = Mix_LoadWAV(filepath.c_str());
}

Mix_Chunk* SoundAsset::GetSound() const
{
	return sound;
};
