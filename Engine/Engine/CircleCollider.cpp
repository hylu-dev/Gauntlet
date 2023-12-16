#include "EngineCore.h"
#include "CircleCollider.h"

IMPLEMENT_DYNAMIC_CLASS(CircleCollider);

bool CircleCollider::HandleCollision(ICollider* other)
{
	return true;
}

// Set the radius of the collider
void CircleCollider::SetRadius(float radius)
{
	m_radius = radius;
}

float CircleCollider::GetRadius() const
{
	return m_radius;
}

float CircleCollider::GetBroadPhaseRadius() const
{
	return m_radius;
}
ColliderType CircleCollider::GetType() const
{
	return ColliderType::Circle;
}