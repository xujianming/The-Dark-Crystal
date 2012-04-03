#ifndef SENSITIVITY_SETTING_H
#define SENSITIVITY_SETTING_H

using namespace std;

class SensitivitySetting
{
public:
	SensitivitySetting();

	unsigned getSensitivity() const;

	void setSentivity(unsigned sensitivity);

private:
	unsigned mSentivity;

};

#endif 