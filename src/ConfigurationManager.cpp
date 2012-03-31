#include "ConfigurationManager.h"

#include <Utils/Logger.hpp>
#include <Utils/Utils.hpp>

std::shared_ptr<ConfigurationManager> ConfigurationManager::mInstance = 
    std::shared_ptr<ConfigurationManager>(new ConfigurationManager());

ConfigurationManager::ConfigurationManager()
{
}

ConfigurationManager* ConfigurationManager::getInstance()
{
    return mInstance.get();
}

bool ConfigurationManager::loadConfig()
{
    QFile config_file(CONFIG_FILE);
    QDomDocument doc;

    // If the file doesn't exist...
    if(!config_file.open(QIODevice::ReadOnly))
    {
        Logger::get().info("The configuration file doesn't exist. Trying to create a new one.");
        
        if(!saveConfig())
        {
            Logger::get().error("Failed to open the configuration file.");

            return false;
        }
        else
        {
            Logger::get().info("Created a new configuration file.");

            return true;
        }
    }

    // Else...
    if(doc.setContent(&config_file))
    {
        QDomElement root = doc.documentElement();

        // Only loads the key settings for now.
        // TODO: Add other loading stuff here.
        for(QDomElement config_node = root.firstChildElement() ; !config_node.isNull() ; config_node = config_node.nextSiblingElement())
        {
            QString tag_name = config_node.tagName();

            if(tag_name == KEY_SETTINGS)
            {
                __loadKeySettings(config_node);
            }
			else if (tag_name == VOLUME_SETTING)
			{
				_loadVolumeSetting(config_node);
			}
			else if (tag_name == RESOLUTION_RATIO_SETTING)
			{
				_loadResolutionRatioSetting(config_node);
			}
			else if (tag_name == FLIP_Y_SETTING)
			{
				_loadFlipYSetting(config_node);
			}
			else if(tag_name == SOUND_EFFECT_SETTING)
			{
				_loadSoundEffectSetting(config_node);
			}
			else if (tag_name == SOUND_EFFECT_SETTING)
			{
				_loadSensitivitySetting(config_node);
			}
        }
    }
    else
    {
        Logger::get().error("Failed to read from the configuration file.");

        return false;
    }

    return true;
}

bool ConfigurationManager::saveConfig() const
{
	QFile config_file(CONFIG_FILE);
	QDomDocument doc;

	if(!config_file.open(QIODevice::WriteOnly))
	{
		Logger::get().error("Cannot create the configuration file.");

		return false;
	}

	// The root.
	auto root = doc.createElement(CONFIG_ROOT);
	doc.appendChild(root);

	// Only saves the key settings for now.
	// TODO: Add other saving stuff here.
	root.appendChild(__saveKeySettings(doc));
	root.appendChild(_saveVolumeSetting(doc));
	root.appendChild(_saveResolutionRatioSetting(doc));
	root.appendChild(_saveFlipYSetting(doc));
	root.appendChild(_saveSoundEffectSetting(doc));
	root.appendChild(_saveSensitivitySetting(doc));

	// Save it to the file.
	QTextStream out(&config_file);
	out << doc.toString();

	// Close the file.
	config_file.close();

	return true;
}

KeySettings ConfigurationManager::getKeySettings() const
{
    return mKeySettings;
}

void ConfigurationManager::setKeySettings(KeySettings key_settings)
{
    mKeySettings = key_settings;
}

VolumeSetting ConfigurationManager::getVolumeSetting() const
{
	return mVolumeSetting;
}

void ConfigurationManager::setVolumeSetting(VolumeSetting volume_setting)
{
	mVolumeSetting = volume_setting;
}

ResolutionRatioSetting ConfigurationManager::getResolutionRatioSetting() const
{
	return mResolutionRatioSetting;
}

void ConfigurationManager::setResolutionRatioSetting(ResolutionRatioSetting resolution_ratio_setting)
{
	mResolutionRatioSetting = resolution_ratio_setting;
}

FlipYSetting ConfigurationManager::getFlipYSetting() const
{
	return mFlipYSetting;
}

void ConfigurationManager::setFlipYSetting(FlipYSetting set_flip_y_setting)
{
	mFlipYSetting = set_flip_y_setting;
}

SoundEffectSetting ConfigurationManager::getSoundEffectSetting() const
{
	return mSoundEffectSetting;
}

void ConfigurationManager::setSoundEffectSetting(SoundEffectSetting sound_effect_setting)
{
	mSoundEffectSetting = sound_effect_setting;
}

SensitivitySetting ConfigurationManager::getSensitivitySetting() const
{
	return mSensitivitySetting;
}

void ConfigurationManager::setSensitivitySetting(SensitivitySetting sensitivity_setting)
{
	mSensitivitySetting = sensitivity_setting;
}

void ConfigurationManager::__loadKeySettings(const QDomElement& element)
{
    for(auto key_node = element.firstChildElement() ; !key_node.isNull() ; key_node = key_node.nextSiblingElement())
    {
        KeySettings::Function function_code = (KeySettings::Function)key_node.attribute(KEY_FUNCTION).toUInt();
        InputManager::InputCode input_code = (InputManager::InputCode)key_node.attribute(KEY_CODE).toUInt();

        mKeySettings.setKey(function_code, input_code);
    }
}

QDomElement ConfigurationManager::__saveKeySettings(QDomDocument& doc) const
{
    auto key_settings = doc.createElement(KEY_SETTINGS);

    for(unsigned function = (unsigned)mKeySettings.begin() ; function <= (unsigned)mKeySettings.end() ; ++function)
    {
        auto element = doc.createElement(mKeySettings.getName((KeySettings::Function)function));
        
        element.setAttribute(KEY_FUNCTION, function);
        element.setAttribute(KEY_CODE, (unsigned)mKeySettings.getKey((KeySettings::Function)function));
        
        key_settings.appendChild(element);
    }

    return key_settings;
}

void ConfigurationManager::_loadVolumeSetting(const QDomElement& element)
{
	auto volume_node = element.firstChildElement();
	unsigned volume_value = volume_node.attribute(VOLUME_VALUE).toUInt();

	mVolumeSetting.setVolume(volume_value);
}

QDomElement ConfigurationManager::_saveVolumeSetting(QDomDocument& doc) const
{
	auto volume_setting = doc.createElement(VOLUME_SETTING);

	auto element = doc.createElement(VOLUME);
	element.setAttribute(VOLUME_VALUE,mVolumeSetting.getVolume());

	volume_setting.appendChild(element);

	return volume_setting;
}

void ConfigurationManager::_loadResolutionRatioSetting(const QDomElement& element)
{
	auto resolution_node_h = element.firstChildElement();
	ResolutionRatioSetting::Direction direction_h = (ResolutionRatioSetting::Direction)resolution_node_h.attribute(DIRECTION).toUInt();
	unsigned resolution_ratio_value_h =  resolution_node_h.attribute(RESOLUTION_RATIO_VALUE).toUInt();
	mResolutionRatioSetting.setResolutionRatio(direction_h,resolution_ratio_value_h);
	
	auto resolution_node_v = resolution_node_h.nextSiblingElement();
	ResolutionRatioSetting::Direction direction_v = (ResolutionRatioSetting::Direction)resolution_node_v.attribute(DIRECTION).toUInt();
	unsigned resolution_ratio_value_v =  resolution_node_v.attribute(RESOLUTION_RATIO_VALUE).toUInt();
	mResolutionRatioSetting.setResolutionRatio(direction_v,resolution_ratio_value_v);


}

QDomElement ConfigurationManager::_saveResolutionRatioSetting(QDomDocument& doc) const
{
	auto resolution_ratio_setting = doc.createElement(RESOLUTION_RATIO_SETTING);

	auto element_h = doc.createElement(mResolutionRatioSetting.getName((ResolutionRatioSetting::Direction)0));
	element_h.setAttribute(DIRECTION,0);
	element_h.setAttribute(RESOLUTION_RATIO_VALUE,mResolutionRatioSetting.getResolutionRatio((ResolutionRatioSetting::Direction)0));
	auto element_v = doc.createElement(mResolutionRatioSetting.getName((ResolutionRatioSetting::Direction)1));
	element_v.setAttribute(DIRECTION,1);
	element_v.setAttribute(RESOLUTION_RATIO_VALUE,mResolutionRatioSetting.getResolutionRatio((ResolutionRatioSetting::Direction)1));

	resolution_ratio_setting.appendChild(element_h);
	resolution_ratio_setting.appendChild(element_v);

	return resolution_ratio_setting;

}

void ConfigurationManager::_loadFlipYSetting(const QDomElement& element)
{
	auto flip_y_node = element.firstChildElement();
	bool flip_y_value = (flip_y_node.attribute(VALUE) == "true");

	mVolumeSetting.setVolume(flip_y_value);
}

QDomElement ConfigurationManager::_saveFlipYSetting(QDomDocument &doc) const
{
	auto flip_y_setting = doc.createElement(FLIP_Y_SETTING);

	auto element = doc.createElement(IS_FLIP_Y);
	element.setAttribute(VALUE,mFlipYSetting.getIsFlipY());

	flip_y_setting.appendChild(element);

	return flip_y_setting;
}

void ConfigurationManager::_loadSoundEffectSetting(const QDomElement& element)
{
	auto sound_effect_node = element.firstChildElement();
	unsigned sound_effect_value = sound_effect_node.attribute(SOUND_EFFECT_VALUE).toUInt();

	mSoundEffectSetting.setSoundEffect(sound_effect_value);
}

QDomElement ConfigurationManager::_saveSoundEffectSetting(QDomDocument &doc) const
{
	auto sound_effect_setting = doc.createElement(SOUND_EFFECT_SETTING);

	auto element = doc.createElement(SOUND_EFFECT);
	element.setAttribute(SOUND_EFFECT_VALUE,mSoundEffectSetting.getSoundEffect());

	sound_effect_setting.appendChild(element);

	return sound_effect_setting;
}

void ConfigurationManager::_loadSensitivitySetting(const QDomElement& element)
{
	auto sensitivity_node = element.firstChildElement();
	unsigned sensitivity_value = sensitivity_node.attribute(SENSITIVITY_VALUE).toUInt();

	mSensitivitySetting.setSentivity(sensitivity_value);
}

QDomElement ConfigurationManager::_saveSensitivitySetting(QDomDocument& doc) const
{
	auto sensitivity_effect_setting = doc.createElement(SENSITIVITY_SETTING);
	auto element = doc.createElement(SENSITIVITY);
	element.setAttribute(SENSITIVITY_VALUE,mSensitivitySetting.getSensitivity());

	sensitivity_effect_setting.appendChild(element);

	return sensitivity_effect_setting;
}