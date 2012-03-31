#include "ResolutionRatioSetting.h"


ResolutionRatioSetting::ResolutionRatioSetting()
{
	mReslutionRatioMap[HORIZONTAL] = pair<QString, unsigned> ("Horizontal",1024);
	mReslutionRatioMap[VERTICAL] = pair<QString, unsigned> ("Vertical",768);
}

void ResolutionRatioSetting::setResolutionRatio(Direction direction, unsigned resolution_ratio)
{
	mReslutionRatioMap[direction].second = resolution_ratio;
}

unsigned ResolutionRatioSetting::getResolutionRatio(Direction direction) const
{
	return mReslutionRatioMap.at(direction).second;
}

QString ResolutionRatioSetting::getName(Direction direction) const
{
	return mReslutionRatioMap.at(direction).first;
}