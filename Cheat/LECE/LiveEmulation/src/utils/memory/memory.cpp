#include "stdafx.h"
#include <sstream>

struct PatternByte {
	PatternByte() : ignore(true) {
		//
	}

	PatternByte(string byteString, bool ignoreThis = false) {
		data = StringToUint8(byteString);
		ignore = ignoreThis;
	}

	bool ignore;
	UINT8 data;

private:
	UINT8 StringToUint8(string str) {
		istringstream iss(str);

		UINT32 ret;

		if (iss >> hex >> ret) {
			return (UINT8)ret;
		}

		return 0;
	}
};

MultilayerPointer::MultilayerPointer(DWORD address)
	: m_address(address)
{}

MultilayerPointer& MultilayerPointer::Add(DWORD offset) {
	m_offsets.push_back(offset);
}

DWORD MultilayerPointer::Get() {
	int ptr = *(int*)(m_address);
	if (!ptr) {
		return 0;
	}

	auto level = m_offsets.size();

	for (auto i = 0; i < level; i++) {
		if (i == level - 1) {
			ptr += m_offsets[i];
			if (!ptr) return 0;
			return ptr;
		}
		else {
			ptr = *(int*)(ptr + m_offsets[i]);
			if (!ptr) return 0;
		}
	}

	return ptr;
}

void Memory::ChangePageProtection(DWORD Address, int Size, int NewProtection, DWORD* OldProtection) {
	VirtualProtect((void*)Address, Size, NewProtection, OldProtection);
}

void Memory::RestorePageProtection(DWORD Address, int Size, DWORD* OldProtection) {
	VirtualProtect((void*)Address, Size, *OldProtection, OldProtection);
}

DWORD Memory::Scan(string signature) {
	PLDR_DATA_TABLE_ENTRY mod = (PLDR_DATA_TABLE_ENTRY)GetModuleHandle(0);
	if (mod) {
		DWORD start = (DWORD)mod->EntryPoint;
		DWORD end = mod->SizeOfFullImage;

		vector<PatternByte> p;
		istringstream iss(signature);
		string w;

		while (iss >> w) {
			if (w.data()[0] == '?') {
				p.push_back(PatternByte());
			}
			else if (w.length() == 2 && isxdigit(w.data()[0]) && isxdigit(w.data()[1])) {
				p.push_back(PatternByte(w));
			}
			else return 0;
		}

		for (DWORD i = 0; i < end; i++) {
			UINT8* currentByte = (UINT8*)(start + i);

			bool found = true;

			for (size_t ps = 0; ps < p.size(); ps++) {
				if (p[ps].ignore == false && currentByte[ps] != p[ps].data) {
					found = false;
					break;
				}
			}

			if (found) {
				return (DWORD)currentByte;
			}
		}
	}

	return 0;
}