#pragma once
#include <Windows.h>
#include <Psapi.h>

extern const bool IsValidPtr(LPVOID ptr);
extern const bool IsValidPtr(DWORD64 ptr);

extern const MODULEINFO GetModuleInfo(const char* szModule);