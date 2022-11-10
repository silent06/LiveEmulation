#include "stdafx.h"

NetSocket g_netSocket;

NetSocket* GetNetSocket() {
	return &g_netSocket;
}