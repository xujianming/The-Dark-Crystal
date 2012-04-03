#include "YInvertedSetting.h"

YInvertedSetting::YInvertedSetting()
{
	mYInverted = false;
}

bool YInvertedSetting::getYInverted() const
{
	return mYInverted;
}

void YInvertedSetting::setYInverted(bool y_inverted)
{
	mYInverted = y_inverted;
}