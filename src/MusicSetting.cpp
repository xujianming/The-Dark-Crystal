#include "MusicSetting.h"

MusicSetting::MusicSetting()
{
	mMusic = 11;
}

void MusicSetting::setMusic(unsigned Music)
{
	mMusic = Music;
}

unsigned MusicSetting::getMusic() const
{
	return mMusic;
}