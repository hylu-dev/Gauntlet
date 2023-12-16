#include "EngineCore.h"
#include "InputSystem.h"

#define NDEBUG_INPUT

/**
 * @class InputSystem
 * @brief Manages input events and state for keyboard, mouse, and game controllers.
 *
 * @author Brandon Bennie
 *
 * This class handles the initialization, updating, and management of input devices.
 * It processes input events and maintains the state of keyboard keys, mouse buttons,
 * and gamepad buttons and axes. It also supports registering event handlers for different inputs.
 */


InputSystem* InputSystem::instance = nullptr;



/**
 * @brief Constructor for the InputSystem class.
 */
InputSystem::InputSystem()
{
}


/**
 * @brief Destructor for the InputSystem class.
 */

InputSystem::~InputSystem()
{
}

/**
 * @brief Initializes the input system by setting up key and mouse button states and event handlers.
 */

void InputSystem::Initialize()
{
	// Initialize the state of the input system
// Initialize keyStates and mouseButtonStates
// Initialize keyEventHandlers and mouseEventHandlers
	for (int i = 0; i < 512; i++) {
		keyStates[i] = false;
	}
	for (int i = 0; i < 5; i++) {
		mouseButtonStates[i] = false;
	}
}

/**
 * @brief Updates the state of the input system by processing SDL events.
 *
 * This method processes various SDL events to update the state of the input system. It handles keyboard,
 * mouse, and game controller inputs. For keyboard events, it updates the key states based on SDL_KEYDOWN and SDL_KEYUP events.
 * For mouse events, it updates mouse button states. It also handles game controller connection events, button presses,
 * button releases, and axis motions. The method ensures that appropriate event handlers are triggered for each type of input.
 * Additionally, it handles the SDL_QUIT event to perform any necessary actions before quitting.
 *
 * This method should be called every frame to ensure all input events are captured and handled promptly.
 */

void InputSystem::Update()
{

	// Update the state of the input system every frame
	   // Handle events
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
		{
			SDL_Scancode scancode = SDL_GetScancodeFromKey(event.key.keysym.sym);
#ifdef DEBUG_INPUT
			LOG((int)scancode);
#endif
			if (!keyStates[scancode]) {
				keyStates[scancode] = true;
				triggerKeyEvent(event.key.keysym.sym, true); // Assuming triggerKeyEvent still uses SDL_Keycode
			}
		}
		break;
		case SDL_KEYUP:
		{
			SDL_Scancode scancode = SDL_GetScancodeFromKey(event.key.keysym.sym);
			if (keyStates[scancode]) {
				keyStates[scancode] = false;
				triggerKeyEvent(event.key.keysym.sym, false); // Assuming triggerKeyEvent still uses SDL_Keycode
			}
		}
		break;
		// Handle other events as before...

		case SDL_MOUSEBUTTONDOWN:
			if (!mouseButtonStates[event.button.button]) { // Only if the button was previously up
				mouseButtonStates[event.button.button] = true;
				triggerMouseEvent(event.button.button, true);
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (mouseButtonStates[event.button.button]) { // Only if the button was previously down
				mouseButtonStates[event.button.button] = false;
				triggerMouseEvent(event.button.button, false);
			}
			break;
		case SDL_CONTROLLERDEVICEADDED:
			gamepadId = event.cdevice.which;
			SDL_GameControllerOpen(gamepadId);
			break;

		case SDL_CONTROLLERBUTTONDOWN:
			handleGamepadButton(event.cbutton.which, static_cast<SDL_GameControllerButton>(event.cbutton.button), true);
			break;
		case SDL_CONTROLLERBUTTONUP:
			handleGamepadButton(event.cbutton.which, static_cast<SDL_GameControllerButton>(event.cbutton.button), false);
			break;
		case SDL_CONTROLLERAXISMOTION:
			handleGamepadAxis(event.caxis.which, static_cast<SDL_GameControllerAxis>(event.caxis.axis), event.caxis.value);
			break;
		case SDL_QUIT:

			if (quitEventHandler) {
				quitEventHandler();
			}

			break;
		}
	}
}

/**
 * @brief Triggers the appropriate key event handlers based on the key action.
 *
 * @param key The SDL_Keycode of the key that was pressed or released.
 * @param pressed A boolean indicating whether the key was pressed (true) or released (false).
 */

void InputSystem::triggerKeyEvent(SDL_Keycode key, bool pressed) {
	auto it = pressed ? keyPressHandlers.find(key) : keyReleaseHandlers.find(key);
	auto end = pressed ? keyPressHandlers.end() : keyReleaseHandlers.end();

	if (it != end) {
		it->second();
	}
}


/**
 * @brief Triggers the appropriate mouse event handlers based on the mouse button action.
 *
 * @param button The button index that was pressed or released.
 * @param pressed A boolean indicating whether the button was pressed (true) or released (false).
 */

void InputSystem::triggerMouseEvent(Uint8 button, bool pressed) {
	auto it = pressed ? mousePressHandlers.find(button) : mouseReleaseHandlers.find(button);
	auto end = pressed ? mousePressHandlers.end() : mouseReleaseHandlers.end();

	if (it != end) {
		it->second();
	}
}

/**
 * @brief Checks if the specified keys are currently pressed.
 *
 * @param keys A vector of SDL_Keycode representing the keys to check.
 * @return True if all specified keys are pressed, false otherwise.
 */

bool InputSystem::areKeysPressed(const std::vector<SDL_Keycode>& keys) const
{
	for (auto key : keys) {
		if (!isKeyPressed(key))
			return false;
	}
	return true;
}


/**
 * @brief Checks if a specific key is currently pressed.
 *
 * This function converts the given SDL_Keycode to its corresponding SDL_Scancode and checks if this key is currently pressed.
 * It's designed to handle keys in a layout-independent manner by using scancodes.
 * The function also includes a safety check to handle out-of-range scancode values,
 * ensuring stability and preventing access violations in the keyStates array.
 *
 * @param keycode The SDL_Keycode of the key to check. This is the symbol on the key and is layout dependent.
 * @return True if the specified key is pressed, false otherwise. For out-of-range keys, it always returns false.
 */

bool InputSystem::isKeyPressed(SDL_Keycode keycode) const {
	const SDL_Scancode key = SDL_GetScancodeFromKey(keycode);
	if (key >= 512)
	{
#ifdef DEBUG_INPUT
		LOG("Unhandled Input (key out of range): " << key);
#endif
		return false;
	}
	return keyStates[key];
}



/**
 * @brief Checks if a specific mouse button is currently pressed.
 *
 * @param button The index of the mouse button to check.
 * @return True if the specified mouse button is pressed, false otherwise.
 */

bool InputSystem::isMouseButtonPressed(Uint8 button) const {
	return mouseButtonStates[button];
}


/**
 * @brief Registers a handler for key events.
 *
 * @param key The SDL_Keycode for which the handler is being registered.
 * @param onPress Indicates whether the handler is for key press (true) or key release (false) events.
 * @param handler The function to call when the event occurs.
 */

void InputSystem::registerKeyEventHandler(SDL_Keycode key, bool onPress, std::function<void()> handler) {
	if (onPress) {
		keyPressHandlers[key] = handler;
	}
	else {
		keyReleaseHandlers[key] = handler;
	}
}

/**
 * @brief Handles the connection of a gamepad controller.
 *
 * This method is responsible for handling the connection of gamepad controllers. When a gamepad is connected,
 * it uses the SDL_GameControllerOpen function to initialize the gamepad controller with the given joystick index.
 * Upon successful initialization, the gamepad controller is added to the `gamepadMap`, associating the joystick index
 * with the SDL_GameController object. This allows the input system to manage and interact with the connected gamepads efficiently.
 *
 * @param joystickIndex The index of the joystick representing the connected gamepad. This index is used to identify and open the gamepad controller.
 */

void InputSystem::handleGamepadConnection(int joystickIndex) {
	SDL_GameController* gamepad = SDL_GameControllerOpen(joystickIndex);
	if (gamepad) {
		InputSystem::gamepadMap[joystickIndex] = gamepad;
	}
}

/**
 * @brief Registers a handler for mouse button events.
 *
 * @param button The index of the mouse button for which the handler is being registered.
 * @param onPress Indicates whether the handler is for button press (true) or button release (false) events.
 * @param handler The function to call when the event occurs.
 */

void InputSystem::registerMouseEventHandler(Uint8 button, bool onPress, std::function<void()> handler) {
	if (onPress) {
		mousePressHandlers[button] = handler;
	}
	else {
		mouseReleaseHandlers[button] = handler;
	}
}

/**
 * @brief Initializes gamepad controllers.
 *
 * Detects and opens all available game controllers.
 */

void InputSystem::initializeGamepads() {
	int numJoysticks = SDL_NumJoysticks();
	for (int i = 0; i < numJoysticks; ++i) {
		if (SDL_IsGameController(i)) {
			SDL_GameController* controller = SDL_GameControllerOpen(i);
			if (controller) {
				// Store the controller in a container if needed
			}
			else {
				SDL_Log("Could not open gamecontroller %i: %s", i, SDL_GetError());
			}
		}
	}
}

/**
 * @brief Handles button press/release events for gamepads.
 * *
 * * @param joystickID The ID of the joystick.
 * * @param button The gamepad button that was pressed/released.
 * * @param pressed True if the button was pressed, false if it was released.
 * */
void InputSystem::handleGamepadButton(SDL_JoystickID joystickID, SDL_GameControllerButton button, bool pressed) {
	// You might want to keep track of the state of each button for each gamepad
	gamepadButtonStates[joystickID][button] = pressed;

	// Trigger gamepad button event callbacks if any
	if (gamepadButtonEventHandlers.find(joystickID) != gamepadButtonEventHandlers.end() &&
		gamepadButtonEventHandlers[joystickID].find(button) != gamepadButtonEventHandlers[joystickID].end()) {
		gamepadButtonEventHandlers[joystickID][button](pressed);
	}

	// Other game logic related to gamepad button press/release...
}

/**
 * @brief Registers a handler for gamepad button events.
 *
 * @param joystickID The ID of the joystick for which the handler is being registered.
 * @param button The gamepad button for which the handler is being registered.
 * @param handler The function to call when the event occurs.
 */

void InputSystem::registerGamepadButtonEventHandler(SDL_JoystickID joystickID, SDL_GameControllerButton button, std::function<void(bool)> handler)
{

	gamepadButtonEventHandlers[joystickID][button] = handler;
}

/**
 * @brief Checks if a specific gamepad button is currently pressed.
 *
 * @param joystickID The ID of the joystick.
 * @param button The gamepad button to check.
 * @return True if the specified gamepad button is pressed, false otherwise.
 */

bool InputSystem::isGamepadButtonPressed(SDL_JoystickID joystickID, SDL_GameControllerButton button) const {
	auto joystickIt = gamepadButtonStates.find(joystickID);
	if (joystickIt != gamepadButtonStates.end()) {
		auto buttonIt = joystickIt->second.find(button);
		if (buttonIt != joystickIt->second.end()) {
			return buttonIt->second;
		}
	}
	return false; // Default to not pressed if not found
}

/*
*
* @brief Handles axis motion events for gamepads.
*
* @param joystickID The ID of the joystick.
* @param axis The gamepad axis that moved.
* @param value The new position of the axis.
*/

void InputSystem::handleGamepadAxis(SDL_JoystickID joystickID, SDL_GameControllerAxis axis, Sint16 value) {
	// Update the state of the axis
	gamepadAxisStates[joystickID][axis] = value;

	// Perform any actions based on the axis movement.
	// This might involve updating game logic, character movement, camera control, etc.
	// For example:
	if (axis == SDL_CONTROLLER_AXIS_LEFTX || axis == SDL_CONTROLLER_AXIS_LEFTY) {
		// Handle left analog stick movement




	}
	else if (axis == SDL_CONTROLLER_AXIS_RIGHTX || axis == SDL_CONTROLLER_AXIS_RIGHTY) {

		// Handle right analog stick movement


	}

	// You can also call specific callbacks or notify observers if your design uses them


}
/**
* @brief Retrieves the state of a gamepad axis.
*
* @param joystickID The ID of the joystick.
* @param axis The gamepad axis to check.
* @return The normalized position of the axis, ranging from -1.0 to 1.0.
*/

float InputSystem::getGamepadAxisState(SDL_JoystickID joystickID, SDL_GameControllerAxis axis) const {
	auto joystickIt = gamepadAxisStates.find(joystickID);
	if (joystickIt != gamepadAxisStates.end()) {
		auto axisIt = joystickIt->second.find(axis);
		if (axisIt != joystickIt->second.end()) {
			// Normalize the value to be between -1.0 and 1.0
			return axisIt->second / 32767.0f;
		}
	}

	// Return 0.0 or an appropriate default value if the axis state is not found
	return 0.0f;
}
/*
*
* @brief Registers a handler for the quit event.
*
* @param handler The function to call when a quit event occurs.
*/

void InputSystem::registerQuitEventHandler(std::function<void()> handler) {
	quitEventHandler = handler;

}

/**
 * @brief Handles the quit event.
 *
 * Calls the registered quit event handler, if any.
 */

void InputSystem::handleQuitEvent() {
	if (quitEventHandler) {
		quitEventHandler();
	}
	else {
		// Default quit behavior if no handler is set
		// For example, you might set a flag to indicate that the game should exit
	}


}
