#include "GameCore.h"
#include "Player.h"
#include "Bullet.h"

#define NDEBUG_PLAYER

IMPLEMENT_DYNAMIC_CLASS(Player)

#include "AnimatedSprite.h"

void Player::Initialize()
{
    Component::Initialize();
    start_pos = ownerEntity->GetTransform().position;
    collider = (BoxCollider*)ownerEntity->GetComponent("BoxCollider");

    up = (TextureAsset*)AssetManager::Get().GetAsset("link_up");
    left = (TextureAsset*)AssetManager::Get().GetAsset("link_left");
    right = (TextureAsset*)AssetManager::Get().GetAsset("link_right");
    down = (TextureAsset*)AssetManager::Get().GetAsset("link_down");

}
void Player::Update() {
    this->HandleMovement();
    this->HandleFire();



    if (collider == nullptr)
    {
        return;
    }
    for (const auto& other: collider->OnCollisionEnter())
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

void Player::HandleFire() {
    if (fired) {
        fireCounter += Time::Instance().DeltaTime();
    }

    if (fireCounter > fireCooldown) {
        fired = false;
        fireCounter = 0;
    }

    const InputSystem& input = InputSystem::Instance();
    if (input.isMouseButtonPressed(SDL_BUTTON_LEFT) && !fired) {
        fired = true;
        fireCounter = 0;

        int mouseX, mouseY;
        const Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
        Vec2 mousePos = { (float)mouseX, (float)mouseY };

        Entity* newBullet = ownerEntity->GetParentScene()->CreateEntity();
        Bullet* bullet = (Bullet*)newBullet->CreateComponent("Bullet");
        Sprite* sprite = (Sprite*)newBullet->CreateComponent("Sprite");
        sprite->SetTextureAsset(
            (TextureAsset*)AssetManager::Get().GetAsset("bullet")
        );
        newBullet->GetTransform().position = ownerEntity->GetTransform().position;

        bullet->SetTarget(mousePos);
    }
}

void Player::HandleMovement() {
    Vec2 dir = Vec2::Zero;
    const InputSystem& input = InputSystem::Instance();

    // Handle horizontal movement
    if (input.isKeyPressed(SDLK_LEFT) || input.isKeyPressed(SDLK_a) || input.isGamepadButtonPressed(0, SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
        dir.x -= 1;
        AnimatedSprite* sprite = (AnimatedSprite*)ownerEntity->GetComponent("AnimatedSprite");
        if (sprite->texture != left) {
            sprite->SetTextureAsset(left);
            sprite->SetSpriteSheet(1, 12, 12);
        }

    }
    if (input.isKeyPressed(SDLK_RIGHT) || input.isKeyPressed(SDLK_d) || input.isGamepadButtonPressed(0, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
        dir.x += 1;
        AnimatedSprite* sprite = (AnimatedSprite*)ownerEntity->GetComponent("AnimatedSprite");
        if (sprite->texture != right) {
            sprite->SetTextureAsset(right);
            sprite->SetSpriteSheet(1, 12, 12);
        }
    }

    // Handle vertical movement
    if (input.isKeyPressed(SDLK_UP) || input.isKeyPressed(SDLK_w) || input.isGamepadButtonPressed(0, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
        dir.y -= 1;
        AnimatedSprite* sprite = (AnimatedSprite*)ownerEntity->GetComponent("AnimatedSprite");
        if (sprite->texture != up) {
            sprite->SetTextureAsset(up);
            sprite->SetSpriteSheet(1, 12, 12);
        }
    }
    if (input.isKeyPressed(SDLK_DOWN) || input.isKeyPressed(SDLK_s) || input.isGamepadButtonPressed(0, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
        dir.y += 1;
        AnimatedSprite* sprite = (AnimatedSprite*)ownerEntity->GetComponent("AnimatedSprite");
        if (sprite->texture != down) {
            sprite->SetTextureAsset(down);
            sprite->SetSpriteSheet(1, 12, 12);
        }
    }

    // Handle gamepad analog stick input
    if (dir == Vec2::Zero) {
        dir.x = input.getGamepadAxisState(0, SDL_CONTROLLER_AXIS_LEFTX);
        dir.y = input.getGamepadAxisState(0, SDL_CONTROLLER_AXIS_LEFTY);
    }

    // Normalize the direction vector if it's not zero
    if (dir != Vec2::Zero) {
        dir.Normalize();
    }

    // Move the player
    ownerEntity->GetTransform().position += dir * (speed * Time::Instance().DeltaTime());
}


void Player::Load(json::JSON& node)
{
    Component::Load(node);
    if (node.hasKey("Speed"))
    {
        speed = static_cast<float>(node.at("Speed").ToFloat());
    }

    if (node.hasKey("DeathScene"))
    {
	    game_over_scene = GetHashCode(node.at("DeathScene").ToString().c_str());
    }
}
