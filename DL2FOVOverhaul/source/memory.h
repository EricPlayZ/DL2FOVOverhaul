#pragma once
#include <Windows.h>
#include <Psapi.h>

extern const bool IsAddressValid(LPVOID ptr);
extern const bool IsAddressValid(DWORD64 ptr);

extern MODULEINFO GetModuleInfo(const char* szModule);

extern PBYTE FindPattern(const PBYTE rangeStart, DWORD64 len, const char* pattern);