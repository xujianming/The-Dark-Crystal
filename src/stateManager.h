/****
	�������Ĺ�����ע��һ������Ҫ��������
	1. include �������ͷ�ļ�
	2. ��StateName�ж���ö�ٳ�������
	3. ��push������дcase������ɡ�
****/


#ifndef _STATEMANAGER_H_
#define _STATEMANAGER_H_
#include <Scene/State.hpp>
#include "testState.h"
#include <string>
#include <Scene/StateManager.hpp>
#include <Core/Root.hpp>

using namespace std; 

enum StateName {
	TEST
};
class NewStateManager {
public:
	void push(StateName stateName);  
	void pop(uint16_t count = 1);
	dt::State * getCurrentState();
	bool shiftStates();
	static NewStateManager & getInstance(); 
//for singleton
private:
	NewStateManager();
	NewStateManager & operator = (const NewStateManager &);
	NewStateManager(const NewStateManager &);
};

#endif