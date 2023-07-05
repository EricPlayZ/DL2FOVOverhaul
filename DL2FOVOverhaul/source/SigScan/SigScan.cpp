#include "SigScan.h"
#include <iostream>

DWORD64 GetModuleSize(const char* module)
{
	MODULEINFO hModuleInfo{};

	HMODULE hModule = GetModuleHandle(module);
	if (hModule == 0)
		return 0;

	GetModuleInformation(GetCurrentProcess(), hModule, &hModuleInfo, sizeof(MODULEINFO));
	return hModuleInfo.SizeOfImage;
}

void *PatternScanner::FindPattern(const std::string &ModuleName, const Pattern &pattern)
{
	return PatternScanner::FindPattern(GetModuleHandle(ModuleName.c_str()), GetModuleSize(ModuleName.c_str()), pattern);
}

void *PatternScanner::FindPattern(const Pattern &pattern)
{
	MODULEINFO hModuleInfo;
	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &hModuleInfo, sizeof(hModuleInfo));
	return PatternScanner::FindPattern(GetModuleHandle(NULL), hModuleInfo.SizeOfImage, pattern);
}

std::vector<void*> PatternScanner::FindPatterns(const std::string &ModuleName, const Pattern &pattern)
{
	return PatternScanner::FindPatterns(GetModuleHandle(ModuleName.c_str()), GetModuleSize(ModuleName.c_str()), pattern);
}

std::vector<void*> PatternScanner::FindPatterns(void *startAddress, DWORD64 searchSize, const Pattern &pattern)
{
	std::vector<void*> ret;

	void *base = startAddress;
	DWORD64 size = searchSize;
	void *addr = base;
	do
	{
		addr = PatternScanner::FindPattern((void*)((DWORD64)addr + 1), static_cast<DWORD64>((size - ((DWORD64)addr - (DWORD64)base + 1))), pattern);
		if (addr)
			ret.push_back(addr);
	} while (addr != nullptr);

	return ret;
}

void *PatternScanner::FindPattern(void *startAddress, DWORD64 searchSize, const Pattern &pattern)
{
	size_t len = strlen(pattern.pattern);
	if (len == 0)
		return nullptr;

	DWORD64 pos = 0;

	size_t byteCount = 1;
	uint32_t i = 0;
	while (i < len - 1)
	{
		if (pattern.pattern[i] == ' ')
			byteCount++;
		i++;
	}

	byte *patt = (byte*)malloc(byteCount + 1);
	if (!patt)
		return nullptr;

	byte *mask = (byte*)malloc(byteCount + 1);
	if (!mask)
		return nullptr;

	int offset = 0;
	int bytesCounted = 0;
	i = 0;
	while (i < len - 1)
	{
		if (pattern.pattern[i] == '[')
		{
			i++;
			offset = bytesCounted;
		}

		if (pattern.pattern[i] == '\0')
			break;

		if (pattern.pattern[i] == '?' && pattern.pattern[i + 1] == '?')
		{
			mask[bytesCounted] = '?';
			patt[bytesCounted] = '\0';
		}
		else
		{
			byte hn = pattern.pattern[i] > '9' ? pattern.pattern[i] - 'A' + 10 : pattern.pattern[i] - '0';
			byte ln = pattern.pattern[i + 1] > '9' ? pattern.pattern[i + 1] - 'A' + 10 : pattern.pattern[i + 1] - '0';
			byte n = (hn << 4) | ln;

			mask[bytesCounted] = 'x';
			patt[bytesCounted] = n;
		}

		bytesCounted++;

		i += 2;
		while (i < len && (pattern.pattern[i] == ' ' || pattern.pattern[i] == '\t' || pattern.pattern[i] == '\r' || pattern.pattern[i] == '\n'))
			i++;
	}
	if (bytesCounted <= byteCount)
		mask[bytesCounted] = '\0';

	size_t searchLen = strlen((char*)&mask[0]) - 1;

	void *ret = nullptr;
	for (DWORD64 retAddress = (DWORD64)startAddress; retAddress < (DWORD64)startAddress + searchSize; retAddress++)
	{
		__try
		{
			if (*(byte*)retAddress == patt[pos] || mask[pos] == '?')
			{
				if (bytesCounted <= byteCount && pos < bytesCounted && mask[pos + 1] == '\0')
				{
					ret = (void*)(retAddress - searchLen + offset);
					break;
				}

				pos++;
			}
			else
				pos = 0;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			pos = 0;
		}
	}

	free(patt);
	free(mask);

	switch (pattern.type)
	{
	case PatternType::Pointer:
		ret = PatternScanner::ResolvePtr<DWORD64>(ret);
		break;

	case PatternType::PointerBYTE:
		ret = PatternScanner::ResolvePtr<BYTE>(ret);
		break;

	case PatternType::PointerWORD:
		ret = PatternScanner::ResolvePtr<WORD>(ret);
		break;

	case PatternType::PointerQWORD:
		ret = PatternScanner::ResolvePtr<DWORD64>(ret);
		break;

	case PatternType::RelativePointer:
		ret = PatternScanner::ResolveRelativePtr<int32_t>(ret);
		break;

	case PatternType::RelativePointerBYTE:
		ret = PatternScanner::ResolveRelativePtr<BYTE>(ret);
		break;

	case PatternType::RelativePointerWORD:
		ret = PatternScanner::ResolveRelativePtr<WORD>(ret);
		break;

	case PatternType::RelativePointerQWORD:
		ret = PatternScanner::ResolveRelativePtr<DWORD64>(ret);
		break;
	}

	return ret;
}

void *PatternScanner::FindPattern(void *startAddress, DWORD64 searchSize, Pattern *patterns, float *ratio)
{
	int totalCount = 0;
	typedef struct
	{
		void *addr;
		int count;
	} result;
	std::list<result> results;
#ifdef DEVELOPER
	std::list<void*> rawResults;
#endif

	int bestCount = 0;
	void *ret = nullptr;

	while (true)
	{
		Pattern pattern = patterns[totalCount];
		if (!pattern.pattern)
			break;

		totalCount++;

		void *addr = PatternScanner::FindPattern(startAddress, searchSize, pattern);
#ifdef DEVELOPER
		rawResults.push_back(addr);
#endif
		if (!addr)
			continue;

		bool found = false;
		for (result &res : results)
		{
			if (res.addr == addr)
			{
				res.count++;
				if (res.count > bestCount)
				{
					ret = addr;
					bestCount = res.count;
				}
				found = true;
				break;
			}
		}

		if (!found)
		{
			result res =
			{
				addr,
				1
			};
			results.push_back(res);
			if (bestCount == 0)
			{
				bestCount = 1;
				ret = addr;
			}
		}
	}

#ifdef DEVELOPER
	bool success = false;
	if (bestCount == 0)
		std::cout << "Patterns are broken. Hack won't work." << std::endl;
	else if (bestCount == 1 && totalCount > 1)
		std::cout << "Unable to determine majority of scan results, the hack most likely won't work." << std::endl;
	else if (bestCount < totalCount)
		std::cout << "Some patterns seem to be broken (Found " << bestCount << "/" << totalCount << ")." << std::endl;
	else
		success = true;

	if (!success)
	{
		int i = 0;
		for (auto &addr : rawResults)
		{
			if (!addr || addr != ret)
				std::cout << "Broken pattern: " << i << " (" << addr << ")" << std::endl;
			i++;
		}
	}
#endif

	if (ratio)
		*ratio = (float)bestCount / totalCount;

	return ret;
}

void *PatternScanner::FindPattern(const std::string &ModuleName, Pattern *patterns, float *ratio)
{
	return PatternScanner::FindPattern(GetModuleHandle(ModuleName.c_str()), GetModuleSize(ModuleName.c_str()), patterns, ratio);
}