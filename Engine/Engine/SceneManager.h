// @file: SceneManager.h
//
// @brief: Header file for SceneManager, a singleton responsible for handling all scenes, entities, and components.
//
// @author: Divyanshu N Singh (DNS)
// @date: 2023-11-28

#pragma once
#ifndef _SCENE_MANAGER_H_

class Scene;
class Entity;

/**
 * @class SceneManager
 *
 * Scene Manager class provides an API to manage scenes and their entities.
 */
class SceneManager
{
private:
	DECLARE_SINGLETON(SceneManager);

	const std::string DATA_FILE = "../Assets/SceneManager.json";

	STRCODE activeSceneId = 0;
	Scene* activeScene = nullptr;

	// Scene to be set as active (happens in pre-update)
	Scene* toBeSetAsActive = nullptr;

	std::list<Scene*> scenesToBeLoaded;
	std::list<Scene*> loadedScenes;
	std::list<Scene*> scenesToBeUnloaded;

	// Keep track of file location for each Scene available
	// (i.e. a scene which either has a JSON or got created by user during runtime)
	std::map <STRCODE, std::string> stringUIDToFile;

protected:
	void Load();
	void Initialize();

	void PreUpdate();
	void Update();
	void PostUpdate();

	void Destroy();

public:
	// ------------------------- Scene-related member functions -------------------------
	Scene* GetActiveScene();
	STRCODE GetActiveSceneId();
	bool SetActiveScene(std::string sceneGuid);
	bool SetActiveScene(STRCODE sceneId);

	Scene* CreateScene();
	
	Scene* LoadScene(json::JSON&);
	Scene* FindScene(std::string sceneGuid);
	Scene* FindScene(STRCODE sceneId);
	bool UnloadScene(std::string sceneGuid);
	bool UnloadScene(STRCODE sceneId);

	// ------------------------- Entity-related member functions -------------------------
	Entity* CreateEntity();

	// Entity look-up always happens in the active scene
	Entity* FindEntity(std::string entityGuid);
	Entity* FindEntity(STRCODE entityId);
	// Entities in a scene can have same name
	std::list<Entity*> FindEntityByName(std::string entityName);
	std::list<Entity*> FindEntityWithComponent(std::string componentClassName);

	bool RemoveEntity(std::string entityGuid);
	bool RemoveEntity(STRCODE entityId);

	friend class Engine;
};

#endif // !_SCENE_MANAGER_H_
