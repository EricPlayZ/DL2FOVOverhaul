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

const std::unordered_map<std::string, int> virtualKeyCodes = {
	// Function keys
	{ "VK_F1", VK_F1 },
	{ "VK_F2", VK_F2 },
	{ "VK_F3", VK_F3 },
	{ "VK_F4", VK_F4 },
	{ "VK_F5", VK_F5 },
	{ "VK_F6", VK_F6 },
	{ "VK_F7", VK_F7 },
	{ "VK_F8", VK_F8 },
	{ "VK_F9", VK_F9 },
	{ "VK_F10", VK_F10 },
	{ "VK_F11", VK_F11 },
	{ "VK_F12", VK_F12 },
	{ "VK_F13", VK_F13 },
	{ "VK_F14", VK_F14 },
	{ "VK_F15", VK_F15 },
	{ "VK_F16", VK_F16 },
	{ "VK_F17", VK_F17 },
	{ "VK_F18", VK_F18 },
	{ "VK_F19", VK_F19 },
	{ "VK_F20", VK_F20 },
	{ "VK_F21", VK_F21 },
	{ "VK_F22", VK_F22 },
	{ "VK_F23", VK_F23 },
	{ "VK_F24", VK_F24 },

	// Number keys
	{ "VK_0", 0x30 },
	{ "VK_1", 0x31 },
	{ "VK_2", 0x32 },
	{ "VK_3", 0x33 },
	{ "VK_4", 0x34 },
	{ "VK_5", 0x35 },
	{ "VK_6", 0x36 },
	{ "VK_7", 0x37 },
	{ "VK_8", 0x38 },
	{ "VK_9", 0x39 },
	{ "0", 0x30 },
	{ "1", 0x31 },
	{ "2", 0x32 },
	{ "3", 0x33 },
	{ "4", 0x34 },
	{ "5", 0x35 },
	{ "6", 0x36 },
	{ "7", 0x37 },
	{ "8", 0x38 },
	{ "9", 0x39 },

	// Alphabetic keys
	{ "VK_A", 0x41 },
	{ "VK_B", 0x42 },
	{ "VK_C", 0x43 },
	{ "VK_D", 0x44 },
	{ "VK_E", 0x45 },
	{ "VK_F", 0x46 },
	{ "VK_G", 0x47 },
	{ "VK_H", 0x48 },
	{ "VK_I", 0x49 },
	{ "VK_J", 0x4A },
	{ "VK_K", 0x4B },
	{ "VK_L", 0x4C },
	{ "VK_M", 0x4D },
	{ "VK_N", 0x4E },
	{ "VK_O", 0x4F },
	{ "VK_P", 0x50 },
	{ "VK_Q", 0x51 },
	{ "VK_R", 0x52 },
	{ "VK_S", 0x53 },
	{ "VK_T", 0x54 },
	{ "VK_U", 0x55 },
	{ "VK_V", 0x56 },
	{ "VK_W", 0x57 },
	{ "VK_X", 0x58 },
	{ "VK_Y", 0x59 },
	{ "VK_Z", 0x5A },
	{ "A", 0x41 },
	{ "B", 0x42 },
	{ "C", 0x43 },
	{ "D", 0x44 },
	{ "E", 0x45 },
	{ "F", 0x46 },
	{ "G", 0x47 },
	{ "H", 0x48 },
	{ "I", 0x49 },
	{ "J", 0x4A },
	{ "K", 0x4B },
	{ "L", 0x4C },
	{ "M", 0x4D },
	{ "N", 0x4E },
	{ "O", 0x4F },
	{ "P", 0x50 },
	{ "Q", 0x51 },
	{ "R", 0x52 },
	{ "S", 0x53 },
	{ "T", 0x54 },
	{ "U", 0x55 },
	{ "V", 0x56 },
	{ "W", 0x57 },
	{ "X", 0x58 },
	{ "Y", 0x59 },
	{ "Z", 0x5A },

	// Special keys
	{"VK_BACK", VK_BACK },
	{"VK_TAB", VK_TAB },
	{"VK_CLEAR", VK_CLEAR },
	{"VK_RETURN", VK_RETURN },
	{"VK_SHIFT", VK_SHIFT },
	{"VK_CONTROL", VK_CONTROL },
	{"VK_MENU", VK_MENU },
	{"VK_PAUSE", VK_PAUSE },
	{"VK_CAPITAL", VK_CAPITAL },
	{"VK_ESCAPE", VK_ESCAPE },
	{"VK_SPACE", VK_SPACE },
	{"VK_PRIOR", VK_PRIOR },
	{"VK_NEXT", VK_NEXT },
	{"VK_END", VK_END },
	{"VK_HOME", VK_HOME },
	{"VK_LEFT", VK_LEFT },
	{"VK_UP", VK_UP },
	{"VK_RIGHT", VK_RIGHT },
	{"VK_DOWN", VK_DOWN },
	{"VK_SELECT", VK_SELECT },
	{"VK_PRINT", VK_PRINT },
	{"VK_EXECUTE", VK_EXECUTE },
	{"VK_SNAPSHOT", VK_SNAPSHOT },
	{"VK_INSERT", VK_INSERT },
	{"VK_DELETE", VK_DELETE },
	{"VK_HELP", VK_HELP },

	// Numpad keys
	{ "VK_NUMPAD0", VK_NUMPAD0 },
	{ "VK_NUMPAD1", VK_NUMPAD1 },
	{ "VK_NUMPAD2", VK_NUMPAD2 },
	{ "VK_NUMPAD3", VK_NUMPAD3 },
	{ "VK_NUMPAD4", VK_NUMPAD4 },
	{ "VK_NUMPAD5", VK_NUMPAD5 },
	{ "VK_NUMPAD6", VK_NUMPAD6 },
	{ "VK_NUMPAD7", VK_NUMPAD7 },
	{ "VK_NUMPAD8", VK_NUMPAD8 },
	{ "VK_NUMPAD9", VK_NUMPAD9 },
	{ "VK_MULTIPLY", VK_MULTIPLY },
	{ "VK_ADD", VK_ADD },
	{ "VK_SEPARATOR", VK_SEPARATOR },
	{ "VK_SUBTRACT", VK_SUBTRACT },
	{ "VK_DECIMAL", VK_DECIMAL },
	{ "VK_DIVIDE", VK_DIVIDE },

	// Modifier keys
	{ "VK_SHIFT", VK_SHIFT },
	{ "VK_LSHIFT", VK_LSHIFT },
	{ "VK_RSHIFT", VK_RSHIFT },
	{ "VK_CONTROL", VK_CONTROL },
	{ "VK_LCONTROL", VK_LCONTROL },
	{ "VK_RCONTROL", VK_RCONTROL },
	{ "VK_MENU", VK_MENU },
	{ "VK_LMENU", VK_LMENU },
	{ "VK_RMENU", VK_RMENU },

	// Browser keys
	{ "VK_BROWSER_BACK", VK_BROWSER_BACK },
	{ "VK_BROWSER_FORWARD", VK_BROWSER_FORWARD },
	{ "VK_BROWSER_REFRESH", VK_BROWSER_REFRESH },
	{ "VK_BROWSER_STOP", VK_BROWSER_STOP },
	{ "VK_BROWSER_SEARCH", VK_BROWSER_SEARCH },
	{ "VK_BROWSER_FAVORITES", VK_BROWSER_FAVORITES },
	{ "VK_BROWSER_HOME", VK_BROWSER_HOME },

	// Volume keys
	{ "VK_VOLUME_MUTE", VK_VOLUME_MUTE },
	{ "VK_VOLUME_DOWN", VK_VOLUME_DOWN },
	{ "VK_VOLUME_UP", VK_VOLUME_UP },

	// Media keys
	{ "VK_MEDIA_NEXT_TRACK", VK_MEDIA_NEXT_TRACK },
	{ "VK_MEDIA_PREV_TRACK", VK_MEDIA_PREV_TRACK },
	{ "VK_MEDIA_STOP", VK_MEDIA_STOP },
	{ "VK_MEDIA_PLAY_PAUSE", VK_MEDIA_PLAY_PAUSE },

	// Application keys
	{ "VK_LAUNCH_MAIL", VK_LAUNCH_MAIL },
	{ "VK_LAUNCH_MEDIA_SELECT", VK_LAUNCH_MEDIA_SELECT },
	{ "VK_LAUNCH_APP1", VK_LAUNCH_APP1 },
	{ "VK_LAUNCH_APP2", VK_LAUNCH_APP2 },

	// Other keys
	{ "VK_OEM_1", VK_OEM_1 },
	{ "VK_OEM_PLUS", VK_OEM_PLUS },
	{ "VK_OEM_COMMA", VK_OEM_COMMA },
	{ "VK_OEM_MINUS", VK_OEM_MINUS },
	{ "VK_OEM_PERIOD", VK_OEM_PERIOD },
	{ "VK_OEM_2", VK_OEM_2 },
	{ "VK_OEM_3", VK_OEM_3 },
	{ "VK_OEM_4", VK_OEM_4 },
	{ "VK_OEM_5", VK_OEM_5 },
	{ "VK_OEM_6", VK_OEM_6 },
	{ "VK_OEM_7", VK_OEM_7 },
	{ "VK_OEM_8", VK_OEM_8 },
	{ "VK_OEM_102", VK_OEM_102 },
	{ "VK_PROCESSKEY", VK_PROCESSKEY },
	{ "VK_PACKET", VK_PACKET },
	{ "VK_ATTN", VK_ATTN },
	{ "VK_CRSEL", VK_CRSEL },
	{ "VK_EXSEL", VK_EXSEL },
	{ "VK_EREOF", VK_EREOF },
	{ "VK_PLAY", VK_PLAY },
	{ "VK_ZOOM", VK_ZOOM },
	{ "VK_NONAME", VK_NONAME },
	{ "VK_PA1", VK_PA1 },
	{ "VK_OEM_CLEAR", VK_OEM_CLEAR }
};

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

CLobbySteam_loc* CLobbySteamLoc = NULL;
CGame* CGameInstance = NULL;
PlayerVariables* PlayerVariablesInstance = NULL;
CVideoSettings* CVideoSettingsInstance = NULL;
CameraFPPDI* CameraFPPDIInstance = NULL;

// Pointers to classes
LPVOID CLobbySteam_ptr = NULL;
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
	configReader.InsertEntry("Keybinds", "ModifierKey", VK_CONTROL);
	configReader.InsertEntry("Keybinds", "FOVIncrease", VK_ADD);
	configReader.InsertEntry("Keybinds", "FOVDecrease", VK_SUBTRACT);
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
static void ResetPointersCLobbySteam(int level = 0) {
	switch (level) {
	case 0: {
		CLobbySteam_ptr = NULL;
		CGameInstance = NULL;
		CVideoSettingsInstance = NULL;
		break;
	}
	case 1: {
		CGameInstance = NULL;
		CVideoSettingsInstance = NULL;
		break;
	}
	case 2: {
		CVideoSettingsInstance = NULL;
	}
	}
}
static void ResetPointersCLevel(int level = 0) {
	switch (level) {
	case 0: {
		CLevel_ptr = NULL;
		CBaseCamera_ptr = NULL;
		FreeCamera_ptr = NULL;
		CoBaseCameraProxy_ptr = NULL;
		CameraFPPDIInstance = NULL;
		break;
	}
	case 1: {
		CBaseCamera_ptr = NULL;
		FreeCamera_ptr = NULL;
		CoBaseCameraProxy_ptr = NULL;
		CameraFPPDIInstance = NULL;
		break;
	}
	case 2: {
		FreeCamera_ptr = NULL;
		CoBaseCameraProxy_ptr = NULL;
		CameraFPPDIInstance = NULL;
		break;
	}
	case 3: {
		CoBaseCameraProxy_ptr = NULL;
		CameraFPPDIInstance = NULL;
		break;
	}
	case 4: {
		CameraFPPDIInstance = NULL;
		break;
	}
	}
}
static void ResetPointersCLevel2(int level = 0) {
	switch (level) {
	case 0: {
		CLevel2_ptr = NULL;
		CGSObject_ptr = NULL;
		PlayerState_ptr = NULL;
		PlayerVariablesInstance = NULL;
		break;
	}
	case 1: {
		CGSObject_ptr = NULL;
		PlayerState_ptr = NULL;
		PlayerVariablesInstance = NULL;
		break;
	}
	case 2: {
		PlayerState_ptr = NULL;
		PlayerVariablesInstance = NULL;
		break;
	}
	case 3: {
		PlayerVariablesInstance = NULL;
		break;
	}
	}
}
static void UpdatePointers() {
	if (!IsAddressValid(CLobbySteamLoc))
		return;
	if (!IsAddressValid(CLobbySteamLoc->CLobbySteam_ptr)) {
		ResetPointersCLobbySteam();
		ResetPointersCLevel();
		ResetPointersCLevel2();
		return;
	}
	CLobbySteam_ptr = CLobbySteamLoc->CLobbySteam_ptr;
	if (!IsAddressValid(CLobbySteamLoc->CLobbySteam_ptr->CGame_ptr)) {
		ResetPointersCLobbySteam(1);
		ResetPointersCLevel();
		ResetPointersCLevel2();
		return;
	}
	CGameInstance = CLobbySteamLoc->CLobbySteam_ptr->CGame_ptr;

	if (IsAddressValid(CGameInstance->CVideoSettings_ptr))
		CVideoSettingsInstance = CGameInstance->CVideoSettings_ptr;
	else
		ResetPointersCLobbySteam(2);

	if (IsAddressValid(CGameInstance->CLevel_ptr)) {
		CLevel_ptr = CGameInstance->CLevel_ptr;
		if (IsAddressValid(CGameInstance->CLevel_ptr->CBaseCamera_ptr)) {
			CBaseCamera_ptr = CGameInstance->CLevel_ptr->CBaseCamera_ptr;
			if (IsAddressValid(CGameInstance->CLevel_ptr->CBaseCamera_ptr->FreeCamera_ptr)) {
				FreeCamera_ptr = CGameInstance->CLevel_ptr->CBaseCamera_ptr->FreeCamera_ptr;
				if (IsAddressValid(CGameInstance->CLevel_ptr->CBaseCamera_ptr->FreeCamera_ptr->CoBaseCameraProxy_ptr)) {
					CoBaseCameraProxy_ptr = CGameInstance->CLevel_ptr->CBaseCamera_ptr->FreeCamera_ptr->CoBaseCameraProxy_ptr;
					if (IsAddressValid(CGameInstance->CLevel_ptr->CBaseCamera_ptr->FreeCamera_ptr->CoBaseCameraProxy_ptr->CameraFPPDI_ptr)) {
						CameraFPPDIInstance = CGameInstance->CLevel_ptr->CBaseCamera_ptr->FreeCamera_ptr->CoBaseCameraProxy_ptr->CameraFPPDI_ptr;
					} else
						ResetPointersCLevel(4);
				} else
					ResetPointersCLevel(3);
			} else
				ResetPointersCLevel(2);
		} else
			ResetPointersCLevel(1);
	} else
		ResetPointersCLevel();

	if (IsAddressValid(CGameInstance->CLevel2_ptr)) {
		CLevel2_ptr = CGameInstance->CLevel2_ptr;
		if (IsAddressValid(CGameInstance->CLevel2_ptr->CGSObject_ptr)) {
			CGSObject_ptr = CGameInstance->CLevel2_ptr->CGSObject_ptr;
			if (IsAddressValid(CGameInstance->CLevel2_ptr->CGSObject_ptr->PlayerState_ptr)) {
				PlayerState_ptr = CGameInstance->CLevel2_ptr->CGSObject_ptr->PlayerState_ptr;
				if (IsAddressValid(CGameInstance->CLevel2_ptr->CGSObject_ptr->PlayerState_ptr->PlayerVariables_ptr)) {
					PlayerVariablesInstance = CGameInstance->CLevel2_ptr->CGSObject_ptr->PlayerState_ptr->PlayerVariables_ptr;
				} else
					ResetPointersCLevel2(3);
			} else
				ResetPointersCLevel2(2);
		} else
			ResetPointersCLevel2(1);
	} else
		ResetPointersCLevel2();
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
    bool searchingForAddr = false;
	while (true) {
		Sleep(10); // Sleep for a short amount of time to reduce possible CPU performance impact

		UpdatePointers();

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
		if (!IsAddressValid(engineModuleInfo.lpBaseOfDll)) {
			engineModuleInfo = GetModuleInfo("engine_x64_rwdi.dll");
			if (!IsAddressValid(engineModuleInfo.lpBaseOfDll))
				continue;
		}

		if (!IsAddressValid(CLobbySteamLoc)) {
			CLobbySteamLoc = reinterpret_cast<CLobbySteam_loc*>((DWORD64)engineModuleInfo.lpBaseOfDll + 0x2307DF8);
			if (!IsAddressValid(CLobbySteamLoc))
				continue;
		}
		if (!IsAddressValid(CGameInstance))
			continue;

		if (IsAddressValid(PlayerVariablesInstance)) {
			// Always set CameraDefaultFOVReduction to the value specified by the config
			if (PlayerVariablesInstance->CameraDefaultFOVReduction != -fovSafezoneReductionAmount)
				PlayerVariablesInstance->CameraDefaultFOVReduction = -fovSafezoneReductionAmount;
		}

		if (!IsAddressValid(CVideoSettingsInstance))
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