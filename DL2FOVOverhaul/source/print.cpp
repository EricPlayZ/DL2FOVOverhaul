#include "print.h"
#include "core.h"
#include "memory.h"
#include "time_tools.h"
#include <iostream>

std::chrono::steady_clock::time_point refreshConsoleTimestamp{};
const int refreshConsoleIntervalMs = 250;

void UpdateConsole(const std::string& text = "", int x = 0, int y = 0) {
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	std::cout.flush();

	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
	if (!text.empty())
		std::cout << text;
}

int GetCurrentCursorPos() {
	const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(hConsole, &csbi))
		return csbi.dwCursorPosition.X;

	return -1; // Return -1 if unable to get the console screen buffer info
}

void PrintSpaces(const int& spaceCount) {
	std::string spaces = " ";
	for (int i = 0; i < spaceCount - 2; ++i) {
		spaces += " ";
	}

	printf(spaces.c_str());
}

void DrawConsoleOut() {
	if (since(refreshConsoleTimestamp).count() < refreshConsoleIntervalMs)
		return;

	const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		UpdateConsole();

		int consoleWidth, consoleHeight;
		COORD fovTextPos{};

		consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		std::string fovText = "[.] ExtraFOV: ";
		fovText.append((IsAddressValid(CVideoSettingsInstance) ? std::to_string(CVideoSettingsInstance->ExtraFOV) : "???") + "; FOV: ");
		fovText.append((IsAddressValid(CameraFPPDIInstance) ? std::to_string(CameraFPPDIInstance->FOV) : "???"));

		// Calculate center for first line in console
		fovTextPos.X = ((SHORT)consoleWidth - (SHORT)fovText.length()) / 2;

		PrintSpaces(static_cast<int>(fovTextPos.X) - GetCurrentCursorPos());
		SetConsoleCursorPosition(hConsole, fovTextPos);
		PrintCustom("[.] ExtraFOV: ", c_brightwhite);
		if (IsAddressValid(CVideoSettingsInstance))
			PrintCustom("%f", c_green, CVideoSettingsInstance->ExtraFOV);
		else
			PrintCustom("???", c_red);
		PrintCustom("; FOV: ", c_brightwhite);
		if (IsAddressValid(CameraFPPDIInstance))
			PrintCustom("%f", c_green, CameraFPPDIInstance->FOV);
		else
			PrintCustom("???", c_red);
		PrintSpaces(consoleWidth - GetCurrentCursorPos());

		printf("\n\n");

		PrintCustom("[.] CLobbySteam_loc addr: ", c_brightwhite);
		PrintCustom(IsAddressValid(CLobbySteamLoc) ? "%p" : "???", IsAddressValid(CLobbySteamLoc) ? c_green : c_red, CLobbySteamLoc);
		PrintSpaces(consoleWidth - GetCurrentCursorPos());
		printf("\n");
		PrintCustom("[.] CLobbySteam addr: ", c_brightwhite);
		PrintCustom(IsAddressValid(CLobbySteam_ptr) ? "%p" : "???", IsAddressValid(CLobbySteam_ptr) ? c_green : c_red, CLobbySteam_ptr);
		PrintSpaces(consoleWidth - GetCurrentCursorPos());
		printf("\n");
		PrintCustom("[.] CLobbySteam->CGame addr: ", c_brightwhite);
		PrintCustom(IsAddressValid(CGameInstance) ? "%p" : "???", IsAddressValid(CGameInstance) ? c_green : c_red, CGameInstance);
		PrintSpaces(consoleWidth - GetCurrentCursorPos());

		printf("\n\n");

		PrintCustom("[.] CGame->CVideoSettings addr: ", c_brightwhite);
		PrintCustom(IsAddressValid(CVideoSettingsInstance) ? "%p" : "???", IsAddressValid(CVideoSettingsInstance) != NULL ? c_green : c_red, CVideoSettingsInstance);
		PrintSpaces(consoleWidth - GetCurrentCursorPos());

		printf("\n\n");

		PrintCustom("[.] CGame->CLevel addr: ", c_brightwhite);
		PrintCustom(CLevel_ptr != NULL ? "%p" : "???", CLevel_ptr != NULL ? c_green : c_red, CLevel_ptr);
		PrintSpaces(consoleWidth - GetCurrentCursorPos());
		printf("\n");
		PrintCustom("[.] CGame->CLevel->CBaseCamera addr: ", c_brightwhite);
		PrintCustom(CBaseCamera_ptr != NULL ? "%p" : "???", CBaseCamera_ptr != NULL ? c_green : c_red, CBaseCamera_ptr);
		PrintSpaces(consoleWidth - GetCurrentCursorPos());
		printf("\n");
		PrintCustom("[.] CGame->CLevel->CBaseCamera->FreeCamera addr: ", c_brightwhite);
		PrintCustom(FreeCamera_ptr != NULL ? "%p" : "???", FreeCamera_ptr != NULL ? c_green : c_red, FreeCamera_ptr);
		PrintSpaces(consoleWidth - GetCurrentCursorPos());
		printf("\n");
		PrintCustom("[.] CGame->CLevel->CBaseCamera->FreeCamera->CoBaseCameraProxy addr: ", c_brightwhite);
		PrintCustom(CoBaseCameraProxy_ptr != NULL ? "%p" : "???", CoBaseCameraProxy_ptr != NULL ? c_green : c_red, CoBaseCameraProxy_ptr);
		PrintSpaces(consoleWidth - GetCurrentCursorPos());
		printf("\n");
		PrintCustom("[.] CGame->CLevel->CBaseCamera->FreeCamera->CoBaseCameraProxy->CameraFPPDI addr: ", c_brightwhite);
		PrintCustom(IsAddressValid(CameraFPPDIInstance) ? "%p" : "???", IsAddressValid(CameraFPPDIInstance) ? c_green : c_red, CameraFPPDIInstance);
		PrintSpaces(consoleWidth - GetCurrentCursorPos());

		printf("\n\n");

		PrintCustom("[.] CGame->CLevel2 addr: ", c_brightwhite);
		PrintCustom(CLevel2_ptr != NULL ? "%p" : "???", CLevel2_ptr != NULL ? c_green : c_red, CLevel2_ptr);
		PrintSpaces(consoleWidth - GetCurrentCursorPos());
		printf("\n");
		PrintCustom("[.] CGame->CLevel2->CGSObject addr: ", c_brightwhite);
		PrintCustom(CGSObject_ptr != NULL ? "%p" : "???", CGSObject_ptr != NULL ? c_green : c_red, CGSObject_ptr);
		PrintSpaces(consoleWidth - GetCurrentCursorPos());
		printf("\n");
		PrintCustom("[.] CGame->CLevel2->CGSObject->PlayerState addr: ", c_brightwhite);
		PrintCustom(PlayerState_ptr != NULL ? "%p" : "???", PlayerState_ptr != NULL ? c_green : c_red, PlayerState_ptr);
		PrintSpaces(consoleWidth - GetCurrentCursorPos());
		printf("\n");
		PrintCustom("[.] CGame->CLevel2->CGSObject->PlayerState->PlayerVariables addr: ", c_brightwhite);
		PrintCustom(IsAddressValid(PlayerVariablesInstance) ? "%p" : "???", IsAddressValid(PlayerVariablesInstance) ? c_green : c_red, PlayerVariablesInstance);
		PrintSpaces(consoleWidth - GetCurrentCursorPos());
		printf("\n");
		PrintSpaces(consoleWidth);
		printf("\n");
		PrintSpaces(consoleWidth);

		refreshConsoleTimestamp = std::chrono::steady_clock::now();
	}
}