#include "MouseSetting.h"

MouseSetting::MouseSetting()
{
	mMouseMap[ATTACK] = pair<QString, InputManager::InputCode>("Attack", InputManager::MC_LEFT);
	mMouseMap[DEFENSE] = pair<QString, InputManager::InputCode>("Defence", InputManager::MC_RIGHT);
}

void MouseSetting::setMouse(Function function, InputManager::InputCode mouse)
{
	mMouseMap[function].second = mouse;
}

InputManager::InputCode MouseSetting::getMouse(Function function) const
{
	return mMouseMap.at(function).second;
}

QString MouseSetting::getName(Function function) const
{
	return mMouseMap.at(function).first;
}

MouseSetting::Function MouseSetting::begin() const
{
	return ATTACK;
}

MouseSetting::Function MouseSetting::end() const
{
	return DEFENSE;
}