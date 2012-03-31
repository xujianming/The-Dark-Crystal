#ifndef FILP_Y_H
#define FILP_Y_H

using namespace std;

class FlipYSetting
{
   public:
      FlipYSetting();
	  
	  bool getIsFlipY() const;
	  
	  void setIsFlipY(bool is_flip_y);
	  
   private:
      bool mIsFlipY;   
};

#endif;