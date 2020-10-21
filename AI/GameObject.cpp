#include "GameObject.h"

void AIComponent::DetermineStateChange() {

	if (m_ManagedObject->healthCmp->m_Health == 0) {
		ChangeState(fsm::States::STATE_DIE);
	}

	if (m_ManagedObject->healthCmp->m_Health <= 5) {
		ChangeState(fsm::States::STATE_FLEE);
	}

	if (m_ManagedObject->healthCmp->m_Health > 5) {
		ChangeState(fsm::States::STATE_SEARCH);
	}
}


void AIComponent::Update(){

	DetermineStateChange();

	if (!ExecuteStateLogic()) {
		std::cout << "COULDNT EXECUTE AI STATELOGIC for " << m_ManagedObject->m_ID << std::endl;
	}
}