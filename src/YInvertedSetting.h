#ifndef Y_INVERTED_SETTING_H
#define Y_INVERTED_SETTING_H

using namespace std;

class YInvertedSetting
{
public:
	YInvertedSetting();

	bool getYInverted() const;

	void setYInverted(bool y_inverted);

private:
	bool mYInverted;   
};

#endif;