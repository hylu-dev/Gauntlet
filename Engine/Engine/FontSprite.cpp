/*
* @FontSprite
*
* Allows the user to display text onto the SDL Window. They can control the text content,
* font used by providing a new asset to use, and the texts display color.
*/

#include "EngineCore.h"
#include "FontSprite.h"

#include <utility>
#include "SDL_ttf.h"
#include "RenderSystem.h"
#include "Entity.h"
#include "FontAsset.h"
#include "AssetManager.h"

IMPLEMENT_DYNAMIC_CLASS(FontSprite);

/*
* @Initialize
*
* There is no default font. You will see nothing until you provide a font to the FontSprite
*/
void FontSprite::Initialize() {
	Renderable::Initialize();
	RegenerateOutput();
}

void FontSprite::Update() {
	Renderable::Update(); RegenerateOutput();
	if (output != nullptr) {
		SDL_QueryTexture(output, nullptr, nullptr, &outputSizing.x, &outputSizing.y);
	}

	const Transform& transform = ownerEntity->GetTransform();

	fontRect = { static_cast<int>(transform.position.x), static_cast<int>(transform.position.y),
		( static_cast<int>(outputSizing.x * transform.scale.x)), (static_cast<int>(outputSizing.y * transform.scale.y)) };

	flip = static_cast<SDL_RendererFlip>((transform.scale.x < 0) | ((transform.scale.y < 0) << 1));
}

void FontSprite::Destroy() {
	SDL_DestroyTexture(output);
	Renderable::Destroy();
}

void FontSprite::Render() {
	if (output == nullptr) {
		return;
	}

	const Transform& transform = ownerEntity->GetTransform();
    const auto size = IVec2(transform.scale * outputSizing);
    const IVec2 pos = transform.position - size / 2;
    fontRect = {
        pos.x,
        pos.y,
        size.x,
        size.y
    };

	SDL_RenderCopyEx(
		&RenderSystem::Instance().GetRenderer(),
		output,
		nullptr,
		&fontRect,
		(double)transform.rotation,
		nullptr,
		flip
	);
}

void FontSprite::Save(json::JSON& document) const {
	document["Text"] = text;

	json::JSON subObject = json::JSON::Object();
	subObject["R"] = fontColor.r;
	subObject["G"] = fontColor.g;
	subObject["B"] = fontColor.b;
	subObject["A"] = fontColor.a;

	document["FontColor"] = subObject;

	document["Font"] = font->GetGuid();
}

void FontSprite::Load(json::JSON& node) {
	Renderable::Load(node);

	if (node.hasKey("Text")) {
		text = node["Text"].ToString();
	}

	if (node.hasKey("FontColor")) {
		json::JSON subObject = node["FontColor"];
		fontColor.r = static_cast<Uint8>(subObject["R"].ToInt());
		fontColor.g = static_cast<Uint8>(subObject["G"].ToInt());
		fontColor.b = static_cast<Uint8>(subObject["B"].ToInt());
		fontColor.a = static_cast<Uint8>(subObject["A"].ToInt());
	}

	std::string guid = node["Font"].ToString();

	font = (FontAsset*)(AssetManager::Get().GetAsset(guid));
	RegenerateOutput();
}

/*
* @SetText
*
* Takes a string for the display text and regenerates the font texture.
*/
void FontSprite::SetText(std::string text) {
	this->text = std::move(text);
	RegenerateOutput();
}

/*
* @SetFont
*
* Takes a TextureAsset for the new font and regenerates the font texture.
*/
void FontSprite::SetFont(FontAsset* font) {
	this->font = font;
	RegenerateOutput();
}

/*
* @SetFontColor
*
* Takes RGB values along with an Alpha and regenerates the font texture.
*/
void FontSprite::SetFontColor(int r, int g, int b, int a) {
	fontColor.r = static_cast<Uint8>(r);
	fontColor.g = static_cast<Uint8>(g);
	fontColor.b = static_cast<Uint8>(b);
	fontColor.a = static_cast<Uint8>(a);
	RegenerateOutput();
}

/*
* @RegenerateOutput
*
* Used by other methods to regenerate the SDL_Texture after things like text, font or color
* have changed.
*/
void FontSprite::RegenerateOutput() {
	if (font == nullptr) {
		return;
	}

	SDL_Surface* textSurface = TTF_RenderText_Solid((*font).GetFont(), text.c_str(), fontColor);
	SDL_DestroyTexture(output);
	output = SDL_CreateTextureFromSurface(&RenderSystem::Instance().GetRenderer(), textSurface);
	SDL_FreeSurface(textSurface);
}