#include "SoundEffectSetting.h"

SoundEffectSetting::SoundEffectSetting()
{
	mSoundEffect = 0;
}

unsigned SoundEffectSetting::getSoundEffect() const
{
	return mSoundEffect;
}

void SoundEffectSetting::setSoundEffect(unsigned sound_effect)
{
	mSoundEffect = sound_effect;
}