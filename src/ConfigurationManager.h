#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

#include "ScreenSetting.h"
#include "SoundSetting.h"
#include "ControlSetting.h"
#include "Definitions.h"

#include <boost/noncopyable.hpp>

#include <QtXml/QtXml>

#include <memory>
#include <map>

class ConfigurationManager : boost::noncopyable
{
public:
	static ConfigurationManager* getInstance();

	bool loadConfig();

	bool saveConfig() const;

	ScreenSetting getScreenSetting() const;

	void setScreenSetting(ScreenSetting screen_setting);

	SoundSetting getSoundSetting() const;

	void setSoundSetting(SoundSetting sound_setting); 

	ControlSetting getControlSetting() const;

	void setControlSetting(ControlSetting control_setting);

private:
	void _loadSreenSetting(const QDomElement& element);

	QDomElement _saveScreenSetting(QDomDocument& doc) const;

	void _loadSoundSetting(const QDomElement& element);

	QDomElement _saveSoundSetting(QDomDocument& doc) const;

	void _loadControlSetting(const QDomElement& element);

	QDomElement _saveControlSetting(QDomDocument& doc) const;

	ConfigurationManager();

	static std::shared_ptr<ConfigurationManager> mInstance;

	ScreenSetting mScreenSetting;

	SoundSetting mSoundSetting;

	ControlSetting mControlSetting;
};

#endif