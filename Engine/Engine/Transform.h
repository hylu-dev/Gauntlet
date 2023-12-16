#pragma once

#include "Component.h"
#include "EngineMath.h"  

class Transform : public Component {
    DECLARE_DYNAMIC_DERIVED_CLASS(Transform, Component);
public:
    Transform();

    //overriding virtual functions from Component
    void Initialize() override;
    void Update() override;
    void Load(json::JSON& node) override;

    //xyz properties, angle in degrees or radians
    Vec2 position;
    float rotation;
    Vec2 scale;

    //manipulating transform
    void Translate(const Vec2& delta);
    void Rotate(float delta);
    void Scale(const Vec2& delta);
};