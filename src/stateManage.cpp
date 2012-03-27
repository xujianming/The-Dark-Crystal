#include "stateManager.h"

dt::State *
	StateManager::create(StateName stateName) {				
		switch (stateName) {
		case TEST:			
			return (new TestState());			
		}
		return	NULL;
}
StateManager stateManager;