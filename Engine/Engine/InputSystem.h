#pragma once
#include "Engine.h"
#include <SDL_keycode.h>
#include <SDL.h>

class InputSystem
{


public:
	// Singleton pattern: 

	friend class Engine;

	static InputSystem& Instance() {

		if (instance == nullptr)
		{
			instance = new InputSystem();

		}
		return *instance;
	}
	InputSystem();
	~InputSystem();
	void Initialize();
	void Update();

	void triggerKeyEvent(SDL_Keycode key, bool pressed);

	void triggerMouseEvent(Uint8 button, bool pressed);

	bool isKeyPressed(SDL_Keycode key) const;
	bool areKeysPressed(const std::vector<SDL_Keycode>& keys) const;
	bool isMouseButtonPressed(Uint8 button) const;

	bool isGamepadButtonPressed(SDL_JoystickID joystickID, SDL_GameControllerButton button) const;

	void handleGamepadAxis(SDL_JoystickID joystickID, SDL_GameControllerAxis axis, Sint16 value);

	float getGamepadAxisState(SDL_JoystickID joystickID, SDL_GameControllerAxis axis) const;

	void registerQuitEventHandler(std::function<void()> handler);

	void handleQuitEvent();

	void registerKeyEventHandler(SDL_Keycode key, bool onPress, std::function<void()> handler);
	void handleGamepadConnection(int joystickIndex);
	void registerMouseEventHandler(Uint8 button, bool onPress, std::function<void()> handler);

	void initializeGamepads();

	void handleGamepadButton(SDL_JoystickID joystickID, SDL_GameControllerButton button, bool pressed);
	void registerGamepadButtonEventHandler(SDL_JoystickID joystickID,
		SDL_GameControllerButton button,
		std::function<void(bool)> handler);





private:
	bool  keyStates[SDL_NUM_SCANCODES] = { false };
	bool mouseButtonStates[5];
	int gamepadId;
	InputSystem(const InputSystem&) = delete;



	InputSystem operator = (const InputSystem&) = delete;
	static InputSystem* instance;
	std::map<SDL_Keycode, std::function<void()>> keyPressHandlers;
	std::map<SDL_Keycode, std::function<void()>> keyReleaseHandlers;
	std::map<Uint8, std::function<void()>> mousePressHandlers;
	std::map<Uint8, std::function<void()>> mouseReleaseHandlers;
	std::map<SDL_JoystickID, std::map<SDL_GameControllerButton, bool>> gamepadButtonStates;
	std::map<SDL_JoystickID, std::map<SDL_GameControllerButton, std::function<void(bool)>>> gamepadButtonEventHandlers;
	std::map<SDL_JoystickID, std::map<SDL_GameControllerAxis, Sint16>> gamepadAxisStates;
	std::map<int, SDL_GameController*> gamepadMap;
	std::map<SDL_JoystickID, std::map<SDL_GameControllerAxis, std::function<void(Sint16)>>> gamepadAxisEventHandlers;

	

	std::function<void()> quitEventHandler;

};


