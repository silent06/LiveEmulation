#pragma once

class Option;

class Submenu {
protected:
	vector<shared_ptr<Option>> m_options;
	const char* m_name;
	const char* m_configSub;
	Submenu* m_parentSubmenu;

public:
	Submenu() : m_name(""), m_parentSubmenu(nullptr), m_oldCurrentOption(0), m_oldScrollOffset(0) {};

	int m_oldCurrentOption;
	int m_oldScrollOffset;

	int GetOptionAmount() {
		int size = 0;
		for (size_t i = 0; i < m_options.size(); i++) {
			if (m_options[i]->IsVisible()) size++;
		}
		return size;
	}

	vector<shared_ptr<Option>> GetOptions() { return m_options; };

	void MenuUpdate();

	virtual void Init(); // Option setup
	virtual void UpdateOnce(); // Update once when submenu entered
	virtual void Update(); // Update while submenu active
	virtual void FeatureUpdate(); // Background Update
	virtual void InitVars(); // init vars

	template<typename T>
	shared_ptr<T> addOption(T& option) {
		shared_ptr<T> ptr = make_shared<T>(option);
		m_options.push_back(ptr);
		return ptr;
	}

	template<typename T>
	void addOption(shared_ptr<T>& option) {
		m_options.push_back(option);
	}

	void SetName(const char* name, const char* configName = NULL) { 
		m_name = name; 

		if (configName == NULL) {
			m_configSub = m_name;
		} else m_configSub = configName;
	};

	template<typename T>
	void SetParentSubmenu() {
		m_parentSubmenu = T::GetInstance();
	};

	const char* GetName() { return m_name; };
	const char* GetConfigName() { return m_configSub; };
	Submenu* GetParentSubmenu() { return m_parentSubmenu; };

	/*Clear options from the submenu*/
	void ClearOptions() {
		m_options.clear();
	}

	/*Clear options from a starting offset*/
	void ClearOptionsOffset(int offset) {
		m_options.resize(offset);
	}
};