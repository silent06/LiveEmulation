#pragma once

enum eResolvedTypes {
	eBOOL,
	eCOLOR,
	eINT,
	eFLOAT,
	eINTTOGGLE,
	eFLOATTOGGLE,
};

struct ConfigResolvedItems {
	string m_submenu;
	string m_option;
	eResolvedTypes m_type;
	bool m_bool;
	RGBA m_color;
	int m_int;
	float m_float;
};

struct ConfigItems {
	string m_submenu;
	string m_option;
	string m_type;
	string m_value;
	string m_extra;

	friend bool operator==(const ConfigItems& left, pair<string, string> right) {
		return !strcmp(left.m_submenu.c_str(), right.first.c_str()) && !strcmp(left.m_option.c_str(), right.second.c_str());
	}
};

class Config {
public:
	static vector<ConfigItems> m_configItemsToSave;

	void GetFilesFromDirectory(vector<string> &invec, string folder, string specificExtension = "*") {
		invec.clear();
		string loc = folder + specificExtension;
		WIN32_FIND_DATAA fd;
		HANDLE hFind = FindFirstFile(loc.c_str(), &fd);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					string orig = fd.cFileName;
					string newstring = orig.substr(0, orig.size() - (specificExtension.size() - 1));
					invec.push_back(newstring);
				}
			} while (FindNextFile(hFind, &fd));
			FindClose(hFind);
		} else {
			printf("err: %i\n", GetLastError());
		}
	}

	void CreateDir(char* dir) {
		if (!Utils::DoesDirExist(dir)) {
			CreateDirectoryA(dir, NULL);
		}
	}
};

Config* GetConfig();