#include "stdafx.h"

ThreadPool g_pool;
DWORD WINAPI ThreadHandler(void* lpFiberParameter) {
	ThreadParams* arguments = (ThreadParams*)lpFiberParameter;
	while (arguments->m_thread->IsRunning()) {
		arguments->m_func();
	}
	return 1;
}

ThreadPool* GetThreadPool() {
	return &g_pool;
}