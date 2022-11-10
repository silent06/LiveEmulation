#include "stdafx.h"

IntegrityManager g_IntegrityManager;

IntegrityManager* GetIntegrityManager() {
	return &g_IntegrityManager;
}