#pragma once
#include "stdafx.h"

#define BASEADDY 0x90DF0000 
//#define DEVKIT
#define DEVELOPERSERVER
//#define DUMPS
#define DEBUG
//#define LOCAL
//#define USE_SECURITY

#define LAUNCH_MOUNT "LE:\\"
#define XEX_Path LAUNCH_MOUNT "LiveEmulation.xex"
#define INI_Path LAUNCH_MOUNT "LE.ini"
#define LAUNCH_INI_PATH LAUNCH_MOUNT "launch.ini"
#define CPUKEY_PathBin LAUNCH_MOUNT "cpukey.bin"
#define CPUKEY_PathTxt LAUNCH_MOUNT "cpukey.txt"
#define KV_Path LAUNCH_MOUNT "kv.bin"
#define LOGPath LAUNCH_MOUNT "LEErrorReporting.log"
#define XSCDUMP_Path LAUNCH_MOUNT "XSC.bin"
#define XKEDUMP_Path LAUNCH_MOUNT "LE_XKE.bin"

#define CALLER XNCALLER_SYSAPP // set the caller for all Networking activity, must be sysapp for use in a sysdll
#define CUR_VER (((XboxKrnlVersion->Major & 0xF) << 28) | ((XboxKrnlVersion->Minor) << 24) | ((XboxKrnlVersion->Build &0xFFFF)<<8) | ((XboxKrnlVersion->Qfe&0xFF)))
#define SEND_RECV_SIZE 2048
#define MaxReconAttempts 0x3