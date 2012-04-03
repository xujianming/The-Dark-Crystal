#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

#include "KeySettings.h"
#include "Definitions.h"
#include "MusicSetting.h"
#include "YInvertedSetting.h"
#include "SoundEffectSetting.h"
#include "SensitivitySetting.h"
#include "MouseSetting.h"
#include "ScreenSetting.h"

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

	KeySettings getKeySettings() const;

	void setKeySettings(KeySettings key_settings);

	MusicSetting getMusicSetting() const;

	void setMusicSetting(MusicSetting Music_setting);

	YInvertedSetting getYInvertedSetting() const;

	void setYInvertedSetting(YInvertedSetting set_flip_setting);

	SoundEffectSetting getSoundEffectSetting() const;

	void setSoundEffectSetting(SoundEffectSetting sound_effect_setting);

	SensitivitySetting getSensitivitySetting() const;

	void setSensitivitySetting(SensitivitySetting sensitivity_setting);

	MouseSetting getMouseSetting() const;

	void setMouseSetting(MouseSetting mouse_setting);

	ScreenSetting getScreenSetting() const;

	void setScreenSetting(ScreenSetting screen_setting);

private:
	void __loadKeySettings(const QDomElement& element);

	QDomElement __saveKeySettings(QDomDocument& doc) const;

	void _loadMusicSetting(const QDomElement& element);

	QDomElement _saveMusicSetting(QDomDocument& doc) const;

	void _loadYInvertedSetting(const QDomElement& element);

	QDomElement _saveYInvertedSetting(QDomDocument& doc) const;

	void _loadSoundEffectSetting(const QDomElement& element);

	QDomElement _saveSoundEffectSetting(QDomDocument& doc) const;

	void _loadSensitivitySetting(const QDomElement& element);

	QDomElement _saveSensitivitySetting(QDomDocument& doc) const;

	void _loadMouseSetting(const QDomElement& element);

	QDomElement _saveMouseSetting(QDomDocument& doc) const;

	void _loadSreenSetting(const QDomElement& element) ;

	QDomElement _saveScreenSetting(QDomDocument& doc) const;

	ConfigurationManager();

	static std::shared_ptr<ConfigurationManager> mInstance;

	KeySettings mKeySettings;

	MusicSetting mMusicSetting;

	YInvertedSetting mYInvertedSetting;

	SoundEffectSetting mSoundEffectSetting;

	SensitivitySetting mSensitivitySetting;

	MouseSetting mMouseSetting;

	ScreenSetting mScreenSetting;

};

#endif