#pragma once
#include <windows.h>
#include <vector>

extern DWORD64 GetPointerAddr(DWORD64 baseAddress, std::vector<DWORD64> offsets);