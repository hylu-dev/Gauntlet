// @file: Scene.cpp
//
// @brief: Cpp file for the Scene class. All entities are part of a Scene.
//
// @author: Divyanshu N Singh (DNS)
// @date: 2023-11-29

#include "EngineCore.h"
#include "Scene.h"
#include "Entity.h"
#include "AssetManager.h"

/**
 * @brief Scene constructor generates a random GUID & UID.
 */
Scene::Scene()
{
	UUID _guid;
	// Create a new GUID
	CreateUUID(&_guid);

	guid = GUIDTostring(_guid);
	uid = GetHashCode(guid.c_str());
}

/**
 * @brief Constructor for Scene which generates a random GUID & UID.
 * 
 * @param _guid GUID of the scene.
 */
Scene::Scene(std::string _guid)
{
	// Use the GUID passed to it
	uid = GetHashCode(guid.c_str());
	guid = _guid;
}

/**
 * @brief Initialize all the entities of this scene.
 */
void Scene::Initialize()
{
	for (Entity* entity : entities)
	{
		entity->Initialize();
	}
}

/**
 * @brief Load Scene data passed in a JSON.
 *
 * @param sceneJSON Scene data JSON.
 */
void Scene::Load(json::JSON& sceneJSON)
{
	THROW_RUNTIME_ERROR(!sceneJSON.hasKey("AssetManager"), "Scene JSON must contain GUIDs of all the assets it needs.");
	THROW_RUNTIME_ERROR(!sceneJSON.hasKey("Scene"), "Scene JSON must contain scene info.");

	// Load all the assets used by this scene
	json::JSON assetsJSON = sceneJSON["AssetManager"];
	if (assetsJSON.hasKey("Assets"))
	{
		for (json::JSON& assetJSON : assetsJSON["Assets"].ArrayRange())
		{
			if (assetJSON.hasKey("GUID"))
			{
				std::string assetGUID = assetJSON["GUID"].ToString();
				AssetManager::Get().LoadSceneAsset(assetGUID);
				assetsGUIDs.push_back(assetGUID);
			}
		}
	}

	json::JSON sceneData = sceneJSON["Scene"];
	if (sceneData.hasKey("Name"))
	{
		name = sceneData["Name"].ToString();
	}
	// If GUID exists, it overwrites the guid & uid populated in Scene constructor
	if (sceneData.hasKey("GUID"))
	{
		guid = sceneData["GUID"].ToString();
		uid = GetHashCode(guid.c_str());
	}
	if (sceneData.hasKey("IsEnabled"))
	{
		isEnabled = sceneData["IsEnabled"].ToBool();
	}

	// Load the entities
	if (sceneData.hasKey("Entities"))
	{
		json::JSON entitiesJSON = sceneData["Entities"];
		for (json::JSON& entityJSON : entitiesJSON.ArrayRange())
		{
			Entity* entity = CreateEntity();
			entity->Load(entityJSON);
		}
	}
}

/**
 * @brief Load the to-be-added entities.
 */
void Scene::PreUpdate()
{
	for (Entity* entity : entitiesToBeAdded)
	{
		entities.push_back(entity);
	}
	entitiesToBeAdded.clear();
}

/**
 * @brief Update all the active entities.
 */
void Scene::Update()
{
	for (Entity* entity : entities)
	{
		if (entity->IsActive())
		{
			entity->PreUpdate();
			entity->Update();
			entity->PostUpdate();
		}
	}
}

/**
 * @brief Remove the to-be-destroyed entities.
 */
void Scene::PostUpdate()
{
	for (Entity* entity : entitiesToDestroy)
	{
		entity->Destroy();
		delete entity;
		entities.remove(entity);
	}
	entitiesToDestroy.clear();
}

/**
 * @brief Destory all the entities.
 */
void Scene::Destroy()
{
	for (Entity* entity : entities)
	{
		entity->Destroy();
		delete entity;
	}
	entities.clear();

	// Unload all assets
	for (std::string& assetGUID : assetsGUIDs)
	{
		AssetManager::Get().UnloadSceneAsset(assetGUID);
	}
}

/**
 * @brief Create a new entity. Scene automatically keeps track of this entity.
 *
 * @return Pointer to the created entity.
 */
Entity* Scene::CreateEntity()
{
	Entity* entity = new Entity();
	entity->ownerScene = this;
	// The scene that creates an entity has its ownership
	entitiesToBeAdded.push_back(entity);
	return entity;
}

/**
 * @brief Find an entity in the scene.
 *
 * @param entityGuid GUID of the entity.
 * @return Pointer to the found entity.
 */
Entity* Scene::FindEntity(std::string entityGuid)
{
	STRCODE entityId = GetHashCode(entityGuid.c_str());
	return FindEntity(entityId);
}

/**
 * @brief Find an entity in the scene.
 *
 * @param entityId UID of the entity.
 * @return Pointer to the found entity.
 */
Entity* Scene::FindEntity(STRCODE entityId)
{
	for (Entity* entity : entities)
	{
		if (entity->GetUid() == entityId)
		{
			return entity;
		}
	}
	return nullptr;
}

/**
 * @brief Search an entity by name in the scene.
 *
 * @param entityName Name of the entity.
 * @return List of pointers to the matched entities.
 */
std::list<Entity*> Scene::FindEntityByName(std::string entityName)
{
	std::list<Entity*> foundEntities;
	for (Entity* entity : entities)
	{
		if (entity->GetName() == entityName)
		{
			foundEntities.push_back(entity);
		}
	}
	return foundEntities;
}

/**
 * @brief Lookup entities with a certain component.
 *
 * @param componentName Name of a class which inherits from Component.
 * @return List of pointers to the found entities.
 */
std::list<Entity*> Scene::FindEntityWithComponent(std::string componentName)
{
	std::list<Entity*> foundEntities;
	for (Entity* entity : entities)
	{
		if (entity->GetComponent(componentName))
		{
			foundEntities.push_back(entity);
		}
	}
	return foundEntities;
}

/**
 * @brief Remove an entity from the Scene.
 *
 * @param entityGuid GUID of the entity.
 * @return Boolean representing if the entity got removed successfully.
 */
bool Scene::RemoveEntity(std::string entityGuid)
{
	STRCODE entityId = GetHashCode(entityGuid.c_str());
	return RemoveEntity(entityId);
}

/**
 * @brief Remove an entity from the Scene.
 *
 * @param entityId UID of the entity.
 * @return Boolean representing if the entity got removed successfully.
 */
bool Scene::RemoveEntity(STRCODE entityId)
{
	for (Entity* entity : entities)
	{
		if (entity->GetUid() == entityId)
		{
			entitiesToDestroy.push_back(entity);
			return true;
		}
	}
	return false;
}

// ------------------------- Getters -------------------------

/**
 * @brief Getter to get the Scene GUID.
 *
 * @return Scene's GUID.
 */
std::string& Scene::GetGUID()
{
	return guid;
}

/**
 * @brief Getter to get the Scene UID.
 *
 * @return Scene's UID.
 */
STRCODE Scene::GetUID()
{
	return uid;
}

/**
 * @brief Getter to get the Scene Name.
 *
 * @return Scene's Name.
 */
std::string& Scene::GetName()
{
	return name;
}

