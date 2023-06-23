#pragma once
#include <windows.h>
#include <vector>
#include <Psapi.h>

MODULEINFO GetModuleInfo(const char* szModule);

extern PBYTE FindPattern(const PBYTE rangeStart, DWORD64 len, const char* pattern);

extern const bool IsAddressValid(LPVOID ptr);