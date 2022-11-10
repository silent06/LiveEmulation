#include "stdafx.h"

void* CryptoTools::EncryptedMemcpy(BYTE* Dest, BYTE* Src, INT Length) {
	CopyMemory(Dest, Src, Length);

	for (auto i = 0; i < Length; i++) 
		Dest[i] ^= (Length + 1);
	return ERROR_SUCCESS;
}

void CryptoTools::CreateAesSbox(DWORD r3, DWORD r4) {

	DWORD Rounds = 0xA;

	DWORD r10 = r3;
	DWORD r9 = (r3 + 4);
	DWORD r8 = (r3 + 8);
	DWORD r7 = (r3 + 12);
	DWORD r5 = (r4 + 0x10F);

	do {

		DWORD r11 = r7 & 0x1;
		DWORD r31 = r5;
		DWORD r30 = (r11 >> 16) & 0x6;
		DWORD r29 = (r11 >> 8) & 0xA;
		DWORD r28 = (r11 & 0x9) + 0x1;

		r11 = r11 >> 24;
		r30 = (r30 + r4);
		r30 = (r29 + r4);
		r31 ^= r30;
		r31++;
		r30 = (r28 + r4);
		r11 = (r11 + r4);
		r11 += r4;
		r31 = r31 << 8;
		r31 = r31 ^ r29;
		r31 = r31 << 8;
		r31 = r31 ^ r30;
		r31 = r31 << 8;
		r11 = r31 ^ r11;
		r10 = r11 ^ r10;
		r9 = r9 ^ r10;
		r8 = r8 ^ r9;
		r7 = r7 ^ r8;
		r3 ^= (r31 + r9);
		r3 -= 1;

		r5--;
		Rounds--;
	} while (Rounds != 0);
	r3 ^= (r9 + r10);
	r4 ^= (r7 + r5);
}

// doesnt seem to be any max values requirements however iv and key must probably be 0x10 alligned 
void CryptoTools::AesCbcEncrypt(BYTE* pbAes_input, BYTE* pbIV, BYTE* pbAes_Key, bool bProtect, DWORD dwSize) {

	XECRYPT_AES_STATE AesState;
	Native::Kernel::XeCryptAesKey(&AesState, pbAes_Key);
	Native::Kernel::XeCryptAesCbc(&AesState, pbAes_input, dwSize, pbAes_input, pbIV, bProtect);
}

// probably only works with 0x10 byte data inputs as it doesnt take a size needs testing
void CryptoTools::Des3Ecb(BYTE* pbDes3_input, BYTE* pbDes3_Key, bool bProtect) {

	XECRYPT_DES3_STATE Des3State;
	Native::Kernel::XeCryptDes3Key(&Des3State, pbDes3_Key);
	Native::Kernel::XeCryptDes3Ecb(&Des3State, pbDes3_input, pbDes3_input, bProtect);
}
