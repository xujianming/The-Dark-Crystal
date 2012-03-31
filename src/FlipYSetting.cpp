#include "FlipYSetting.h"

FlipYSetting::FlipYSetting()
{
	mIsFlipY = false;
}

bool FlipYSetting::getIsFlipY() const
{
	return mIsFlipY;
}

void FlipYSetting::setIsFlipY(bool is_flip_y)
{
	mIsFlipY = is_flip_y;
}