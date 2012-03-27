/****
	管理场景的工厂，注册一个场景要做三步：
	1. include 场景类的头文件
	2. 在StateName中定义枚举常量名字
	3. 在create函数中写case语句生成。
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