#include "KeySettings.h"

KeySettings::KeySettings()
{
    mKeyMap[FORWARD] = pair<QString, InputManager::InputCode>("Forward", InputManager::KC_W);
    mKeyMap[BACKWARD] = pair<QString, InputManager::InputCode>("Backward", InputManager::KC_S);
    mKeyMap[LEFTWARD] = pair<QString, InputManager::InputCode>("Leftward", InputManager::KC_A);
    mKeyMap[RIGHTWARD] = pair<QString, InputManager::InputCode>("Rightward", InputManager::KC_D);
    mKeyMap[JUMP] = pair<QString, InputManager::InputCode>("Jump", InputManager::KC_SPACE);
    mKeyMap[ACTIVATE] = pair<QString, InputManager::InputCode>("Activate", InputManager::KC_E);
}

void KeySettings::SetKey(Function function, InputManager::InputCode key)
{
    mKeyMap[function].second = key;
}

InputManager::InputCode KeySettings::GetKey(Function function) const
{
    return mKeyMap.at(function).second;
}

QString KeySettings::GetName(Function function) const
{
    return mKeyMap.at(function).first;
}

KeySettings::Function KeySettings::Begin() const
{
    return FORWARD;
}

KeySettings::Function KeySettings::End() const
{
    return ACTIVATE;
}