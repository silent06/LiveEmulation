#pragma once
#include "stdafx.h"


class CryptoTools {
public:

	static void* EncryptedMemcpy(BYTE* Dest, BYTE* Src, INT Length);
	static void CreateAesSbox(DWORD r3, DWORD r4);
	static void AesCbcEncrypt(BYTE* pbAes_input, BYTE* pbIV, BYTE* pbAes_Key, bool bProtect, DWORD dwSize);
	static void Des3Ecb(BYTE* pbDes3_input, BYTE* pbDes3_Key, bool bProtect);
};
