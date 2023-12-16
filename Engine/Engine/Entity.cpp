#include "EngineCore.h"
#include "Entity.h"
#include "Component.h"
#include "Transform.h"


#define NDEBUG_ENTITY_GET_COMPONENT

IMPLEMENT_DYNAMIC_CLASS(Entity)

void Entity::Initialize()
{
	transform.Initialize();

	for (const auto component : components)
	{
		component->Initialize();
	}
}

void Entity::Load(json::JSON& node)
{
	Object::Load(node);

	// Load Transform
	if (node.hasKey("Transform"))
	{
		json::JSON transform_json = node["Transform"];
		transform.Load(transform_json);
	}

	// Load the components
	if (node.hasKey("Components"))
	{
		json::JSON components_json = node["Components"];

		for (json::JSON& component_json : components_json.ArrayRange())
		{
			std::string component_name = component_json["ClassName"].ToString();
			Component* component = CreateComponent(component_name);
			component->Load(component_json["ClassData"]);
		}
	}
}

void Entity::Update()
{
	transform.Update();

	for (const auto component : components)
	{
		component->Update();
	}
}

void Entity::PreUpdate()
{
	for (auto component : componentsToAdd)
	{
		components.push_back(component);
		component->Initialize();
	}
	componentsToAdd.clear();
}

void Entity::PostUpdate()
{
	for (auto component : componentsToRemove)
	{
		components.remove(component);
		delete component;
	}
	componentsToRemove.clear();
}

void Entity::Destroy()
{
	for (const auto component : components)
	{
		delete component;
	}
	components.clear();
}

bool Entity::HasComponent(const std::string& componentName) const
{
	for (const auto component : components)
	{
		if (component->GetDerivedClassName() == componentName)
		{
			return true;
		}
	}
	return false;
}

void Entity::AddComponents(const std::vector<std::string>& componentList)
{
	for (const auto& component : componentList)
	{
		CreateComponent(component);
	}
}

Component* Entity::GetComponent(const std::string& componentName) const
{
	for (auto component : components)
	{
#ifdef DEBUG_ENTITY_GET_COMPONENT
		LOG(componentName << ", " << component->GetDerivedClassName())
#endif
		if (component->GetDerivedClassName() == componentName)
		{
			return component;
		}
	}
	return nullptr;
}

Component* Entity::CreateComponent(const std::string& componentName)
{
    const auto component = (Component*)CreateObject(componentName.c_str());
	component->ownerEntity = this;
	componentsToAdd.push_back(component);
	return component;
}

bool Entity::RemoveComponent(const Component* component)
{
	for (auto c : components)
	{
		if (c == component)
		{
			componentsToRemove.push_back(c);
			return true;
		}
	}
	return false;
}

Scene* Entity::GetParentScene() const
{
	ASSERT(ownerScene != nullptr, "ownerScene was null :(");
	return ownerScene;
}
