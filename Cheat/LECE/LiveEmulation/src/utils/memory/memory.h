#pragma once

class Memory {
public:
	static void ChangePageProtection(DWORD Address, int Size, int NewProtection, DWORD* OldProtection);
	static void RestorePageProtection(DWORD Address, int Size, DWORD* OldProtection);
	static DWORD Scan(string signature);
};

class MultilayerPointer {
public:
	MultilayerPointer(DWORD address);
	MultilayerPointer& Add(DWORD offset);
	DWORD Get();
private:
	vector<DWORD> m_offsets;
	DWORD m_address;
};