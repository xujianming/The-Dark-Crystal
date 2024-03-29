#include "stateManager.h"


void
NewStateManager::push(StateName stateName) {	
		dt::State * res = NULL; 
		switch (stateName) {
		case TEST:						
			res = new TestState(); 
			break; 
		}
		if (res != NULL)
		dt::Root::getInstance().getStateManager()->setNewState(res); 
}
NewStateManager & 
NewStateManager::getInstance() {
	static NewStateManager instance; 
	return instance;
}


void
NewStateManager::pop(uint16_t count) {
	dt::Root::getInstance().getStateManager()->pop();
}
