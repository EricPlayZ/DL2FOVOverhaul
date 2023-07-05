#include "SigScan.h"

// macro for lazily adding signatures to this sig list
#define AddOffset(name, moduleName, pattern, type) \
static DWORD64 Get_## name () {\
	static DWORD64 name = NULL;\
	if (IsValidPtr(name)) return name; \
	return name=(DWORD64)PatternScanner::FindPattern(moduleName, {pattern, type});\
} 

// macro for lazily adding offsets to the sig list
#define AddStaticOffset(name, off) \
static DWORD64 Get_## name () {\
	static DWORD64 name = NULL;\
	if (name != NULL) return name; \
	return name=(DWORD64)off;\
} 

// same but to resolve a call (call func)
#define AddFuncCall(name, pattern, type) \
static DWORD64 Get_## name () {\
	static DWORD64 name = NULL;\
	if (name != NULL) return name; \
	DWORD64 val = (DWORD64)PatternScanner::FindPattern({pattern, type}); \
	return name = val + 5 + (*(int*)(val + 1)); \
} 

// Static offsets go here
struct StaticOffsets {
	AddOffset(CLobbySteamOffset, "engine_x64_rwdi.dll", "48 8B 05 [?? ?? ?? ?? 48 85 C0 74 05 48 83 C0 08 C3", PatternType::RelativePointer)
};

#undef AddFuncCall
#undef AddOffset
#undef AddStaticOffset