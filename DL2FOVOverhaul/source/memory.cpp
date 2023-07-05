#include <Windows.h>
#include <Psapi.h>
#include <malloc.h>

const bool IsAddressValid(LPVOID ptr) {
	const DWORD64 MIN_VALID_ADDRESS = 0x180000000;
	const DWORD64 MAX_VALID_ADDRESS = 0x7FFFFFFFFFFF;

	const DWORD64 ptrValue = reinterpret_cast<DWORD64>(ptr);
	return ptrValue != NULL && (ptrValue >= MIN_VALID_ADDRESS && ptrValue <= MAX_VALID_ADDRESS);
}
const bool IsAddressValid(DWORD64 ptr) {
	const DWORD64 MIN_VALID_ADDRESS = 0x180000000;
	const DWORD64 MAX_VALID_ADDRESS = 0x7FFFFFFFFFFF;

	return ptr != NULL && (ptr >= MIN_VALID_ADDRESS && ptr <= MAX_VALID_ADDRESS);
}

const bool IsValidPtr(LPVOID ptr) {
	return IsAddressValid(ptr) && !IsBadReadPtr(ptr, sizeof(LPVOID));
}
const bool IsValidPtr(DWORD64 ptr) {
	return IsAddressValid(ptr) && !IsBadReadPtr(reinterpret_cast<LPVOID>(ptr), sizeof(LPVOID));
}

const MODULEINFO GetModuleInfo(const char* szModule) {
	MODULEINFO moduleInfo{};
	const HMODULE hModule = GetModuleHandle(szModule);
	if (hModule == 0)
		return moduleInfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(MODULEINFO));
	return moduleInfo;
}