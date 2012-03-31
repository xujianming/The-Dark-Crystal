#ifndef SOUNDEFFECT_SETTING_H
#define SOUNDEFFECT_SETTING_H

using namespace std;

class SoundEffectSetting
{
  public:
     SoundEffectSetting();
	 
	 void setSoundEffect(unsigned sound_effect);
	 
	 unsigned getSoundEffect() const;
	 
  private:
     unsigned mSoundEffect;
};

#endif