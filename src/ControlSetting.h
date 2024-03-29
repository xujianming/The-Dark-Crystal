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
        ATTACK,
        DEFENSE,
        ACTIVATE
    };

    void setKey(KeyFunction KeyFunction, InputManager::InputCode key);

    InputManager::InputCode getKey(KeyFunction KeyFunction) const;

    QString getKeyName(KeyFunction KeyFunction) const;

    KeyFunction keyBegin() const;

    KeyFunction keyEnd() const;

    //YInvertedSetting
    bool getYInverted() const;

    void setYInverted(bool y_inverted);

    //SensitivitySetting
    unsigned getSensitivity() const;

    void setSentivity(unsigned sensitivity);


private:
    map<KeyFunction, pair<QString, InputManager::InputCode>> mKeyMap;
    bool mYInverted;
    unsigned mSentivity;
};

#endif