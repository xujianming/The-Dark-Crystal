#ifndef RESOLUTION_RATIO_SETTING_H
#define RESOLUTION_RATIO_SETTING_H

#include <Input/InputManager.hpp>
#include<map>


using namespace std;

class ResolutionRatioSetting
{
   public:
      enum Direction
	  {
		HORIZONTAL,
	    VERTICAL
		
	  };
	  
	  ResolutionRatioSetting();
	  
	  void setResolutionRatio(Direction direction,unsigned resolution_ratio);
	  
	  unsigned getResolutionRatio(Direction direction) const;
	  
	  QString getName(Direction direction) const;
	  
   private:
      map<Direction, pair<QString, unsigned>> mReslutionRatioMap;   
	  
     
};

#endif