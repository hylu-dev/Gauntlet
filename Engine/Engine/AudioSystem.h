#pragma once

#ifndef _AUDIO_SYSTEM_H_
#define _AUDIO_SYSTEM_H_

class SoundAsset;
class MusicAsset;

class AudioSystem {
	int sampleRate = 44100;
	DECLARE_SINGLETON(AudioSystem);
	friend class Engine;
protected:

	void Initialize();

	void Destroy();
	
	void Load(const std::string& config_file);
public:
	void SetMusic(MusicAsset* music);
	void PlayFX(SoundAsset* sound);
};

#endif