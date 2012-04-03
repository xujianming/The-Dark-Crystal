#ifndef MOUSE_SETTING_H
#define MOUSE_SETTING_H

#include <Input/InputManager.hpp>

#include <map>

using namespace dt;
using namespace std;

class MouseSetting
{
public:
	enum Function
	{
		ATTACK,
		DEFENSE
	};

	MouseSetting();

	void setMouse(Function function, InputManager::InputCode mouse);

	InputManager::InputCode getMouse(Function function) const;

	QString getName(Function function) const;

	Function begin() const;

	Function end() const;

private:
	map<Function, pair<QString, InputManager::InputCode>> mMouseMap;
};

#endif

