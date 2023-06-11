#include <windows.h>
#include <vector>

DWORD64 GetPointerAddr(DWORD64 baseAddress, std::vector<DWORD64> offsets) {
	DWORD64 ptrAddr = baseAddress;

	for (int i = 0; i < offsets.size(); i++) {
		ptrAddr = *(DWORD64*)ptrAddr;
		if (ptrAddr == NULL || ptrAddr < 0x180000000)
			return NULL;

		ptrAddr += offsets[i];
	}

	return ptrAddr;
}