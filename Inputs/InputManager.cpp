#include <SDL2/SDL_events.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_joystick.h>
#include <cstdint>
#include <cstdio>
#include "../Errors.h"
#include "InputManager.h"
#include <string>
#include <SDL2/SDL.h>

InputManager::InputManager() {
	SDL_Init(SDL_INIT_GAMECONTROLLER);
}

InputManager::~InputManager() {
	for (int i = 0; i < _controllers.size(); ++i) {
		SDL_JoystickClose(_controllers[i].joystick);
		_controllers[i].joystick = nullptr;
	}
}

void InputManager::processInput() {
	// updates previous key map via foreach loop
	_previousKeyMap = _keyMap;

	// I wanted to declare this only within the case statement where we are dealing with controllerAxis's,
	// but doing so creates a "compiler error" where the variable is non-accessible to other case statements.
	// I disagree fully with this. This is stupid and dumb. -xm
	float controllerAxis = 0;

	deltaScroll = 0;
	
	SDL_Event _event;
	while (SDL_PollEvent(&_event))
	{
		switch (_event.type) {
			case SDL_QUIT:
				_quitGameCalled = true;
				break;
			case SDL_KEYDOWN:
				_keyMap[_event.key.keysym.sym] = true;
				_lastInput = (SDL_KeyCode)_event.key.keysym.sym;
				break;
			case SDL_KEYUP:
				_keyMap[_event.key.keysym.sym] = false;
				break;
			case SDL_JOYAXISMOTION:
				controllerAxis = _event.jaxis.value; // convert from int to float

				if (controllerAxis < 0) { // add one to negative values so that both positive and negative values are between the same range.
					controllerAxis += 1;
				}
				controllerAxis /= 32767; // divide by integer limit to get a number between -1 and 1.

				switch (_event.jaxis.axis) {
					case 0: // X Axis on main stick
						_controllers[_event.jaxis.which].leftStick.x = controllerAxis;
						break;
					case 1: // Y Axis on main stick
						_controllers[_event.jaxis.which].leftStick.y = controllerAxis;
						break;
					case 2: // X Axis on right stick
						_controllers[_event.jaxis.which].rightStick.x = controllerAxis;
						break;
					case 3: // Y Axis on right stick
						_controllers[_event.jaxis.which].rightStick.y = controllerAxis;
						break;
					case 4: // Left trigger
						_controllers[_event.jaxis.which].triggers.x = controllerAxis;
						break;
					case 5: // Right trigger
						_controllers[_event.jaxis.which].triggers.y = controllerAxis;
						break;
					default:
						Jauntlet::error("WARNING: undefined stick axis detected " + std::to_string(_event.jaxis.axis));
						continue;
				}
				break;
			case SDL_JOYBUTTONDOWN:
				switch (_event.cbutton.button) {
					case SDL_CONTROLLER_BUTTON_Y:
						switch (getController(_event.cbutton.which)->type) {
							case Controller_Type::PLAYSTATION:
							case Controller_Type::XBOX:
							case Controller_Type::AMAZON_LUNA: // Assumption
							case Controller_Type::GOOGLE_STADIA: // Assumption
							case Controller_Type::NIVIDIA_SHIELD: // Assumption
								_keyMap[CONTROLLER_FACE_WEST] = true;
								_lastInput = CONTROLLER_FACE_WEST;
								break;
							case Controller_Type::NINTENDO_SWITCH:
							default:
								_keyMap[CONTROLLER_FACE_NORTH] = true;
								_lastInput = CONTROLLER_FACE_NORTH;
								break;
						}
						break;
					case SDL_CONTROLLER_BUTTON_A:
						switch (getController(_event.cbutton.which)->type) {
							case Controller_Type::PLAYSTATION:
							case Controller_Type::XBOX:
							case Controller_Type::AMAZON_LUNA: // Assumption
							case Controller_Type::GOOGLE_STADIA: // Assumption
							case Controller_Type::NIVIDIA_SHIELD: // Assumption
								_keyMap[CONTROLLER_FACE_NORTH] = true;
								_lastInput = CONTROLLER_FACE_NORTH;
								break;
							case Controller_Type::NINTENDO_SWITCH:
							default:
								_keyMap[CONTROLLER_FACE_EAST] = true;
								_lastInput = CONTROLLER_FACE_EAST;
								break;
						}
						break;
					case SDL_CONTROLLER_BUTTON_B:
						switch (getController(_event.cbutton.which)->type) {
							case Controller_Type::PLAYSTATION:
							case Controller_Type::XBOX:
							case Controller_Type::AMAZON_LUNA: // Assumption
							case Controller_Type::GOOGLE_STADIA: // Assumption
							case Controller_Type::NIVIDIA_SHIELD: // Assumption
								_keyMap[CONTROLLER_FACE_EAST] = true;
								_lastInput = CONTROLLER_FACE_EAST;
								break;
							case Controller_Type::NINTENDO_SWITCH:
							default:
								_keyMap[CONTROLLER_FACE_SOUTH] = true;
								_lastInput = CONTROLLER_FACE_SOUTH;
								break;
						}
						break;
					case SDL_CONTROLLER_BUTTON_X:
						switch (getController(_event.cbutton.which)->type) {
							case Controller_Type::PLAYSTATION:
							case Controller_Type::XBOX:
							case Controller_Type::AMAZON_LUNA: // Assumption
							case Controller_Type::GOOGLE_STADIA: // Assumption
							case Controller_Type::NIVIDIA_SHIELD: // Assumption
								_keyMap[CONTROLLER_FACE_SOUTH] = true;
								_lastInput = CONTROLLER_FACE_SOUTH;
								break;
							case Controller_Type::NINTENDO_SWITCH:
							default:
								_keyMap[CONTROLLER_FACE_WEST] = true;
								_lastInput = CONTROLLER_FACE_WEST;
								break;
						}
						break;
					case SDL_CONTROLLER_BUTTON_BACK:
						_keyMap[CONTROLLER_BACK] = true;
						_lastInput = CONTROLLER_BACK;
						break;
					case SDL_CONTROLLER_BUTTON_GUIDE:
						_keyMap[CONTROLLER_GUIDE] = true;
						_lastInput = CONTROLLER_GUIDE;
						break;
					case SDL_CONTROLLER_BUTTON_START:
						_keyMap[CONTROLLER_START] = true;
						_lastInput = CONTROLLER_START;
						break;
					case SDL_CONTROLLER_BUTTON_LEFTSTICK:
						_keyMap[CONTROLLER_LEFTSTICK] = true;
						_lastInput = CONTROLLER_LEFTSTICK;
						break;
					case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
						_keyMap[CONTROLLER_RIGHTSTICK] = true;
						_lastInput = CONTROLLER_RIGHTSTICK;
						break;
					case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
						_keyMap[CONTROLLER_LEFTSHOULDER] = true;
						_lastInput = CONTROLLER_LEFTSHOULDER;
						break;
					case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
						_keyMap[CONTROLLER_RIGHTSHOULDER] = true;
						_lastInput = CONTROLLER_RIGHTSHOULDER;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_UP:
						_keyMap[CONTROLLER_DPAD_UP] = true;
						_lastInput = CONTROLLER_DPAD_UP;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
						_keyMap[CONTROLLER_DPAD_DOWN] = true;
						_lastInput = CONTROLLER_DPAD_DOWN;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
						_keyMap[CONTROLLER_DPAD_LEFT] = true;
						_lastInput = CONTROLLER_DPAD_LEFT;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
						_keyMap[CONTROLLER_DPAD_RIGHT] = true;
						_lastInput = CONTROLLER_DPAD_RIGHT;
						break;
					case SDL_CONTROLLER_BUTTON_MISC1:
						_keyMap[CONTROLLER_MISC] = true;
						_lastInput = CONTROLLER_MISC;
						break;
					case SDL_CONTROLLER_BUTTON_PADDLE1:
						_keyMap[CONTROLLER_PADDLE1] = true;
						_lastInput = CONTROLLER_PADDLE1;
						break;
					case SDL_CONTROLLER_BUTTON_PADDLE2:
						_keyMap[CONTROLLER_PADDLE2] = true;
						_lastInput = CONTROLLER_PADDLE2;
						break;
					case SDL_CONTROLLER_BUTTON_PADDLE3:
						_keyMap[CONTROLLER_PADDLE3] = true;
						_lastInput = CONTROLLER_PADDLE3;
						break;
					case SDL_CONTROLLER_BUTTON_PADDLE4:
						_keyMap[CONTROLLER_PADDLE4] = true;
						_lastInput = CONTROLLER_PADDLE4;
						break;
					case SDL_CONTROLLER_BUTTON_TOUCHPAD:
						_keyMap[CONTROLLER_TOUCHPAD] = true;
						_lastInput = CONTROLLER_TOUCHPAD;
						break;
					case SDL_CONTROLLER_BUTTON_MAX:
						_keyMap[CONTROLLER_MAX] = true;
						_lastInput = CONTROLLER_MAX;
						break;
					default: // Runs in case of SDL_CONTROLLER_BUTTON_INVALID
						break;
				}
				break;
			case SDL_JOYBUTTONUP:
				switch (_event.cbutton.button) {
					case SDL_CONTROLLER_BUTTON_Y:
						switch (getController(_event.cbutton.which)->type) {
							case Controller_Type::PLAYSTATION:
							case Controller_Type::XBOX:
							case Controller_Type::AMAZON_LUNA: // Assumption
							case Controller_Type::GOOGLE_STADIA: // Assumption
							case Controller_Type::NIVIDIA_SHIELD: // Assumption
								_keyMap[CONTROLLER_FACE_WEST] = false;
								break;
							case Controller_Type::NINTENDO_SWITCH:
							default:
								_keyMap[CONTROLLER_FACE_NORTH] = false;
								break;
						}
						break;
					case SDL_CONTROLLER_BUTTON_A:
						switch (getController(_event.cbutton.which)->type) {
							case Controller_Type::PLAYSTATION:
							case Controller_Type::XBOX:
							case Controller_Type::AMAZON_LUNA: // Assumption
							case Controller_Type::GOOGLE_STADIA: // Assumption
							case Controller_Type::NIVIDIA_SHIELD: // Assumption
								_keyMap[CONTROLLER_FACE_NORTH] = false;
								break;
							case Controller_Type::NINTENDO_SWITCH:
							default:
								_keyMap[CONTROLLER_FACE_EAST] = false;
								break;
						}
						break;
					case SDL_CONTROLLER_BUTTON_B:
						switch (getController(_event.cbutton.which)->type) {
							case Controller_Type::PLAYSTATION:
							case Controller_Type::XBOX:
							case Controller_Type::AMAZON_LUNA: // Assumption
							case Controller_Type::GOOGLE_STADIA: // Assumption
							case Controller_Type::NIVIDIA_SHIELD: // Assumption
								_keyMap[CONTROLLER_FACE_EAST] = false;
								break;
							case Controller_Type::NINTENDO_SWITCH:
							default:
								_keyMap[CONTROLLER_FACE_SOUTH] = false;
								break;
						}
						break;
					case SDL_CONTROLLER_BUTTON_X:
						switch (getController(_event.cbutton.which)->type) {
							case Controller_Type::PLAYSTATION:
							case Controller_Type::XBOX:
							case Controller_Type::AMAZON_LUNA: // Assumption
							case Controller_Type::GOOGLE_STADIA: // Assumption
							case Controller_Type::NIVIDIA_SHIELD: // Assumption
								_keyMap[CONTROLLER_FACE_SOUTH] = false;
								break;
							case Controller_Type::NINTENDO_SWITCH:
							default:
								_keyMap[CONTROLLER_FACE_WEST] = false;
								break;
						}
						break;
					case SDL_CONTROLLER_BUTTON_BACK:
						_keyMap[CONTROLLER_BACK] = false;
						break;
					case SDL_CONTROLLER_BUTTON_GUIDE:
						_keyMap[CONTROLLER_GUIDE] = false;
						break;
					case SDL_CONTROLLER_BUTTON_START:
						_keyMap[CONTROLLER_START] = false;
						break;
					case SDL_CONTROLLER_BUTTON_LEFTSTICK:
						_keyMap[CONTROLLER_LEFTSTICK] = false;
						break;
					case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
						_keyMap[CONTROLLER_RIGHTSTICK] = false;
						break;
					case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
						_keyMap[CONTROLLER_LEFTSHOULDER] = false;
						break;
					case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
						_keyMap[CONTROLLER_RIGHTSHOULDER] = false;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_UP:
						_keyMap[CONTROLLER_DPAD_UP] = false;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
						_keyMap[CONTROLLER_DPAD_DOWN] = false;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
						_keyMap[CONTROLLER_DPAD_LEFT] = false;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
						_keyMap[CONTROLLER_DPAD_RIGHT] = false;
						break;
					case SDL_CONTROLLER_BUTTON_MISC1:
						_keyMap[CONTROLLER_MISC] = false;
						break;
					case SDL_CONTROLLER_BUTTON_PADDLE1:
						_keyMap[CONTROLLER_PADDLE1] = false;
						break;
					case SDL_CONTROLLER_BUTTON_PADDLE2:
						_keyMap[CONTROLLER_PADDLE2] = false;
						break;
					case SDL_CONTROLLER_BUTTON_PADDLE3:
						_keyMap[CONTROLLER_PADDLE3] = false;
						break;
					case SDL_CONTROLLER_BUTTON_PADDLE4:
						_keyMap[CONTROLLER_PADDLE4] = false;
						break;
					case SDL_CONTROLLER_BUTTON_TOUCHPAD:
						_keyMap[CONTROLLER_TOUCHPAD] = false;
						break;
					case SDL_CONTROLLER_BUTTON_MAX:
						_keyMap[CONTROLLER_MAX] = false;
						break;
					default: // Runs in case of SDL_CONTROLLER_BUTTON_INVALID
						break;
				}
				break;
			case SDL_JOYHATMOTION:
				if (_event.jhat.type == 1538) { // this is the identifier for the D-pad of a controller
					switch (_event.jhat.value) {
						case SDL_HAT_LEFTUP:
							_controllers[_event.jhat.which].dPad = glm::vec2(-1, 1);
							_keyMap[CONTROLLER_DPAD_LEFT] = true;
							_keyMap[CONTROLLER_DPAD_UP] = true;
							_keyMap[CONTROLLER_DPAD_DOWN] = false;
							_keyMap[CONTROLLER_DPAD_RIGHT] = false;
							break;
						case SDL_HAT_UP:
							_controllers[_event.jhat.which].dPad = glm::vec2(0, 1);
							_keyMap[CONTROLLER_DPAD_LEFT] = false;
							_keyMap[CONTROLLER_DPAD_UP] = true;
							_keyMap[CONTROLLER_DPAD_DOWN] = false;
							_keyMap[CONTROLLER_DPAD_RIGHT] = false;
							break;
						case SDL_HAT_RIGHTUP:
							_controllers[_event.jhat.which].dPad = glm::vec2(1, 1);
							_keyMap[CONTROLLER_DPAD_LEFT] = false;
							_keyMap[CONTROLLER_DPAD_UP] = true;
							_keyMap[CONTROLLER_DPAD_DOWN] = false;
							_keyMap[CONTROLLER_DPAD_RIGHT] = true;
							break;
						case SDL_HAT_LEFT:
							_controllers[_event.jhat.which].dPad = glm::vec2(-1, 0);
							_keyMap[CONTROLLER_DPAD_LEFT] = true;
							_keyMap[CONTROLLER_DPAD_UP] = false;
							_keyMap[CONTROLLER_DPAD_DOWN] = false;
							_keyMap[CONTROLLER_DPAD_RIGHT] = false;
							break;
						case SDL_HAT_RIGHT:
							_controllers[_event.jhat.which].dPad = glm::vec2(1, 0);
							_keyMap[CONTROLLER_DPAD_LEFT] = false;
							_keyMap[CONTROLLER_DPAD_UP] = false;
							_keyMap[CONTROLLER_DPAD_DOWN] = false;
							_keyMap[CONTROLLER_DPAD_RIGHT] = true;
							break;
						case SDL_HAT_LEFTDOWN:
							_controllers[_event.jhat.which].dPad = glm::vec2(-1, -1);
							_keyMap[CONTROLLER_DPAD_LEFT] = true;
							_keyMap[CONTROLLER_DPAD_UP] = false;
							_keyMap[CONTROLLER_DPAD_DOWN] = true;
							_keyMap[CONTROLLER_DPAD_RIGHT] = false;
							break;
						case SDL_HAT_DOWN:
							_controllers[_event.jhat.which].dPad = glm::vec2(0, -1);
							_keyMap[CONTROLLER_DPAD_LEFT] = false;
							_keyMap[CONTROLLER_DPAD_UP] = false;
							_keyMap[CONTROLLER_DPAD_DOWN] = true;
							_keyMap[CONTROLLER_DPAD_RIGHT] = false;
							break;
						case SDL_HAT_RIGHTDOWN:
							_controllers[_event.jhat.which].dPad = glm::vec2(1, -1);
							_keyMap[CONTROLLER_DPAD_LEFT] = false;
							_keyMap[CONTROLLER_DPAD_UP] = false;
							_keyMap[CONTROLLER_DPAD_DOWN] = true;
							_keyMap[CONTROLLER_DPAD_RIGHT] = true;
							break;
						case SDL_HAT_CENTERED:
						default:
							_controllers[_event.jhat.which].dPad = glm::vec2(0, 0);
							_keyMap[CONTROLLER_DPAD_LEFT] = false;
							_keyMap[CONTROLLER_DPAD_UP] = false;
							_keyMap[CONTROLLER_DPAD_DOWN] = false;
							_keyMap[CONTROLLER_DPAD_RIGHT] = false;
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				_keyMap[_event.button.button] = true;
				break;
			case SDL_MOUSEBUTTONUP:
				_keyMap[_event.button.button] = false;
				break;
			case SDL_MOUSEMOTION:
				_mouseCoords.x = _event.motion.x;
				_mouseCoords.y = _event.motion.y;
				break;
			case SDL_MOUSEWHEEL:
				deltaScroll += _event.wheel.y;
				break;
			case SDL_WINDOWEVENT:
				if (_event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					_windowResized = true;
				}
				break;
			case SDL_CONTROLLERDEVICEADDED:
				while (SDL_NumJoysticks() > _controllers.size()) {
					_controllers.emplace_back(SDL_GameControllerOpen(_event.cdevice.which));
				}
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				for (int i = 0; i < _controllers.size(); ++i) {
					if (_event.cdevice.which == SDL_JoystickInstanceID(_controllers[i].joystick)) {
						_controllers.erase(_controllers.begin() + i);
						break;
					}
				}
				break;
		}
	}
}

bool InputManager::isKeyDown(unsigned int keyID) {
	return _keyMap[keyID];
}

bool InputManager::isKeyPressed(unsigned int keyID) {
	return _keyMap[keyID] ? !_previousKeyMap[keyID] : false;
}
bool InputManager::onKeyUp(unsigned int keyID) {
	return !_keyMap[keyID] ? _previousKeyMap[keyID] : false;
}

bool InputManager::isKeyUp(unsigned int keyID) {
	return !_keyMap[keyID];
}


uint32_t InputManager::lastButtonPressed() {
	return _lastInput;
}
void InputManager::clearLastButtonPressed() {
	_lastInput = SDLK_ESCAPE;
};

bool InputManager::windowResized() {
	if (_windowResized) {
		_windowResized = false;
		return true;
	}
	else return false;
}

bool InputManager::quitGameCalled() {
	return _quitGameCalled;
}

glm::vec2 InputManager::getMouseCoords() {
	return _mouseCoords;
}

glm::vec2 InputManager::getControllerAxis(Axis type, int controllerID) {
	if (controllerID >= _controllers.size()) {
		Jauntlet::error("WARNING: Tried to get Axis on non-existent Controller");
		return glm::vec2(0);
	}
	switch (type) {
		case Axis::LeftStick:
			return _controllers[controllerID].leftStick;
		case Axis::RightStick:
			return _controllers[controllerID].rightStick;
		case Axis::Triggers:
			return _controllers[controllerID].triggers;
		case Axis::dPad:
			return _controllers[controllerID].dPad;
		default:
		Jauntlet::error("WARNING: invalid parameter passed into getControllerAxis!");
		return glm::vec2(0);
	}
}
glm::vec2 InputManager::getControllerAxis(Axis type) {
	glm::vec2 output = glm::vec2(0);
	
	switch (type) {
	case Axis::LeftStick:
		for (int i = 0; i < _controllers.size(); ++i) {
			output += _controllers[i].leftStick;
		}
		break;
	case Axis::RightStick:
		for (int i = 0; i < _controllers.size(); ++i) {
			output += _controllers[i].rightStick;
		}
		break;
	case Axis::Triggers:
		for (int i = 0; i < _controllers.size(); ++i) {
			output += _controllers[i].triggers;
		}
		break;
	case Axis::dPad:
		for (int i = 0; i < _controllers.size(); ++i) {
			output += _controllers[i].dPad;
		}
		break;
	default:
		Jauntlet::error("WARNING: invalid parameter passed into getControllerAxis!");
		return glm::vec2(0);
	}
	
	return output /= _controllers.size();
}

Controller* InputManager::getController(int controllerID) {
	return &_controllers[controllerID];
}

unsigned int InputManager::getControllerCount() {
	return _controllers.size();
}