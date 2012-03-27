#ifndef TESTSTATE_H_
#define TESTSTATE_H_
#include <Scene/State.hpp>
class TestState: public dt::State{	
public:
	void OnInitialize(); 
	void UpdateStateFrame(double simulation_frame_time);
};
#endif