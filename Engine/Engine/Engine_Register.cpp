#include "EngineCore.h"

#include "Object.h"
#include "Component.h"
#include "Renderable.h"
#include "FontSprite.h"
#include "AnimatedSprite.h"
#include "Transform.h"
#include "Asset.h"
#include "FontAsset.h"
#include "MusicAsset.h"
#include "SoundAsset.h"
#include "TextureAsset.h"
#include "ICollider.h"
#include "CircleCollider.h"
#include "BoxCollider.h"

void Engine_Register()
{
	REGISTER_TYPECLASS(Object);
	REGISTER_TYPECLASS(Component);
	REGISTER_TYPECLASS(Renderable);
	REGISTER_TYPECLASS(Sprite);
	REGISTER_TYPECLASS(FontSprite);
	REGISTER_TYPECLASS(AnimatedSprite);
	REGISTER_TYPECLASS(Asset);
	REGISTER_TYPECLASS(Transform);
	REGISTER_TYPECLASS(TextureAsset);
	REGISTER_TYPECLASS(FontAsset);
	REGISTER_TYPECLASS(SoundAsset);
	REGISTER_TYPECLASS(MusicAsset);
	REGISTER_TYPECLASS(ICollider);
	REGISTER_TYPECLASS(BoxCollider);
	REGISTER_TYPECLASS(CircleCollider);
}
