#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H

#include "KeySettings.h"
#include "Definitions.h"
#include "VolumeSetting.h"
#include "ResolutionRatioSetting.h"
#include "FlipYSetting.h"
#include "SoundEffectSetting.h"
#include "SensitivitySetting.h"

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

	VolumeSetting getVolumeSetting() const;

	void setVolumeSetting(VolumeSetting volume_setting);

	ResolutionRatioSetting getResolutionRatioSetting() const;

	void setResolutionRatioSetting(ResolutionRatioSetting resolution_ratio_setting);

	FlipYSetting getFlipYSetting() const;

	void setFlipYSetting(FlipYSetting set_flip_setting);

	SoundEffectSetting getSoundEffectSetting() const;

	void setSoundEffectSetting(SoundEffectSetting sound_effect_setting);

	SensitivitySetting getSensitivitySetting() const;

	void setSensitivitySetting(SensitivitySetting sensitivity_setting);

private:
    void __loadKeySettings(const QDomElement& element);

    QDomElement __saveKeySettings(QDomDocument& doc) const;

	void _loadVolumeSetting(const QDomElement& element);

	QDomElement _saveVolumeSetting(QDomDocument& doc) const;

	void _loadResolutionRatioSetting(const QDomElement& element);

	QDomElement _saveResolutionRatioSetting(QDomDocument& doc) const;

	void _loadFlipYSetting(const QDomElement& element);

	QDomElement _saveFlipYSetting(QDomDocument& doc) const;

	void _loadSoundEffectSetting(const QDomElement& element);

	QDomElement _saveSoundEffectSetting(QDomDocument& doc) const;

	void _loadSensitivitySetting(const QDomElement& element);

	QDomElement _saveSensitivitySetting(QDomDocument& doc) const;

    ConfigurationManager();

    static std::shared_ptr<ConfigurationManager> mInstance;

    KeySettings mKeySettings;

	VolumeSetting mVolumeSetting;

	ResolutionRatioSetting mResolutionRatioSetting;

	FlipYSetting mFlipYSetting;

	SoundEffectSetting mSoundEffectSetting;

	SensitivitySetting mSensitivitySetting;
	
};

#endif