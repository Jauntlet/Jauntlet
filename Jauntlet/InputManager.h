#pragma once
#include <unordered_map>

namespace Jauntlet {
class InputManager
{
public:
	InputManager();

	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);

	bool isKeyDown(unsigned int keyID);
private:
	std::unordered_map<unsigned int, bool> _keyMap;
};
}

