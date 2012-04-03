#include "ScreenSetting.h"

ScreenSetting::ScreenSetting()
{
	mResolution.width = 1280;
	mResolution.height = 800;
}

unsigned ScreenSetting::getResolutionWidth() const
{
	return mResolution.width;
}

unsigned ScreenSetting::getResolutionHeight() const
{
	return mResolution.height;
}

void ScreenSetting::setResolutionWidth(unsigned width)
{
	mResolution.width = width;
}

void ScreenSetting::setResolutionHeight(unsigned height)
{
	mResolution.height = height;
}