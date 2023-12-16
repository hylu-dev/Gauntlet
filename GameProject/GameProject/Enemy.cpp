#include "GameCore.h"
#include "Enemy.h"

#define NDEBUG_ENEMY

IMPLEMENT_DYNAMIC_CLASS(Enemy)

#include "AnimatedSprite.h"

void Enemy::Initialize()
{
    Component::Initialize();
    start_pos = ownerEntity->GetTransform().position;
    collider = (BoxCollider*)ownerEntity->GetComponent("BoxCollider");
}
void Enemy::Update() {
    target = SceneManager::Get().FindEntityByName("Player").front()->GetTransform();
    Vec2 dir = target.position - ownerEntity->GetTransform().position;

    // Normalize the direction vector if it's not zero
    if (dir != Vec2::Zero) {
        dir.Normalize();
    }

    // Move the player
    ownerEntity->GetTransform().position += dir * (speed * Time::Instance().DeltaTime());

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

        Scene* current_scene = SceneManager::Get().GetActiveScene();
        if (SceneManager::Get().SetActiveScene(game_over_scene))
        {
            current_scene->isEnabled = false;
        }

        ownerEntity->GetTransform().position = start_pos;
    }
}
void Enemy::Load(json::JSON& node)
{
    Component::Load(node);
    if (node.hasKey("Speed"))
    {
        speed = static_cast<float>(node.at("Speed").ToFloat());
    }
}