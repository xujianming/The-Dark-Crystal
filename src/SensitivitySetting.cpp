#include "SensitivitySetting.h"

SensitivitySetting::SensitivitySetting()
{
	mSentivity = 10;
}

unsigned SensitivitySetting::getSensitivity() const
{
	return mSentivity;
}

void SensitivitySetting::setSentivity(unsigned sensitivity)
{
	mSentivity = sensitivity;
}