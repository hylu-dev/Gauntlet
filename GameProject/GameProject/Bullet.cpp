#include "GameCore.h"
#include "Bullet.h"

#define NDEBUG_BULLET

IMPLEMENT_DYNAMIC_CLASS(Bullet)

#include "AnimatedSprite.h"

void Bullet::Initialize()
{
    Component::Initialize();
    start_pos = ownerEntity->GetTransform().position;
    collider = (BoxCollider*)ownerEntity->CreateComponent("BoxCollider");
    ownerEntity->SetName("Bullet");
}
void Bullet::Update() {
    // Move the player
    ownerEntity->GetTransform().position += direction * (speed * Time::Instance().DeltaTime());

    if (collider == nullptr)
    {
        return;
    }
    for (const auto& other : collider->OnCollisionEnter())
    {
        if (other->GetOwner()->GetName() != "Enemy")
        {
            continue;
        }
        ownerEntity->GetParentScene()->RemoveEntity(uid);
    }
}
void Bullet::Load(json::JSON& node)
{
    Component::Load(node);
    if (node.hasKey("Speed"))
    {
        speed = static_cast<float>(node.at("Speed").ToFloat());
    }
}

void Bullet::SetTarget(Vec2 dir) {
    direction = dir - ownerEntity->GetTransform().position;
    if (direction != Vec2::Zero) {
        direction.Normalize();
    }
}
