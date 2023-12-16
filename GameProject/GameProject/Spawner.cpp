#include "GameCore.h"
#include "Spawner.h"
#include "Enemy.h"
#include "Sprite.h"

#define NDEBUG_SPAWNER

IMPLEMENT_DYNAMIC_CLASS(Spawner)

void Spawner::Initialize()
{
    Component::Initialize();
    start_pos = ownerEntity->GetTransform().position;
    ownerEntity->SetName("Spawner");
}
void Spawner::Update() {
    if (spawned) {
        spawnCounter += Time::Instance().DeltaTime();
    }

    if (spawnCounter > spawnCooldown) {
        spawned = false;
        spawnCounter = 0;
    }

    if (!spawned) {
        spawned = true;
        spawnCounter = 0;

        Entity* newEnemy = ownerEntity->GetParentScene()->CreateEntity();
        newEnemy->GetTransform().position = start_pos;
        Enemy* enemy = (Enemy*)newEnemy->CreateComponent("Enemy");
        Sprite* sprite = (Sprite*)newEnemy->CreateComponent("Sprite");
        sprite->SetTextureAsset(
            (TextureAsset*)AssetManager::Get().GetAsset("bat")
        );
    }
}
void Spawner::Load(json::JSON& node)
{
    Component::Load(node);
}