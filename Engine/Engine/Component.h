#pragma once

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "Object.h"

class Entity;

class Component : public Object
{
    DECLARE_ABSTRACT_DERIVED_CLASS(Component, Object)

protected:
    Entity* ownerEntity = nullptr;

public:
    Component() = default;
    ~Component() override = default;

    void Initialize() override;
    // Change to pure virtual function
    virtual void Update() {
#ifdef DEBUG_COMPONENT
        LOG("Component Update called on " << this->uid)
#endif
    };
    void Load(json::JSON& node) override;
    void Destroy() override;

    void SetOwner(Entity* owner);
    Entity* GetOwner() const;

    friend class Entity; // To allow Entity to access protected/private members
};

#endif // !_COMPONENT_H_
