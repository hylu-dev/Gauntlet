#include "EngineCore.h"
#include "AudioSystem.h"

#include "SoundAsset.h"
#include "MusicAsset.h"

void AudioSystem::Initialize()
{
	Mix_Init(0);
	Mix_OpenAudio(sampleRate, MIX_DEFAULT_FORMAT, 2, 1024);
}

void AudioSystem::Destroy()
{
	Mix_CloseAudio();
	Mix_Quit();
}

void AudioSystem::Load(const std::string& config_file)
{
	std::ifstream inputStream(config_file.c_str());
	std::string str((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
	json::JSON node = json::JSON::Load(str);

	if (node.hasKey("SampleRate")) {
		sampleRate = node.at("SampleRate").ToInt();
	}
}

void AudioSystem::SetMusic(MusicAsset* music)
{
	Mix_PlayMusic(music->GetMusic(), 0);
}

void AudioSystem::PlayFX(SoundAsset* sound)
{
	Mix_PlayChannel(-1, sound->GetSound(), 0);
}
