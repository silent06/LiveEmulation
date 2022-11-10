#pragma once

class Thread {
private:
	const char* m_name;
	HANDLE m_thread;
	bool m_isRunning;
	bool m_security;
	void* m_function;
public:
	Thread() : m_name(""), m_thread(0), m_isRunning(true) {}
	Thread(const char* name) : m_name(name), m_thread(0), m_isRunning(true) {}

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

	const char* GetName() {
		return m_name;
	}
};

struct ThreadParams {
	Thread* m_thread;
	void(*m_func)();
};

DWORD WINAPI ThreadHandler(LPVOID lpFiberParameter);

class ThreadPool {
private:
	vector<Thread*> m_Threads;
public:
	__declspec(noinline) Thread* RegisterThread(const char* name, void(*func)(), bool security = false) {
		Thread* c_thread = new Thread(name);
		c_thread->SetSecure(security);

		ThreadParams* params = new ThreadParams();
		params->m_thread = c_thread;
		params->m_func = func;

		c_thread->SetHandle(CreateThread(0, 0, ThreadHandler, params, 0, 0));
		m_Threads.push_back(c_thread);

#ifdef DEVELOPER_BUILD
		LOG(false, "[Threading] Registered thread %s @ 0x%llx\n", name, (DWORD)params->m_func);
#endif
		return c_thread;
	}

	void RestartThreads() {
#ifdef DEVELOPER_BUILD
		LOG(false, "[Threading] Restarting threads!");
#endif

		for (int i = 0; i < GetThreads().size(); i++) {
			ThreadParams* params = new ThreadParams();
			params->m_thread = GetThreads().at(i);
			params->m_func = (void(*)())GetThreads().at(i)->GetFunction();

			GetThreads().at(i)->SetHandle(CreateThread(0, 0, ThreadHandler, params, 0, 0));

#ifdef DEVELOPER_BUILD
			LOG(false, "[Threading] Restarted thread %s @ 0x%llx\n", GetThreads().at(i)->GetName(), (DWORD)params->m_func);
#endif
		}
	}

	vector<Thread*> GetThreads() { return m_Threads; };
};

ThreadPool* GetThreadPool();