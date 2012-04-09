# ifndef _CONTROL_SETTING_H
# define _CONTROL_SETTING_H

#include <Input/InputManager.hpp>

#include <map>

using namespace dt;
using namespace std;

class ControlSetting
{
public:
	ControlSetting();

	//KeySetting
	enum KeyFunction
	{
		FORWARD,
		BACKWARD,
		LEFTWARD,
		RIGHTWARD,
		JUMP,
		SPRINT,
		SWITCH,
		ARM1,
		ARM2,
		ARM3,
		ARM4,
		ACTIVATE
	};

	void setKey(KeyFunction KeyFunction, InputManager::InputCode key);

	InputManager::InputCode getKey(KeyFunction KeyFunction) const;

	QString getKeyName(KeyFunction KeyFunction) const;

	KeyFunction keyBegin() const;

	KeyFunction keyEnd() const;
	
	//MouseSetting
	enum MouseFunction
	{
		ATTACK,
		DEFENSE
	};

	void setMouse(MouseFunction MouseFunction, InputManager::InputCode mouse);

	InputManager::InputCode getMouse(MouseFunction MouseFunction) const;

	QString getMouseName(MouseFunction MouseFunction) const;

	MouseFunction mouseBegin() const;

	MouseFunction mouseEnd() const;

	//YInvertedSetting
	bool getYInverted() const;

	void setYInverted(bool y_inverted);

	//SensitivitySetting
	unsigned getSensitivity() const;

	void setSentivity(unsigned sensitivity);


private:
	map<KeyFunction, pair<QString, InputManager::InputCode>> mKeyMap;
	map<MouseFunction, pair<QString, InputManager::InputCode>> mMouseMap;
	bool mYInverted;
	unsigned mSentivity;
};

#endif