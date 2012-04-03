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
			else if (tag_name == MUSIC_SETTING)
			{
				_loadMouseSetting(config_node);
			}
			else if (tag_name == SCREEN_SETTING)
			{
				_loadSreenSetting(config_node);
			}
			else if (tag_name == Y_INVERTED_SETTING)
			{
				_loadYInvertedSetting(config_node);
			}
			else if(tag_name == SOUND_EFFECT_SETTING)
			{
				_loadSoundEffectSetting(config_node);
			}
			else if (tag_name == SENSITIVITY_SETTING)
			{
				_loadSensitivitySetting(config_node);
			}
			else if (tag_name == MOUSE_SETTING)
			{
				_loadMouseSetting(config_node);
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
	root.appendChild(_saveMusicSetting(doc));
	root.appendChild(_saveScreenSetting(doc));
	root.appendChild(_saveYInvertedSetting(doc));
	root.appendChild(_saveSoundEffectSetting(doc));
	root.appendChild(_saveSensitivitySetting(doc));
	root.appendChild(_saveMouseSetting(doc));

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

MusicSetting ConfigurationManager::getMusicSetting() const
{
	return mMusicSetting;
}

void ConfigurationManager::setMusicSetting(MusicSetting Music_setting)
{
	mMusicSetting = Music_setting;
}

YInvertedSetting ConfigurationManager::getYInvertedSetting() const
{
	return mYInvertedSetting;
}

void ConfigurationManager::setYInvertedSetting(YInvertedSetting set_y_inverted_setting)
{
	mYInvertedSetting = set_y_inverted_setting;
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

MouseSetting ConfigurationManager::getMouseSetting() const
{
	return mMouseSetting;
}

void ConfigurationManager::setMouseSetting(MouseSetting mouse_setting) 
{
	mMouseSetting = mouse_setting;
}

ScreenSetting ConfigurationManager::getScreenSetting() const
{
	return mScreenSetting;
}

void ConfigurationManager::setScreenSetting(ScreenSetting screen_setting)
{
	mScreenSetting = screen_setting;
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

void ConfigurationManager::_loadMusicSetting(const QDomElement& element)
{
	auto music_node = element.firstChildElement();
	unsigned music_value = music_node.attribute(MUSIC_VALUE).toUInt();

	mMusicSetting.setMusic(music_value);
}

QDomElement ConfigurationManager::_saveMusicSetting(QDomDocument& doc) const
{
	auto music_setting = doc.createElement(MUSIC_SETTING);

	auto element = doc.createElement(MUSIC);
	element.setAttribute(MUSIC_VALUE,mMusicSetting.getMusic());

	music_setting.appendChild(element);

	return music_setting;
}

void ConfigurationManager::_loadYInvertedSetting(const QDomElement& element)
{
	auto y_inverted_node = element.firstChildElement();
	bool y_inverted_value = y_inverted_node.attribute(VALUE).toUInt();

	mYInvertedSetting.setYInverted(y_inverted_value);
}

QDomElement ConfigurationManager::_saveYInvertedSetting(QDomDocument &doc) const
{
	auto y_inverted_setting = doc.createElement(Y_INVERTED_SETTING);

	auto element = doc.createElement(Y_INVERTED);
	element.setAttribute(VALUE,mYInvertedSetting.getYInverted());

	y_inverted_setting.appendChild(element);

	return y_inverted_setting;
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

void ConfigurationManager::_loadMouseSetting(const QDomElement& element)
{
	for(auto mouse_node = element.firstChildElement() ; !mouse_node.isNull() ; mouse_node = mouse_node.nextSiblingElement())
	{
		MouseSetting::Function function_code = (MouseSetting::Function)mouse_node.attribute(MOUSE_FUNCTION).toUInt();
		InputManager::InputCode input_code = (InputManager::InputCode)mouse_node.attribute(MOUSE_CODE).toUInt();

		mMouseSetting.setMouse(function_code, input_code);
	}
}

QDomElement ConfigurationManager::_saveMouseSetting(QDomDocument& doc) const
{
	auto mouse_setting = doc.createElement(MOUSE_SETTING);

	for(unsigned function = (unsigned)mMouseSetting.begin() ; function <= (unsigned)mMouseSetting.end() ; ++function)
	{
		auto element = doc.createElement(mMouseSetting.getName((MouseSetting::Function)function));

		element.setAttribute(MOUSE_FUNCTION, function);
		element.setAttribute(MOUSE_CODE, (unsigned)mMouseSetting.getMouse((MouseSetting::Function)function));

		mouse_setting.appendChild(element);
	}

	return mouse_setting;
}

void ConfigurationManager::_loadSreenSetting(const QDomElement& element)
{
	auto resolution_node = element.firstChildElement(RESOLUTION);
	unsigned width_value = resolution_node.attribute(WIDTH).toUInt();
	unsigned height_value = resolution_node.attribute(HEIGHT).toUInt();
	mScreenSetting.setResolutionWidth(width_value);
	mScreenSetting.setResolutionHeight(height_value);
}

QDomElement ConfigurationManager::_saveScreenSetting(QDomDocument& doc) const
{
	auto screen_setting = doc.createElement(SCREEN_SETTING);
	auto resolution = doc.createElement(RESOLUTION);
	resolution.setAttribute(WIDTH,mScreenSetting.getResolutionWidth());
	resolution.setAttribute(HEIGHT,mScreenSetting.getResolutionHeight());
	screen_setting.appendChild(resolution);
	return screen_setting;
}