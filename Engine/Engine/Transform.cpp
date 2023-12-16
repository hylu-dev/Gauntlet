#include "EngineCore.h"
#include "Transform.h"

IMPLEMENT_DYNAMIC_CLASS(Transform);

Transform::Transform() : position(Vec2::Zero), rotation(0), scale(Vec2(1)) {} //constructor & initialize pos to 0, rotation to 0, scale to 1

void Transform::Initialize() { // empty function but initial values are est in constructor
	//LOG(position.x << ", " << position.y << ", " << rotation)
}

void Transform::Update() {
	Component::Update();
}

void Transform::Load(json::JSON& node)
{
	Component::Load(node);

	if (node.hasKey("Position")) {
		position = vec2_from_json(node["Position"]);
	}
	if (node.hasKey("Rotation"))
	{
		rotation = static_cast<float>(node["Rotation"].ToFloat());
	}
	if (node.hasKey("Scale")) {
		scale = vec2_from_json(node["Scale"]);
	}
}

void Transform::Translate(const Vec2& delta) { // next 3 functions for adjusting position, rotation and scale by a given vector
	position += delta;
}

void Transform::Rotate(float delta) {
	rotation += delta;
}

void Transform::Scale(const Vec2& delta) {
	scale *= delta;
}

