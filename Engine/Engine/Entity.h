/**
* @class Entity
*
* The Entity class is the base class for all the objects in the game.
* It represents a game object and contains a list of Components and a Transform.
*/
#pragma once

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Object.h"

class Transform;
class Component;
class Scene;

class Entity final : public Object
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Entity, Object)

private:
    Scene* ownerScene = nullptr;
    Transform transform;

    std::list<Component*> components;
    std::list<Component*> componentsToAdd;
    std::list<Component*> componentsToRemove;

protected:
    /**
     * @brief Default constructor for the Entity class.
     * This constructor is protected to prevent direct instantiation of the Entity class. 
     * Use derived classes to create specific entity types.
     */
    Entity() = default;

    /**
     * @brief Constructor for the Entity class.
     *
     * @param _guid The GUID of the entity.
     */
    Entity(std::string _guid) : Object(_guid) { }

    /**
     * @brief Destructor for the Entity class.
     */
    ~Entity() override = default;

    /**
     * @brief Initializes the entity.
     * This function is called during the initialization of the entity.
     */
    void Initialize() override;

    /**
     * @brief Loads entity data from JSON.
     *
     * @param node The JSON object containing the entity data.
     */
    void Load(json::JSON& node) override;

    /**
     * @brief Updates the entity's state.
     */
    void Update();

    /**
     * @brief Performs pre-update operations on the entity.
     * This function adds the components in the componentsToAdd list to the entity.
     */
    void PreUpdate();

    /**
     * @brief Performs post-update operations on the entity.
     * This function removes the components in the componentsToRemove list from the entity.
     */
    void PostUpdate();

    /**
     * @brief Destroys the entity.
     * This function is called when the entity is being destroyed,
     * can be overridden by derived classes to perform additional cleanup.
     */
    void Destroy() override;

public:
    /**
     * @brief Check if the Entity has a Component with the given name
     *
     * @param componentName Name of the Component
     * @return true if the Entity has the Component, false if not
     */
    bool HasComponent(const std::string& componentName) const;

    /**
     * @brief Add a list of Components by their names to the Entity
     *
     * @param componentList List of Component names
     */
    void AddComponents(const std::vector<std::string>& componentList);

    /**
     * @brief Get a Component from Entity with the given name
     *
     * @param componentName Name of the Component
     * @return Component* if the Entity has the Component, nullptr if not
     */
    Component* GetComponent(const std::string& componentName) const;

    /**
     * @brief Create a single Component by its name
     *
     * @param componentName Name of the Component
     * @return Component* if the Component is successfully created, nullptr if not
     */
    Component* CreateComponent(const std::string& componentName);

    /**
    * @brief Remove a Component from the Entity
    *
    * @param component Component to remove
    * @return true if the Component is successfully removed, false if not
    */
    bool RemoveComponent(const Component* component);

    /**
    * @brief Get all the Components of the Entity
    *
    * @return a list of Components from the Entity
    */
    std::list<Component*> GetComponents() const { return components; }

    /**
    * @brief Get the Transform of the Entity
    *
    * @return the transform from the Entity
    */
    Transform& GetTransform() { return transform; }

    Scene* GetParentScene() const;

    friend class Scene;
};

#endif // !_ENTITY_H_