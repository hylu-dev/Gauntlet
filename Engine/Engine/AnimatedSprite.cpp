#include "EngineCore.h"
#include "AnimatedSprite.h"
#include "RenderSystem.h"
#include "EngineTime.h"
#include "Entity.h"

IMPLEMENT_DYNAMIC_CLASS(AnimatedSprite);

void AnimatedSprite::Initialize() {
    Sprite::Initialize();
}

void AnimatedSprite::Destroy() {
    Sprite::Destroy();
}

void AnimatedSprite::Update() {
    Sprite::Update();

    if (!running) return;
    frameCounter += Time::Instance().DeltaTime();

    sourceRect.x = spriteWidth * (currentFrame % spriteSheetColumns);
    sourceRect.y = spriteHeight * (currentFrame / spriteSheetColumns);

    if (currentFrame == totalFrames) {
        if (loop) {
            currentFrame = 0;
        }
        else {
            running = false;
        }
    }
    if (frameCounter >= frameDelay) {
        frameCounter = 0;
        currentFrame++;
    }
}

void AnimatedSprite::Load(json::JSON& node)
{
    Sprite::Load(node);
    if (node.hasKey("SpriteSheet")) {
        json::JSON spriteSheetNode = node["SpriteSheet"];

        if (spriteSheetNode.hasKey("Rows")) {
            spriteSheetRows = spriteSheetNode["Rows"].ToInt();
        }
        if (spriteSheetNode.hasKey("Columns")) {
            spriteSheetColumns = spriteSheetNode["Columns"].ToInt();
        }
        if (spriteSheetNode.hasKey("Frames")) {
            totalFrames = spriteSheetNode["Frames"].ToInt();
        }

        this->SetSpriteSheet(spriteSheetRows, spriteSheetColumns, totalFrames);
    }
}

void AnimatedSprite::SetSpriteSheet(int rows, int cols, int _totalFrames) {
    spriteSheetRows = rows;
    spriteSheetColumns = cols;
    totalFrames = _totalFrames;

    const auto texRect = texture->GetDimensions();

    size = texRect / IVec2(cols, rows);

    spriteWidth = texRect.x / cols;
    spriteHeight = texRect.y / rows;

    sourceRect.w = size.x;
    sourceRect.h = size.y;
}

void AnimatedSprite::Restart() {
    currentFrame = defaultFrameNumber;
}

void AnimatedSprite::Stop() {
    currentFrame = defaultFrameNumber;
    running = false;
}

void AnimatedSprite::Play() {
    running = true;
}

void AnimatedSprite::Pause() {
    running = false;
}