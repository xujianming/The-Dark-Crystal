#ifndef MUSIC_SETTING_H
#define MUSIC_SETTING_H

using namespace std;

class MusicSetting
{
public:
	MusicSetting();

	void setMusic(unsigned Music);

	unsigned getMusic() const;

private:
	unsigned mMusic;
};

#endif