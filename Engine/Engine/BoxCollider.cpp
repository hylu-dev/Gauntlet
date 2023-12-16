#include "EngineCore.h"
#include "BoxCollider.h"

IMPLEMENT_DYNAMIC_CLASS(BoxCollider);

bool BoxCollider::HandleCollision(ICollider* other) {
	return false;
}

void BoxCollider::SetSize(int width, int height) const
{
	*m_rect = { static_cast<int>(ownerEntity->GetTransform().position.x), static_cast<int>(ownerEntity->GetTransform().position.y), width, height };
}

SDL_Rect BoxCollider::GetBounds() const
{
	if (m_rect == nullptr) {
		return { 0, 0, 0, 0 };
	}
	return *m_rect;
}


float BoxCollider::GetBroadPhaseRadius() const {
	if (m_rect == nullptr) {
		return 0.0f;
	}
	return Vec2(static_cast<float>(m_rect->w), static_cast<float>(m_rect->h)).Magnitude();
}

ColliderType BoxCollider::GetType() const {
	return ColliderType::Box;
}
