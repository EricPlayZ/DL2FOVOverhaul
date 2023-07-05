#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <thread>
#include <mutex>
#include <io.h>
#include "memory.h"
#include "ini.h"
#include "game_classes.h"
#include "print.h"
#include "time_tools.h"
#include "SigScan\StaticOffsets.h"

static std::ostringstream GetTimestamp() {
	time_t timeInstance = time(0);
	tm timestamp{};
	localtime_s(&timestamp, &timeInstance);
	std::ostringstream oss{};
	oss << "[" << std::setw(2) << std::setfill('0') << timestamp.tm_hour << "h:"
		<< std::setw(2) << std::setfill('0') << timestamp.tm_min << "m:"
		<< std::setw(2) << std::setfill('0') << timestamp.tm_sec << "s] ";

	return oss;
}

CLobbySteam* CLobbySteamInstance = NULL;
CGame* CGameInstance = NULL;
PlayerVariables* PlayerVariablesInstance = NULL;
CVideoSettings* CVideoSettingsInstance = NULL;
CameraFPPDI* CameraFPPDIInstance = NULL;

// Pointers to classes
LPVOID CLobbySteamLoc = NULL;
LPVOID CLevel_ptr, CBaseCamera_ptr, FreeCamera_ptr, CoBaseCameraProxy_ptr = NULL;
LPVOID CLevel2_ptr, CGSObject_ptr, PlayerState_ptr = NULL;

// Config stuff
const char* configFileName = "FOVOverhaul.ini";
std::string configStatus{};
std::string configError{};
inih::INIReader configReader{};
int modifierKey = VK_CONTROL;
int fovIncreaseKey = VK_ADD;
int fovDecreaseKey = VK_SUBTRACT;
float fovSafezoneReductionAmount = 0.0f;

static void LoadDefaultConfig(inih::INIReader& configReader) {
	configReader = inih::INIReader();
	configReader.InsertEntry("Keybinds", "ModifierKey", "VK_CONTROL");
	configReader.InsertEntry("Keybinds", "FOVIncrease", "VK_ADD");
	configReader.InsertEntry("Keybinds", "FOVDecrease", "VK_SUBTRACT");
	configReader.InsertEntry("Options", "FOVSafezoneReductionAmount", 0.0f);

	modifierKey = VK_CONTROL;
	fovIncreaseKey = VK_ADD;
	fovDecreaseKey = VK_SUBTRACT;
	fovSafezoneReductionAmount = 0.0f;
}
static void LoadAndWriteDefaultConfig(inih::INIReader& configReader) {
	LoadDefaultConfig(configReader);
	try {
		inih::INIWriter writer{};
		writer.write(configFileName, configReader);
	} catch (const std::runtime_error& e) {
		std::ostringstream oss = GetTimestamp();
		oss << "Error writing file " << configFileName << ": " << e.what();
		configError = oss.str();
	}
}
static const bool ConfigExists() {
	return std::filesystem::exists(configFileName);
}
static void CreateConfig(inih::INIReader& configReader) {
	std::ostringstream oss = GetTimestamp();
	oss << configFileName << " does not exist (will create now); using default config values";
	configStatus = oss.str();

	LoadAndWriteDefaultConfig(configReader);
}
static void ReadConfig(inih::INIReader& configReader, const bool& configUpdate = false) {
	try {
		configReader = inih::INIReader(configFileName);

		modifierKey = configReader.Get<int>("Keybinds", "ModifierKey", VK_CONTROL);
		fovIncreaseKey = configReader.Get<int>("Keybinds", "FOVIncrease", VK_ADD);
		fovDecreaseKey = configReader.Get<int>("Keybinds", "FOVDecrease", VK_SUBTRACT);
		fovSafezoneReductionAmount = configReader.Get<float>("Options", "FOVSafezoneReductionAmount", 10.0f); // Keep original game value if value doesn't exist

		std::ostringstream oss = GetTimestamp();
		oss << (configUpdate ? "Successfully read updated config!" : "Successfully read config!");
		configStatus = oss.str();
	} catch (const std::runtime_error& e) {
		std::ostringstream oss = GetTimestamp();
		oss << "Error writing file " << configFileName << "; using default config values: " << e.what();
		configError = oss.str();

		LoadDefaultConfig(configReader);
	}
}

// Pointer stuff
static void UpdatePointers() {
	if (!IsValidPtr(CLobbySteamInstance))
		return;
	if (!IsValidPtr(CLobbySteamInstance->CGame_ptr))
		return;
	CGameInstance = CLobbySteamInstance->CGame_ptr;

	if (IsValidPtr(CGameInstance->CVideoSettings_ptr))
		CVideoSettingsInstance = CGameInstance->CVideoSettings_ptr;

	if (IsValidPtr(CGameInstance->CLevel_ptr)) {
		CLevel_ptr = CGameInstance->CLevel_ptr;
		if (IsValidPtr(CGameInstance->CLevel_ptr->CBaseCamera_ptr)) {
			CBaseCamera_ptr = CGameInstance->CLevel_ptr->CBaseCamera_ptr;
			if (IsValidPtr(CGameInstance->CLevel_ptr->CBaseCamera_ptr->FreeCamera_ptr)) {
				FreeCamera_ptr = CGameInstance->CLevel_ptr->CBaseCamera_ptr->FreeCamera_ptr;
				if (IsValidPtr(CGameInstance->CLevel_ptr->CBaseCamera_ptr->FreeCamera_ptr->CoBaseCameraProxy_ptr)) {
					CoBaseCameraProxy_ptr = CGameInstance->CLevel_ptr->CBaseCamera_ptr->FreeCamera_ptr->CoBaseCameraProxy_ptr;
					if (IsValidPtr(CGameInstance->CLevel_ptr->CBaseCamera_ptr->FreeCamera_ptr->CoBaseCameraProxy_ptr->CameraFPPDI_ptr))
						CameraFPPDIInstance = CGameInstance->CLevel_ptr->CBaseCamera_ptr->FreeCamera_ptr->CoBaseCameraProxy_ptr->CameraFPPDI_ptr;
				}
			}
		}
	}

	if (IsValidPtr(CGameInstance->CLevel2_ptr)) {
		CLevel2_ptr = CGameInstance->CLevel2_ptr;
		if (IsValidPtr(CGameInstance->CLevel2_ptr->CGSObject_ptr)) {
			CGSObject_ptr = CGameInstance->CLevel2_ptr->CGSObject_ptr;
			if (IsValidPtr(CGameInstance->CLevel2_ptr->CGSObject_ptr->PlayerState_ptr)) {
				PlayerState_ptr = CGameInstance->CLevel2_ptr->CGSObject_ptr->PlayerState_ptr;
				if (IsValidPtr(CGameInstance->CLevel2_ptr->CGSObject_ptr->PlayerState_ptr->PlayerVariables_ptr))
					PlayerVariablesInstance = CGameInstance->CLevel2_ptr->CGSObject_ptr->PlayerState_ptr->PlayerVariables_ptr;
			}
		}
	}
}

// Console stuff
HANDLE originalStdoutHandle = NULL;
HANDLE nullStdoutHandle = NULL;
FILE* consoleDisabledOutStream = NULL;
FILE* consoleOutConStream = NULL;
std::mutex consoleMutex;

static void DisableConsoleQuickEdit() {
	DWORD prev_mode = 0;
	const HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hInput, &prev_mode);
	SetConsoleMode(hInput, prev_mode & ENABLE_EXTENDED_FLAGS);
}
static void SetConsoleWindowSize(const int& width, const int& height) {
	const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);

	const SMALL_RECT windowSize = { 0, 0, (SHORT)width - 1, (SHORT)height - 1 };
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

	const COORD bufferSize = { (SHORT)width, (SHORT)height };
	SetConsoleScreenBufferSize(hConsole, bufferSize);
}
static void LockConsoleWindowSize(const int& width, const int& height) {
	SetConsoleWindowSize(width, height);

	HWND hwndConsole = GetConsoleWindow();
	if (hwndConsole != NULL) {
		LONG style = GetWindowLong(hwndConsole, GWL_STYLE);
		style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
		SetWindowLong(hwndConsole, GWL_STYLE, style);
		SetWindowPos(hwndConsole, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
}
void DisableStdout() {
	if (consoleDisabledOutStream == NULL) {
		if (consoleOutConStream != NULL) {
			fclose(stdout);
			fclose(stderr);
			consoleOutConStream = NULL;
		}

		if (originalStdoutHandle == NULL) {
			originalStdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			nullStdoutHandle = CreateFile("NUL", GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

			SetStdHandle(STD_OUTPUT_HANDLE, nullStdoutHandle);
		}

		freopen_s(&consoleDisabledOutStream, "NUL", "w", stdout);
		freopen_s(&consoleDisabledOutStream, "NUL", "w", stderr);
	}
}
void RestoreStdout() {
	if (consoleOutConStream == NULL) {
		if (consoleDisabledOutStream != NULL) {
			fclose(stdout);
			fclose(stderr);
			consoleDisabledOutStream = NULL;
		}

		if (originalStdoutHandle != NULL) {
			SetStdHandle(STD_OUTPUT_HANDLE, originalStdoutHandle);
			CloseHandle(nullStdoutHandle);

			originalStdoutHandle = NULL;
			nullStdoutHandle = NULL;
		}

		freopen_s(&consoleOutConStream, "CONOUT$", "w", stdout);
		freopen_s(&consoleOutConStream, "CONOUT$", "w", stderr);
	}
}

static void ConsoleThread() {
	while (true) {
		std::lock_guard<std::mutex> lock(consoleMutex); // Lock the console output to this thread using the consoleMutex

		RestoreStdout();
		DrawConsoleOut();
		DisableStdout();

		Sleep(refreshConsoleIntervalMs);
	}
}

DWORD64 WINAPI MainThread(HMODULE hModule) {
    // Create a console window
    AllocConsole();
	DisableStdout();

	DisableConsoleQuickEdit();
	LockConsoleWindowSize(100, 25);

	ConfigExists() ? ReadConfig(configReader) : CreateConfig(configReader);

	// Storing write time of config file for checking changes to config file
	std::filesystem::file_time_type configPreviousWriteTime = std::filesystem::last_write_time(configFileName);
	std::filesystem::file_time_type configLastWriteTime = configPreviousWriteTime;

    // Module stuff
	MODULEINFO engineModuleInfo{};

    // Key press delay for holding down the key without the value going vroom
    const int keyPressSleepMs = 120;
    std::chrono::steady_clock::time_point timeStartAfterKeyPress{};

	bool fovIncreasePressed = false;
	bool fovDecreasePressed = false;
	bool canPress = false;

	// Create console output thread
	std::thread consoleThread(ConsoleThread);

    // Main loop
	while (true) {
		Sleep(10); // Sleep for a short amount of time to reduce possible CPU performance impact

		UpdatePointers();

		// Create config if it doesn't exist
		if (!ConfigExists()) {
			CreateConfig(configReader);
			Sleep(200);
			configPreviousWriteTime = std::filesystem::last_write_time(configFileName);
		}

		// Check for config changes
		configLastWriteTime = std::filesystem::last_write_time(configFileName);
		if (configLastWriteTime != configPreviousWriteTime) {
			configPreviousWriteTime = configLastWriteTime;

			Sleep(200);
			ReadConfig(configReader, true);
		}

		// Search for the module that contains all the necessary pointers
		if (!IsValidPtr(engineModuleInfo.lpBaseOfDll)) {
			engineModuleInfo = GetModuleInfo("engine_x64_rwdi.dll");
			if (!IsValidPtr(engineModuleInfo.lpBaseOfDll))
				continue;
		}
		
		if (!IsValidPtr(CLobbySteamInstance)) {
			CLobbySteamLoc = reinterpret_cast<LPVOID>(StaticOffsets::Get_CLobbySteamOffset());
			CLobbySteamInstance = reinterpret_cast<CLobbySteam*>(*reinterpret_cast<LPVOID*>(StaticOffsets::Get_CLobbySteamOffset()));
			if (!IsValidPtr(CLobbySteamInstance))
				continue;
		}
		if (!IsValidPtr(CGameInstance))
			continue;

		if (IsValidPtr(PlayerVariablesInstance)) {
			// Always set CameraDefaultFOVReduction to the value specified by the config
			if (PlayerVariablesInstance->CameraDefaultFOVReduction != -fovSafezoneReductionAmount)
				PlayerVariablesInstance->CameraDefaultFOVReduction = -fovSafezoneReductionAmount;
		}

		if (!IsValidPtr(CVideoSettingsInstance))
			continue;

		// Get key press states and check if user can press (cooldown so when you hold the key it doesnt go vroom)
		fovIncreasePressed = (GetAsyncKeyState(modifierKey) & GetAsyncKeyState(fovIncreaseKey) & 0x8000) || (fovIncreaseKey == VK_ADD && GetAsyncKeyState(modifierKey) & GetAsyncKeyState(VK_OEM_PLUS) & 0x8000) || (fovIncreaseKey == VK_OEM_PLUS && GetAsyncKeyState(modifierKey) & GetAsyncKeyState(VK_ADD) & 0x8000);
		fovDecreasePressed = GetAsyncKeyState(modifierKey) & GetAsyncKeyState(fovDecreaseKey) & 0x8000 || (fovDecreaseKey == VK_SUBTRACT && GetAsyncKeyState(modifierKey) & GetAsyncKeyState(VK_OEM_MINUS) & 0x8000) || (fovDecreaseKey == VK_OEM_MINUS && GetAsyncKeyState(modifierKey) & GetAsyncKeyState(VK_SUBTRACT) & 0x8000);
		canPress = since(timeStartAfterKeyPress).count() > keyPressSleepMs;

		// Increase or decrease FOV
		if ((fovIncreasePressed || fovDecreasePressed) && canPress) {
			if (fovIncreasePressed)
				CVideoSettingsInstance->ExtraFOV++;
			else if (fovDecreasePressed)
				CVideoSettingsInstance->ExtraFOV--;

			timeStartAfterKeyPress = std::chrono::steady_clock::now();
		}
	}

	// Close allocated console
	consoleThread.join();
	RestoreStdout();
    FreeConsole();

    return TRUE;
}