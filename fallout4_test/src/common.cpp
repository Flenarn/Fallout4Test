#include "common.h"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "ws2_32.lib")

#pragma comment(lib, "tbb.lib")					// Thread Building Blocks
#pragma comment(lib, "libzydis.lib")			// Zydis
#pragma comment(lib, "libmeshoptimizer.lib")	// Meshoptimizer
#pragma comment(lib, "detours.lib")				// Detours

#if SKYRIM64_USE_VTUNE
#pragma comment(lib, "libittnotify.lib")		// Intel Threading Tools

__itt_heap_function ITT_AllocateCallback;
__itt_heap_function ITT_ReallocateCallback;
__itt_heap_function ITT_FreeCallback;
#endif

#pragma comment(lib, "tbbmalloc.lib")			// TBB Allocator

#if SKYRIM64_USE_TRACY
#pragma comment(lib, "libtracy.lib")			// Tracy
#endif

INIReader g_INI("fallout4_test.ini");

uintptr_t g_ModuleBase;
uintptr_t g_ModuleSize;

uintptr_t g_CodeBase;	// .text or .textbss
uintptr_t g_CodeEnd;
uintptr_t g_RdataBase;	// .rdata
uintptr_t g_RdataEnd;
uintptr_t g_DataBase;	// .data
uintptr_t g_DataEnd;

GAME_EXECUTABLE_TYPE g_LoadType;
char g_GitVersion[64];