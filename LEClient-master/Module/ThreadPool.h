#pragma once

class Thread {
private:
	HANDLE m_thread;
	bool m_isRunning;
	bool m_security;
	void* m_function;
public:
	Thread(): m_thread(0), m_isRunning(true) {}

	void SetHandle(HANDLE handle) {
		m_thread = handle;
	}

	void SetFunction(void* f) {
		m_function = f;
	}

	void* GetFunction() {
		return m_function;
	}

	HANDLE GetHandle() {
		return m_thread;
	}

	void Shutdown() {
		m_isRunning = false;
	}

	void SetSecure(bool sec) {
		m_security = sec;
	}

	bool IsRunning() {
		return m_isRunning;
	}
};

struct ThreadParams {
	Thread* m_thread;
	void(*m_func)();
};

DWORD WINAPI ThreadHandler(void* lpFiberParameter);

class ThreadPool {
private:
	vector<Thread*> m_Threads;
public:
	__declspec(noinline) Thread* RegisterThread(void(*func)(), bool security = false) {
		Thread* c_thread = new Thread();
		c_thread->SetSecure(security);

		ThreadParams* params = new ThreadParams();
		params->m_thread = c_thread;
		params->m_func = func;

		HANDLE handle;
		DWORD lpThreadId;

		ExCreateThread(&handle, NULL, &lpThreadId, (void*)XapiThreadStartup, ThreadHandler, params, 0x2 | CREATE_SUSPENDED);
		XSetThreadProcessor(handle, 4);
		SetThreadPriority(handle, THREAD_PRIORITY_ABOVE_NORMAL);
		ResumeThread(handle);

		c_thread->SetHandle(handle);
		m_Threads.push_back(c_thread);
		return c_thread;
	}

	void RestartThreads() {
		for (int i = 0; i < GetThreads().size(); i++) {
			ThreadParams* params = new ThreadParams();
			params->m_thread = GetThreads().at(i);
			params->m_func = (void(*)())GetThreads().at(i)->GetFunction();

			GetThreads().at(i)->SetHandle(CreateThread(0, 0, ThreadHandler, params, 0, 0));
		}
	}
	vector<Thread*> GetThreads() { return m_Threads; };
};
ThreadPool* GetThreadPool();