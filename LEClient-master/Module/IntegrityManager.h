#pragma once

struct IntegrityItem {
	void* m_function;
	int m_size;
	vector<DWORD> m_bytes;
};

class IntegrityManager {
public:
	void Add(void* function, int size) {
		Security::AddRegionToWhitelist((DWORD)function, size + 0x4);

		IntegrityItem item;
		item.m_function = function;
		item.m_size = size;

		for (int i = 0; i < size; i++) {
			item.m_bytes.push_back(*(DWORD*)((DWORD)function + i));
		}

		m_items.push_back(item);
	}
	vector<IntegrityItem> GetItems() { return m_items; };
private:
	vector<IntegrityItem> m_items;
};

IntegrityManager* GetIntegrityManager();