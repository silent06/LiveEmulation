#pragma once

/*Made by ronin - some lit shit*/

#define STRINGIZE(x) #x
#define PROFILER_START(x) Test x(__FILE__, STRINGIZE(x));
#define PROFILER() Test x(__FILE__, __FUNCTION__);
#define PROFILER_END(x) x.end();

const double NANOTOMILLI = 1.0 / 1000000.0;

inline unsigned long long __rdtsc() {
	unsigned long long retVal;
	__asm {
		mfspr r3, 268
		mr retVal, r3
	}

	return retVal;
}

inline int GetCurrentMS() {
	SYSTEMTIME time;
	GetSystemTime(&time);
	return int((time.wSecond * 1000) + time.wMilliseconds);
}

class ProfileEntry {
private:
	bool m_started;
	const char* m_fileName;
	const char* m_functionName;
	__int64 m_cyclesBegin;
	__int64 m_cyclesTotal;
	__int64 m_cyclesLatest;

	int m_timeBegin;
	UINT64 m_timeTotal;

	double m_timeAverageList[600];
	int m_timeAverageIndex;
	double m_timeAverageSum;
	double m_timeLatest;
	UINT64 m_calls;
public:
	ProfileEntry() : m_started(false), m_fileName(""), m_functionName(""), m_cyclesBegin(0), m_cyclesLatest(0), m_cyclesTotal(0), m_calls(0), m_timeAverageIndex(0), m_timeAverageSum(0), m_timeLatest(0) {}
	ProfileEntry(const char* fileName, const char* functionName) : m_started(false), m_fileName(fileName), m_functionName(functionName), m_cyclesBegin(0), m_cyclesLatest(0), m_cyclesTotal(0), m_calls(0), m_timeAverageIndex(0), m_timeAverageSum(0), m_timeLatest(0) {}

	void Start() {
		if (m_started) {
			return;
		}
		m_calls++;
		m_cyclesBegin = __rdtsc();
		m_timeBegin = GetCurrentMS();
		m_started = true;
	}

	void End() {
		if (!m_started) {
			return;
		}
		m_cyclesLatest = __rdtsc() - m_cyclesBegin;
		m_cyclesTotal += m_cyclesLatest;

		m_timeLatest = double(GetCurrentMS() - m_timeBegin);
		CalculateAverage(m_timeLatest);
		m_started = false;

	}

	void CalculateAverage(double newtick) {
		m_timeAverageSum -= m_timeAverageList[m_timeAverageIndex];
		m_timeAverageSum += newtick;
		m_timeAverageList[m_timeAverageIndex] = newtick;
		if (++m_timeAverageIndex == 600) m_timeAverageIndex = 0;
	}

	const char* GetFileName() { return m_fileName; }
	const char* GetFunctionName() { return m_functionName; }

	__int64 GetCalls() { return m_calls; }
	__int64 GetCycles() { return m_cyclesLatest; }
	double GetLatest() { return m_timeLatest; }
	double GetAverage() { return (m_timeAverageSum / 600.0); }
};

class Profiler {
private:
	map<const char*, ProfileEntry> m_entries;
public:
	ProfileEntry* GetEntry(const char* fileName, const char* functionName) {
		if (m_entries.find(functionName) != m_entries.end()) return &m_entries[functionName];
		m_entries[functionName] = ProfileEntry(fileName, functionName);
		return &m_entries[functionName];
	}

	void Push(const char* fileName, const char* functionName) {
		GetEntry(fileName, functionName)->Start();
	}

	void Pop(const char* fileName, const char* functionName) {
		ProfileEntry* entry = GetEntry(fileName, functionName);
		entry->End();
	}

	vector<ProfileEntry> GetEntries() {
		vector<ProfileEntry> toRet;
		for (auto it = m_entries.begin(); it != m_entries.end(); ++it) {
			toRet.push_back(it->second);
		}
		return toRet;
	}
};

Profiler* GetProfiler();

class Test {
public:
	const char* m_fileName;
	const char* m_functionName;
	bool m_hasEnded;
	Test(const char* fileName, const char* functionName) : m_fileName(fileName), m_functionName(functionName), m_hasEnded(false) { GetProfiler()->Push(m_fileName, functionName); }
	~Test() {
		end();
	}

	void end() {
		if (!m_hasEnded) GetProfiler()->Pop(m_fileName, m_functionName);
		m_hasEnded = true;
	}
};