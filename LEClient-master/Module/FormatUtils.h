#include "stdafx.h"
#pragma once

class FormatUtils {
public:

	static DWORD HexStringToByteArray(BYTE* pbArray, CHAR* pchHexString, DWORD dwLength);
	static INT toWCHAR(CHAR* input, PWCHAR output);
	static PWCHAR toWCHAR(CHAR* Text, ...);
	static CHAR* toCHAR(PWCHAR Text);
	static PWCHAR LinkWChar(PWCHAR Text, ...);
	static CHAR* LinkChar(CONST CHAR* Text, ...);
	static void ReplaceString(CONST CHAR* newtext, DWORD addr, INT length);
};

