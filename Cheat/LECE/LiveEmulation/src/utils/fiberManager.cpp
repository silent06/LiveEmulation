#include "stdafx.h"

LPVOID ScriptFiber_t::_mainFiber = 0;
array<ScriptFiber_t, 10> g_scriptFibers;
bool _bScriptFibersInit = false;

void WAIT(unsigned long waitTime) {
	auto& fibersFirstIt = g_scriptFibers.begin();
	auto& fibersLastIt = g_scriptFibers.end();
	auto& sfit = std::find_if(fibersFirstIt, fibersLastIt, [](const ScriptFiber_t& a) -> bool { return a.scriptFiber == GetCurrentFiber(); });
	if (sfit != fibersLastIt) {
		if (sfit->wakeTime == 0) sfit->wakeTime = 5;
		sfit->wakeTime = Utils::timeGetTime() + waitTime;
		SwitchToFiber(sfit->nextFiber->mainFiber);
	}
}

VOID WINAPI ScriptFiberHandler(LPVOID lpFiberParameter) {
	int* a = (int*)lpFiberParameter;
	try {
		for (;;) {
			((void(*)(void)) a[1])();
			SwitchToFiber((LPVOID)a[2]);
		}
	}
	catch (std::exception& e) {
		printf("exception: %s\n", e.what());
		delete[] a;
	}
}

void ScriptFiber_t::Init() {
	if (!_bScriptFibersInit) {
		for (auto sf = g_scriptFibers.begin(); sf != g_scriptFibers.end(); ++sf) {
			auto& nextIt = sf + 1;
			if (nextIt != g_scriptFibers.end())
				sf->nextFiber = &*nextIt;
			else
				sf->nextFiber = &g_scriptFibers.front();

			if (!sf->mainFiber) {
				if (ScriptFiber_t::_mainFiber) {
					sf->mainFiber = ScriptFiber_t::_mainFiber;
				}
				else if (!ScriptFiber_t::_mainFiber && GetLastError() == ERROR_ALREADY_FIBER) {
					sf->mainFiber = GetCurrentFiber();
				}
				else {
					ScriptFiber_t::_mainFiber = ConvertThreadToFiber(nullptr);
					sf->mainFiber = ScriptFiber_t::_mainFiber;
				}
			}
		}
		_bScriptFibersInit = true;
	}
}

void ScriptFiber_t::Tick() {
	if (!this->mainFiber) {
		if (ScriptFiber_t::_mainFiber) {
			this->mainFiber = ScriptFiber_t::_mainFiber;
		}
		else if (!ScriptFiber_t::_mainFiber && GetLastError() == ERROR_ALREADY_FIBER) {
			this->mainFiber = GetCurrentFiber();
		}
		else {
			ScriptFiber_t::_mainFiber = ConvertThreadToFiber(nullptr);
			this->mainFiber = ScriptFiber_t::_mainFiber;
		}
		return;
	}

	if (!nextFiber) {
		this->nextFiber = this;
	}

	if (Utils::timeGetTime() > this->wakeTime) {
		if (this->scriptFiber != nullptr)
			SwitchToFiber(this->scriptFiber);
		else {
			int* args = new int[3];
			args[0] = 0;
			args[1] = (int)this->functionToCall;
			args[2] = (int)this->nextFiber->mainFiber;

			this->scriptFiber = CreateFiber(NULL, ScriptFiberHandler, args);
		}
	}
}