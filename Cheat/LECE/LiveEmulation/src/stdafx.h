#pragma once

#pragma warning(disable: 4018)
#pragma warning(disable: 4244)
#pragma warning(disable: 4800)

#include <xtl.h>
#include <xboxmath.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <time.h>
#include <vector>
#include <array>
#include <ppcintrinsics.h>
#include <math.h>
#include <fstream>
#include <cstdio>
#include <tchar.h>

using namespace std;

#define DEVELOPER_BUILD
#define NUMOF(x) (sizeof(x)/sizeof(x[0]))

/*Memory helper*/
#include "utils/memory/memory.h"

/*Utils*/
#include "utils/profiler.h"
#include "utils/output/log.h"
#include "utils/output/launch.h"
#include "utils/kernel.h"
#include "utils/enums.h"
#include "utils/structures.h"
#include "utils/utilities.h"
#include "utils/color.h"
#include "utils/threadPool.h"
#include "utils/fiberManager.h"
#include "utils/hypervisor.h"
#include "utils/notify.h"
#include "utils/config.h"

/*Encryption*/
#include "security/integrityManager.h"
#include "security/encryption.h"

/*Auth*/
#include "authorization/network/requests.h"

/*Backend*/
#include "backend/backendHooking.h"
#include "backend/backendMain.h"
#include "backend/backendSecurity.h"

/*Invoker*/
#include "engine/invoker.h"
#include "engine/natives.h"

/*Auth*/
#include "authorization/network/network.h"

/*Renderer*/
#include "menu/renderer.h"

/*Input*/
#include "utils/input/controller.h"

/*Options*/
#include "menu/options/option.h"
#include "menu/options/buttonOption.h"
#include "menu/options/submenuOption.h"
#include "menu/options/toggleOption.h"
#include "menu/options/scrollOption.h"
#include "menu/options/numberOption.h"
#include "menu/options/colorOption.h"
#include "menu/options/breakOption.h"

/*Submenu*/
#include "menu/submenu.h"
#include "menu/submenuHandler.h"

/*Menu*/
#include "menu/menu.h"
#include "menu/util/menuInput.h"
#include "menu/util/menuFunctions.h"
//#include "menu/util/playerManager.h"