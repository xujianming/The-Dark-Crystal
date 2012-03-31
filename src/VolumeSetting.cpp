#include "VolumeSetting.h"

VolumeSetting::VolumeSetting()
{
	mVolume = 0;
}

void VolumeSetting::setVolume(unsigned volume)
{
	mVolume = volume;
}

unsigned VolumeSetting::getVolume() const
{
	return mVolume;
}