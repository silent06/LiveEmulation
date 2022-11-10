#include "stdafx.h"

#pragma pack(1)
typedef enum _ODD_POLICY {
	ODD_POLICY_FLAG_CHECK_FIRMWARE = 0x120,
} ODD_POLICY;

typedef union _INQUIRY_DATA {
	struct {
		BYTE DeviceType : 5;
		BYTE DeviceTypeQualifier : 3;
		BYTE DeviceTypeModifier : 7;
		BYTE RemovableMedia : 1;
		BYTE Versions : 8;
		BYTE ResponseDataFormat : 4;
		BYTE HiSupport : 1;
		BYTE NormACA : 1;
		BYTE ReservedBit : 1;
		BYTE AERC : 1;
		BYTE AdditionalLength : 8;
		WORD Reserved : 16;
		BYTE SoftReset : 1;
		BYTE CommandQueue : 1;
		BYTE Reserved2 : 1;
		BYTE LinkedCommands : 1;
		BYTE Synchronous : 1;
		BYTE Wide16Bit : 1;
		BYTE Wide32Bit : 1;
		BYTE RelativeAddressing : 1;
		BYTE VendorId[8];
		BYTE ProductId[16];
		BYTE ProductRevisionLevel[4];
	};
	BYTE Data[0x24];
} INQUIRY_DATA, *pINQUIRY_DATA;

typedef struct _XEIKA_ODD_DATA {
	BYTE         Version;
	BYTE         PhaseLevel;
	INQUIRY_DATA InquiryData;
} XEIKA_ODD_DATA, *PXEIKA_ODD_DATA;

typedef struct _XEIKA_DATA {
	XECRYPT_RSAPUB_2048 PublicKey;
	DWORD               Signature;
	WORD                Version;
	XEIKA_ODD_DATA      OddData;
	BYTE                Padding[4];
} XEIKA_DATA, *PXEIKA_DATA;

typedef struct _XEIKA_CERTIFICATE {
	WORD       Size;
	XEIKA_DATA Data;
	BYTE       Padding[0x1146];
} XEIKA_CERTIFICATE, *PXEIKA_CERTIFICATE;

typedef struct _KEY_VAULT {
	BYTE  HmacShaDigest[0x10]; // 0x00
	BYTE  Confounder[0x08]; // 0x10
	BYTE  ManufacturingMode; // 0x18
	BYTE  AlternateKeyVault; // 0x19
	BYTE  RestrictedPrivilegesFlags; // 0x1A
	BYTE  ReservedByte3; // 0x1B
	WORD  OddFeatures; // 0x1C
	WORD  OddAuthtype; // 0x1E
	DWORD RestrictedHvextLoader; // 0x20
	DWORD PolicyFlashSize; // 0x24
	DWORD PolicyBuiltinUsbmuSize; // 0x28
	DWORD ReservedDword4; // 0x2C
	QWORD RestrictedPrivileges; // 0x30
	QWORD ReservedQword2; // 0x38
	QWORD ReservedQword3; // 0x40
	QWORD ReservedQword4; // 0x48
	BYTE  ReservedKey1[0x10]; // 0x50
	BYTE  ReservedKey2[0x10]; // 0x60
	BYTE  ReservedKey3[0x10]; // 0x70
	BYTE  ReservedKey4[0x10]; // 0x80
	BYTE  ReservedRandomKey1[0x10]; // 0x90
	BYTE  ReservedRandomKey2[0x10]; // 0xA0
	BYTE  ConsoleSerialNumber[0xC]; // 0xB0
	BYTE  MoboSerialNumber[0xC]; // 0xBC
	WORD  GameRegion; // 0xC8
	BYTE  Padding1[0x6]; // 0xCA
	BYTE  ConsoleObfuscationKey[0x10]; // 0xD0
	BYTE  KeyObfuscationKey[0x10]; // 0xE0
	BYTE  RoamableObfuscationKey[0x10]; // 0xF0
	BYTE  DvdKey[0x10]; // 0x100
	BYTE  PrimaryActivationKey[0x18]; // 0x110
	BYTE  SecondaryActivationKey[0x10]; // 0x128
	BYTE  GlobalDevice2desKey1[0x10]; // 0x138
	BYTE  GlobalDevice2desKey2[0x10]; // 0x148
	BYTE  WirelessControllerMs2desKey1[0x10]; // 0x158
	BYTE  WirelessControllerMs2desKey2[0x10]; // 0x168
	BYTE  WiredWebcamMs2desKey1[0x10]; // 0x178
	BYTE  WiredWebcamMs2desKey2[0x10]; // 0x188
	BYTE  WiredControllerMs2desKey1[0x10]; // 0x198
	BYTE  WiredControllerMs2desKey2[0x10]; // 0x1A8
	BYTE  MemoryUnitMs2desKey1[0x10]; // 0x1B8
	BYTE  MemoryUnitMs2desKey2[0x10]; // 0x1C8
	BYTE  OtherXsm3DeviceMs2desKey1[0x10]; // 0x1D8
	BYTE  OtherXsm3DeviceMs2desKey2[0x10]; // 0x1E8
	BYTE  WirelessController3p2desKey1[0x10]; // 0x1F8
	BYTE  WirelessController3p2desKey2[0x10]; // 0x208
	BYTE  WiredWebcam3p2desKey1[0x10]; // 0x218
	BYTE  WiredWebcam3p2desKey2[0x10]; // 0x228
	BYTE  WiredController3p2desKey1[0x10]; // 0x238
	BYTE  WiredController3p2desKey2[0x10]; // 0x248
	BYTE  MemoryUnit3p2desKey1[0x10]; // 0x258
	BYTE  MemoryUnit3p2desKey2[0x10]; // 0x268
	BYTE  OtherXsm3Device3p2desKey1[0x10]; // 0x278
	BYTE  OtherXsm3Device3p2desKey2[0x10]; // 0x288
	XECRYPT_RSAPRV_1024 ConsolePrivateKey; // 0x298
	XECRYPT_RSAPRV_2048 XeikaPrivateKey; // 0x468
	XECRYPT_RSAPRV_1024 CardeaPrivateKey; // 0x7F8
	XE_CONSOLE_CERTIFICATE ConsoleCertificate; // 0x9C8
	XEIKA_CERTIFICATE XeikaCertificate; // 0xB70
	BYTE  KeyVaultSignature[0x100]; // 0x1DF8
	BYTE  CardeaCertificate[0x2108]; // 0x1EF8
} KEY_VAULT, *PKEY_VAULT;
C_ASSERT(sizeof(KEY_VAULT) == 0x4000);

#pragma pack()

class KV {
public:

	static BOOL IsDevkit;
	static BOOL CRL;
	static BOOL FCRT;

	static BYTE StockMacAddress[0x6];
	static BYTE sMacAddress[0x6];
	static BYTE CpuKey[0x10];
	static BYTE KvCpuKey[0x10];
	static BYTE HmacKVHash[0x14];

	static DWORD UpdateSequence;

	static KEY_VAULT KeyVault;
	static DWORD SetupKeyvault();
};

