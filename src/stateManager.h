/****
	管理场景的工厂，注册一个场景要做三步：
	1. include 场景类的头文件
	2. 在StateName中定义枚举常量名字
	3. 在push函数中写case语句生成。
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
	static NewStateManager & getInstance(); 
//for singleton
private:
	NewStateManager();
	NewStateManager & operator = (const NewStateManager &);
	NewStateManager(const NewStateManager &);
};

#endif