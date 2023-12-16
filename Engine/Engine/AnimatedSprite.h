#pragma once

#ifndef _ANIMATED_SPRITE_H_
#define _ANIMATED_SPRITE_H_

#include "Sprite.h"

/**
 * @class AnimatedSprite
 *
 * AnimatedSprite is a derived class of Sprite that renders an animation using a spritesheet
 */
class AnimatedSprite : public Sprite {
	DECLARE_DYNAMIC_DERIVED_CLASS(AnimatedSprite, Sprite);
public:
	int spriteSheetRows = 1;
	int spriteSheetColumns = 1;

	int spriteWidth = 0;
	int spriteHeight = 0;

	int totalFrames = 0;
	int defaultFrameNumber = 0;

	int currentFrame = 0;
	float frameDelay = 0.05f;

private:
	float frameCounter = 0;
	bool running = true;
	bool loop = true;

	/**
	 * @brief Initializes any variables as needed
	 *
	 */
	void Initialize() override;

	/**
	 * @brief Cleans up any memory and pointers as needed
	 *
	 */
	void Destroy() override;

	/**
	 * @brief Updates the animation frame to render based off of game time
	 *
	 */
	void Update() override;

	void Load(json::JSON&) override;

public:
	/**
	 * @brief AnimatedSprite constructor
	 *
	 */
	AnimatedSprite() = default;

	/**
	 * @brief AnimatedSprite destructor
	 *
	 */
	~AnimatedSprite() override = default;

	/**
	 * @brief Setter for how to parse the spritesheet
	 *
	 * @param rows How many rows from the spritesheet to parse
	 * @param cols How many columns from the spritesheet to parse
	 * @param frames How many frames/sprites make up this animation
	 */
	void SetSpriteSheet(int rows, int cols, int frames);

	/**
	 * @brief Restart the current animation frame to 0
	 */
	void Restart();

	/**
	 * @brief Stops the animation, reverting to the default frame
	 */
	void Stop();

	/**
	 * @brief Plays the animation from the current frame
	 */
	void Play();

	/**
	 * @brief Pauses the animation on the current frame
	 */
	void Pause();
};

#endif