#pragma once

class ScriptFiber_t {
public:
	void(*functionToCall)(void);
	static LPVOID _mainFiber;
	LPVOID mainFiber;
	ScriptFiber_t* nextFiber;
	LPVOID scriptFiber;
	unsigned long wakeTime;
	ScriptFiber_t() :
		functionToCall(0),
		mainFiber(0),
		nextFiber(0),
		scriptFiber(0),
		wakeTime(0)
	{
	}

	static void Init();

	void Tick();
};

extern array<ScriptFiber_t, 10> g_scriptFibers;

void WAIT(unsigned long waitTime);