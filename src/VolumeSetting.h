#ifndef VOLUME_SETTING_H
#define VOLUME_SETTING_H

using namespace std;

class VolumeSetting
{
  public:
     VolumeSetting();
	 
	 void setVolume(unsigned volume);
	 
	 unsigned getVolume() const;
	 
  private:
     unsigned mVolume;
};

#endif