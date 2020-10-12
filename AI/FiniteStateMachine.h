#pragma once

#include"Platform.h"

namespace fsm {

	enum States {

		STATE_INVALID = -1,
		STATE_WAIT = 0,
		STATE_SEARCH = 1,
		STATE_ATTACK = 2,
		STATE_DEFEND = 3,
		STATE_FLEE = 4,
		STATE_DIE = 5
	};


	class IStateLogic {
	public:

		virtual void execute() = 0;
	};


	class State {
	public:
		State(fsm::States state) {

			m_CurrentState = state;
		}

		void NewState(fsm::States state) {

			m_CurrentState = state;
		}

		fsm::States GetState() {
			return m_CurrentState;
		}


	private:
		fsm::States m_CurrentState = fsm::States::STATE_INVALID;

	private:
	};
}