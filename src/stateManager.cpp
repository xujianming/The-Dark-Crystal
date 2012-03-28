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
		dt::Root::GetInstance().GetStateManager()->SetNewState(res); 
}
NewStateManager & 
NewStateManager::getInstance() {
	static NewStateManager instance; 
	return instance;
}
bool
NewStateManager::shiftStates() {
	return dt::Root::GetInstance().GetStateManager()->ShiftStates();
}

void
NewStateManager::pop(uint16_t count) {
	dt::Root::GetInstance().GetStateManager()->Pop();
}
dt::State *
NewStateManager::getCurrentState() {
	return dt::Root::GetInstance().GetStateManager()->GetCurrentState();
}	