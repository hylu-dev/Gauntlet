#include "GameCore.h"
#include "GameOver.h"

IMPLEMENT_DYNAMIC_CLASS(GameOver)

void GameOver::Initialize()
{
    Component::Initialize();
    is_playing_audio = false;
    sound_timer = sound_delay;
    ownerEntity->GetTransform().position = Vec2(RenderSystem::Instance().GetWindowSize()) * 0.5f;
}
void GameOver::Update() {
	sound_timer -= Time::Instance().DeltaTime();
    if (!is_playing_audio && sound_timer < 0.0f)
    {
        AudioSystem::Get().PlayFX(sound_fx);
        is_playing_audio = true;
    }

    if (is_playing_audio && sound_timer < -scene_load_delay) {
    	Scene* current_scene = SceneManager::Get().GetActiveScene();
    	if (SceneManager::Get().SetActiveScene(scene_to_load))
    	{
    		current_scene->isEnabled = false;
    	}
        is_playing_audio = false;
        sound_timer = sound_delay;
    }
}
void GameOver::Load(json::JSON& node)
{
    Component::Load(node);
    if (node.hasKey("SoundDelay"))
    {
        sound_delay = static_cast<float>(node.at("SoundDelay").ToFloat());
    }

    if (node.hasKey("SceneToLoad"))
    {
	    scene_to_load = GetHashCode(node.at("SceneToLoad").ToString().c_str());
    }

    if (node.hasKey("Sound"))
    {
	    const std::string sound_asset_guid = node["Sound"].ToString();
	    sound_fx = (SoundAsset*)(AssetManager::Get().GetAsset(sound_asset_guid));
    }

    if (node.hasKey("SceneLoadDelay"))
    {
        scene_load_delay = static_cast<float>(node.at("SceneLoadDelay").ToFloat());
    }
}
