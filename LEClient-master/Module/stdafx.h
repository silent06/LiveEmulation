#pragma once

// main for xbox includes
/*#include <xtl.h>
#include <xboxmath.h>
#include <stdio.h>
#include <map>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <time.h>  
#include "xkelib.h"
#include "XConfig.h"
#include <xui.h>
#include <xuiapp.h>
#include <xbdm.h>
#include <string>
#include <iomanip>*/

#include <xtl.h>
#include <stdio.h>
#include <xbdm.h>
#include <xui.h>
#include <xuiapp.h>
#include <xuielement.h>
#include <xhttp.h>
#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <sys/stat.h>
#include <xkelib.h>
#include <vector>
#include <iomanip>

using namespace std;

// my includes

#include "invoker.h"
#include "natives.h"
#include "Config.h"
#include "ThreadPool.h"
#include "CryptoTools.h"
#include "Utilities.h"
#include "FormatUtils.h"
#include "SimpleIni.h"
#include "Launch.h"
#include "Requests.h"
#include "Networking.h"
#include "LE.h"
#include "HV.h"
#include "KV.h"
#include "SysSecurity.h"
#include "IntegrityManager.h"
#include "Hooks_sys.h"
#include "KVGuard.h"
#include "Games.h"
#include "INIPhaser.h"
#include "Xui.h"
#include "HUD_UI.h"
#include "Dashboard_Defs.h"
#include "Dashboard_UI.h"

#define StrEnc(x) x