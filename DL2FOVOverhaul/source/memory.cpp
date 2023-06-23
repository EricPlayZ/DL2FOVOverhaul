#include <Windows.h>
#include <Psapi.h>
#include <malloc.h>

#define INRANGE(x,a,b)		(x >= a && x <= b) 
#define getBits( x )		(INRANGE(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define getByte( x )		(getBits(x[0]) << 4 | getBits(x[1]))

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

MODULEINFO GetModuleInfo(const char* szModule) {
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandle(szModule);
	if (hModule == 0) return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

static bool IsMatch(const PBYTE addr, const PBYTE pat, const PBYTE msk) {
	size_t n = 0;
	while (addr[n] == pat[n] || msk[n] == (BYTE)'?') {
		if (!msk[++n])
			return true;
	}
	return false;
}
PBYTE FindPattern(const PBYTE rangeStart, DWORD64 len, const char* pattern) {
	size_t l = strlen(pattern);
	PBYTE patt_base = static_cast<PBYTE>(_malloca(l >> 1));
	PBYTE msk_base = static_cast<PBYTE>(_malloca(l >> 1));
	PBYTE pat = patt_base;
	PBYTE msk = msk_base;
	l = 0;
	while (*pattern) {
		if (*pattern == ' ')
			pattern++;
		if (!*pattern)
			break;
		if (*(PBYTE)pattern == (BYTE)'\?') {
			*pat++ = 0;
			*msk++ = '?';
			pattern += ((*(PWORD)pattern == (WORD)'\?\?') ? 2 : 1);
		} else {
			if (IsAddressValid(pat))
				*pat++ = getByte(pattern);
			*msk++ = 'x';
			pattern += 2;
		}
		l++;
	}
	if (IsAddressValid(msk))
		*msk = 0;
	pat = patt_base;
	msk = msk_base;
	for (DWORD64 n = 0; n < (len - l); ++n) {
		if (IsMatch(rangeStart + n, patt_base, msk_base))
			return rangeStart + n;
	}
	return NULL;
}