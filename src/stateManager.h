/****
	�������Ĺ�����ע��һ������Ҫ��������
	1. include �������ͷ�ļ�
	2. ��StateName�ж���ö�ٳ�������
	3. ��create������дcase������ɡ�
****/


#ifndef _STATEMANAGER_H_
#define _STATEMANAGER_H_
#include <Scene/State.hpp>
#include "testState.h"
#include <string>

using namespace std; 

enum StateName {
	TEST
};
class StateManager {
public:
	dt::State * create(StateName stateName);  
};
extern StateManager stateManager;
#endif