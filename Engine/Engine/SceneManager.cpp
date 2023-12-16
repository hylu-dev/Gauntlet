// @file: SceneManager.cpp
//
// @brief: Cpp file for SceneManager, a singleton responsible for handling all scenes, entities, and components.
//
// @author: Divyanshu N Singh (DNS)
// @date: 2023-11-28

#include "EngineCore.h"
#include "SceneManager.h"
#include "Scene.h"

/**
 * @brief Load Scene Manager saved data from SceneManager.json file
 */
void SceneManager::Load()
{
	std::ifstream inputStream(DATA_FILE);
	std::string str((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
	json::JSON sceneManagerJSON = json::JSON::Load(str);

	// Store mapping of scene id to scene path location
	// SceneManager must know about every scene in existance
	THROW_RUNTIME_ERROR(!sceneManagerJSON.hasKey("AllScenes"), "Scene Manager must have GIUD & path data of all available scenes.");
	json::JSON allScenes = sceneManagerJSON["AllScenes"];
	for (json::JSON& sceneInfo : allScenes.ArrayRange())
	{
		if (sceneInfo.hasKey("GUID") && sceneInfo.hasKey("Path"))
		{
			std::string sceneGUID = sceneInfo["GUID"].ToString();
			std::string scenePath = sceneInfo["Path"].ToString();

			STRCODE sceneId = GetHashCode(sceneGUID.c_str());
			stringUIDToFile[sceneId] = scenePath;
		}
	}
	
	// Get the active scene GUID. Convert it to STRCODE
	THROW_RUNTIME_ERROR(!sceneManagerJSON.hasKey("ActiveSceneGUID"), "Scene Manager must have an active scene.");
	activeSceneId = GetHashCode(sceneManagerJSON["ActiveSceneGUID"].ToString().c_str());

	// Load the scenes to be loaded at the beginning of the game
	if (sceneManagerJSON.hasKey("LoadedScenes"))
	{
		json::JSON scenesJSON = sceneManagerJSON["LoadedScenes"];
		for (json::JSON& sceneGuidJSON : scenesJSON.ArrayRange())
		{
			THROW_RUNTIME_ERROR(!sceneGuidJSON.hasKey("GUID"), "A loaded scene must have a GUID.");

			STRCODE sceneID = GetHashCode(sceneGuidJSON["GUID"].ToString().c_str());
			if (stringUIDToFile.find(sceneID) != stringUIDToFile.end())
			{
				// Load up Scene JSON file & send the JSON object to Scene
				std::string sceneFileLocation = "../Assets/" + stringUIDToFile[sceneID];

				std::ifstream sceneInputStream(sceneFileLocation);
				std::string sceneStr((std::istreambuf_iterator<char>(sceneInputStream)), std::istreambuf_iterator<char>());
				json::JSON sceneJSON = json::JSON::Load(sceneStr);

				// Load the scene & track it
				Scene* scene = LoadScene(sceneJSON);

				// If this is the active scene, set it
				if (sceneID == activeSceneId)
				{
					activeScene = scene;
					// active scene has to be enabled, or else it won't update and render
					activeScene->isEnabled = true;
				}
			}
		}
	}
	LOG("Loaded SceneManager: " << scenesToBeLoaded.size() << " scenes loaded from JSON.");
}

/**
 * @brief Initialize all the scenes.
 */
void SceneManager::Initialize()
{
	for (Scene* scene : loadedScenes)
	{
		scene->Initialize();
	}
}

/**
 * @brief Load the to-be-loaded scenes.
 */
void SceneManager::PreUpdate()
{
	for (Scene* scene : scenesToBeLoaded)
	{
		loadedScenes.push_back(scene);
	}
	scenesToBeLoaded.clear();
	// Check if active scene got modified
	if (toBeSetAsActive != nullptr)
	{
		activeScene = toBeSetAsActive;
		// Active scene must be enabled
		activeScene->isEnabled = true;
		toBeSetAsActive = nullptr;
	}
}

/**
 * @brief Update all the enabled scenes.
 */
void SceneManager::Update()
{
	for (Scene* scene : loadedScenes)
	{
		if (scene->isEnabled)
		{
			scene->PreUpdate();
			scene->Update();
			scene->PostUpdate();
		}
	}
}

/**
 * @brief Unload the to-be-unloaded scenes.
 */
void SceneManager::PostUpdate()
{
	for (Scene* scene : scenesToBeUnloaded)
	{
		scene->Destroy();
		delete scene;
		loadedScenes.remove(scene);
	}
	scenesToBeUnloaded.clear();
}

/**
 * @brief Destory all the scenes.
 */
void SceneManager::Destroy()
{
	if (scenesToBeLoaded.size() != 0)
		loadedScenes.merge(scenesToBeLoaded);
	if (scenesToBeUnloaded.size() != 0)
		loadedScenes.merge(scenesToBeUnloaded);
	
	for (Scene* scene : loadedScenes)
	{
		scene->Destroy();
		delete scene;
	}
	scenesToBeUnloaded.clear();
	loadedScenes.clear();
	scenesToBeLoaded.clear();
}

// ------------------------- Scene-related member functions -------------------------

/**
 * @brief Create a new scene. Scene Manager automatically keeps track of this scene.
 * 
 * @return Pointer to the created scene.
 */
Scene* SceneManager::CreateScene()
{
	// This scene might or might not have a JSON file. Can not add it to stringUIDToFile
	Scene* scene = new Scene();

	// Created scenes automatically get added
	scenesToBeLoaded.push_back(scene);
	
	return scene;
}

/**
 * @brief Load a scene using JSON object. Scene Manager automatically keeps track of this scene.
 * 
 * @param sceneJSON json object containing scene data.
 * @return Pointer to the created scene.
 */
Scene* SceneManager::LoadScene(json::JSON& sceneJSON)
{
	Scene* scene = new Scene();
	scene->Load(sceneJSON);

	// Loaded scenes automatically get added
	scenesToBeLoaded.push_back(scene);

	return scene;
}

/**
 * @brief Getter function to get the active scene.
 *
 * @return Pointer to the active scene.
 */
Scene* SceneManager::GetActiveScene()
{
	return activeScene;
}

/**
 * @brief Getter function to get the active scene.
 *
 * @return ID of the active scene.
 */
STRCODE SceneManager::GetActiveSceneId()
{
	return activeScene->uid;
}

/**
 * @brief Setter function to set the active scene using Scene GUID.
 *
 * @param sceneGuid GUID of the scene to be set as active.
 * @return Pointer to the active scene.
 */
bool SceneManager::SetActiveScene(std::string sceneGuid)
{
	STRCODE sceneId = GetHashCode(sceneGuid.c_str());
	return SetActiveScene(sceneId);
}

/**
 * @brief Set a scene as active scene. This scene must be loaded first.
 *
 * @param sceneId UID of the scene to be set as active.
 * @return Pointer to the active scene.
 */
bool SceneManager::SetActiveScene(STRCODE sceneId)
{
	// Look for the scene in scenesToBeLoaded & loadedScenes
	for (Scene* scene : scenesToBeLoaded)
	{
		if (scene->GetUID() == sceneId)
		{
			toBeSetAsActive = scene;
			return true;
		}
	}
	for (Scene* scene : loadedScenes)
	{
		if (scene->GetUID() == sceneId)
		{
			// Ensure that this scene is not scheduled to be deleted
			auto it = std::find_if(scenesToBeUnloaded.begin(), scenesToBeUnloaded.end(),
								   [sceneId](Scene* sc) {
										return sc->GetUID() == sceneId;
									});
			THROW_RUNTIME_ERROR(it != scenesToBeUnloaded.end(), "Error! The scene being set as active does not exist anymore.");
			toBeSetAsActive = scene;
			return true;
		}
	}
	return false;
}

/**
 * @brief Find a scene among the loaded scenes.
 *
 * @param sceneGuid GUID of the scene.
 * @return Pointer to the scene. if not found, returns nullptr.
 */
Scene* SceneManager::FindScene(std::string sceneGuid)
{
	STRCODE sceneId = GetHashCode(sceneGuid.c_str());
	return FindScene(sceneId);
}

/**
 * @brief Find a scene among the loaded scenes.
 *
 * @param sceneId UID of the scene.
 * @return Pointer to the scene. if not found, returns nullptr.
 */
Scene* SceneManager::FindScene(STRCODE sceneId)
{
	for (Scene* scene : loadedScenes)
	{
		if (scene->GetUID() == sceneId)
		{
			return scene;
		}
	}
	return nullptr;
}

/**
 * @brief Unload a scene.
 *
 * @param sceneGuid GUID of the scene.
 * @return Boolean represeting if unloading was successful.
 */
bool SceneManager::UnloadScene(std::string sceneGuid)
{
	STRCODE sceneId = GetHashCode(sceneGuid.c_str());
	return UnloadScene(sceneId);
}

/**
 * @brief Unload a scene.
 *
 * @param sceneId UID of the scene.
 * @return Boolean represeting if unloading was successful.
 */
bool SceneManager::UnloadScene(STRCODE sceneId)
{
	// Ensure that user is not trying to remove the active scene
	Scene* actualActiveScene = (toBeSetAsActive == nullptr) ? activeScene : toBeSetAsActive;
	if (sceneId == actualActiveScene->GetUID())
		return false;

	for (Scene* scene : loadedScenes)
	{
		if (scene->GetUID() == sceneId)
		{
			scenesToBeUnloaded.push_back(scene);
			return true;
		}
	}
	return false;
}

// ------------------------- Entity-related member functions -------------------------

/**
 * @brief Create a new entity in the active scene.
 *
 * @return Pointer to the newly created entity.
 */
Entity* SceneManager::CreateEntity()
{
	return activeScene->CreateEntity();
}

/**
 * @brief Find an entity in the active scene.
 *
 * @param entityGuid GUID of the entity
 * @return Pointer to the found entity. If not found, returns nullptr.
 */
Entity* SceneManager::FindEntity(std::string entityGuid)
{
	return activeScene->FindEntity(entityGuid);
}

/**
 * @brief Find an entity in the active scene.
 *
 * @param entityId UID of the entity
 * @return Pointer to the found entity. If not found, returns nullptr.
 */
Entity* SceneManager::FindEntity(STRCODE entityId)
{
	return activeScene->FindEntity(entityId);
}

/**
 * @brief Find entities in the active scene by name.
 *
 * @param entityName Name of the entity
 * @return List of entity pointers.
 */
std::list<Entity*> SceneManager::FindEntityByName(std::string entityName)
{
	return activeScene->FindEntityByName(entityName);
}

/**
 * @brief Find entities in the active scene which contain a component class.
 *
 * @param componentClassName Name of a class which inherits from Component class
 * @return List of entity pointers.
 */
std::list<Entity*> SceneManager::FindEntityWithComponent(std::string componentClassName)
{
	return activeScene->FindEntityWithComponent(componentClassName);
}

/**
 * @brief Remove an entity from the active scene.
 *
 * @param entityGuid GUID of the entity
 * @return Boolean representing if entity got removed successfully.
 */
bool SceneManager::RemoveEntity(std::string entityGuid)
{
	return activeScene->RemoveEntity(entityGuid);
}

/**
 * @brief Remove an entity from the active scene.
 *
 * @param entityId UID of the entity
 * @return Boolean representing if entity got removed successfully.
 */
bool SceneManager::RemoveEntity(STRCODE entityId)
{
	return activeScene->RemoveEntity(entityId);
}