#include <iostream>
#include "print.h"
#include "core.h"
#include "memory.h"

const int refreshConsoleIntervalMs = 250;

void RefreshConsole(const std::string& text = "", const int& x = 0, const int& y = 0) {
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	std::cout.flush();

	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
	if (!text.empty())
		std::cout << text;
}
COORD GetCurrentCursorPos() {
	const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(hConsole, &csbi))
		return csbi.dwCursorPosition;

	return COORD{ 0, 0 };
}
int GetCurrentCursorPosX() {
	const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(hConsole, &csbi))
		return csbi.dwCursorPosition.X;

	return -1;
}
int GetCurrentCursorPosY() {
	const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(hConsole, &csbi))
		return csbi.dwCursorPosition.Y;

	return -1;
}

void PrintSpaces(const int& spaceCount) {
	std::string spaces = " ";
	for (int i = 0; i < spaceCount - 2; ++i) {
		spaces += " ";
	}

	printf(spaces.c_str());
}

void DrawConsoleOut() {
	const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		RefreshConsole();

		// Calculate center in console for lines
		std::string fovText = "[.] ExtraFOV: ";
		fovText.append((IsValidPtr(CVideoSettingsInstance) ? std::to_string(CVideoSettingsInstance->ExtraFOV) : "???") + "; FOV: ");
		fovText.append((IsValidPtr(CameraFPPDIInstance) ? std::to_string(CameraFPPDIInstance->FOV) : "???"));
		const COORD fovTextPos = { ((SHORT)consoleWidth - (SHORT)fovText.length()) / 2 , 0 };

		std::string fovText2 = "[.] Safezone FOV Reduction: ";
		fovText2.append((IsValidPtr(PlayerVariablesInstance) ? std::to_string(-PlayerVariablesInstance->CameraDefaultFOVReduction) : "???"));
		const COORD fovText2Pos = { ((SHORT)consoleWidth - (SHORT)fovText2.length()) / 2 , 0 };

		PrintSpaces((int)fovTextPos.X - GetCurrentCursorPosX());
		PrintCustom("[.] ExtraFOV: ", c_brightwhite);
		if (IsValidPtr(CVideoSettingsInstance))
			PrintCustom("%f", c_green, CVideoSettingsInstance->ExtraFOV);
		else
			PrintCustom("???", c_red);
		PrintCustom("; FOV: ", c_brightwhite);
		if (IsValidPtr(CameraFPPDIInstance))
			PrintCustom("%f", c_green, CameraFPPDIInstance->FOV);
		else
			PrintCustom("???", c_red);
		PrintSpaces(consoleWidth - GetCurrentCursorPosX());
		printf("\n");
		PrintSpaces((int)fovText2Pos.X - GetCurrentCursorPosX());
		PrintCustom("[.] Safezone FOV Reduction: ", c_brightwhite);
		if (IsValidPtr(PlayerVariablesInstance))
			PrintCustom("%f", c_green, -PlayerVariablesInstance->CameraDefaultFOVReduction);
		else
			PrintCustom("???", c_red);
		PrintSpaces(consoleWidth - GetCurrentCursorPosX());

		printf("\n");

		if (configStatus.empty())
			PrintSpaces(consoleWidth);
		else {
			const COORD configStatusTextPos = { ((SHORT)consoleWidth - (SHORT)configStatus.length()) / 2 , 0 };
			PrintSpaces((int)configStatusTextPos.X - GetCurrentCursorPosX());
			PrintCustom(configStatus, c_green);
			PrintSpaces(consoleWidth - GetCurrentCursorPosX());
		}
		
		printf("\n");

		if (configError.empty())
			PrintSpaces(consoleWidth);
		else {
			const COORD configErrorTextPos = { ((SHORT)consoleWidth - (SHORT)configError.length()) / 2 , 0 };
			PrintSpaces((int)configErrorTextPos.X - GetCurrentCursorPosX());
			PrintCustom(configError, c_green);
			PrintSpaces(consoleWidth - GetCurrentCursorPosX());
		}

		printf("\n");
		PrintSpaces(consoleWidth);
		printf("\n");

		PrintCustom("[.] CLobbySteam_loc addr: ", c_brightwhite);
		PrintCustom(IsValidPtr(CLobbySteamLoc) ? "%p" : "???", IsValidPtr(CLobbySteamLoc) ? c_green : c_red, CLobbySteamLoc);
		PrintSpaces(consoleWidth - GetCurrentCursorPosX());
		printf("\n");
		PrintCustom("[.] CLobbySteam addr: ", c_brightwhite);
		PrintCustom(IsValidPtr(CLobbySteamInstance) ? "%p" : "???", IsValidPtr(CLobbySteamInstance) ? c_green : c_red, CLobbySteamInstance);
		PrintSpaces(consoleWidth - GetCurrentCursorPosX());
		printf("\n");
		PrintCustom("[.] CLobbySteam->CGame addr: ", c_brightwhite);
		PrintCustom(IsValidPtr(CGameInstance) ? "%p" : "???", IsValidPtr(CGameInstance) ? c_green : c_red, CGameInstance);
		PrintSpaces(consoleWidth - GetCurrentCursorPosX());

		printf("\n");
		PrintSpaces(consoleWidth);
		printf("\n");

		PrintCustom("[.] CGame->CVideoSettings addr: ", c_brightwhite);
		PrintCustom(IsValidPtr(CVideoSettingsInstance) ? "%p" : "???", IsValidPtr(CVideoSettingsInstance) != NULL ? c_green : c_red, CVideoSettingsInstance);
		PrintSpaces(consoleWidth - GetCurrentCursorPosX());

		printf("\n");
		PrintSpaces(consoleWidth);
		printf("\n");

		PrintCustom("[.] CGame->CLevel addr: ", c_brightwhite);
		PrintCustom(CLevel_ptr != NULL ? "%p" : "???", CLevel_ptr != NULL ? c_green : c_red, CLevel_ptr);
		PrintSpaces(consoleWidth - GetCurrentCursorPosX());
		printf("\n");
		PrintCustom("[.] CGame->CLevel->CBaseCamera addr: ", c_brightwhite);
		PrintCustom(CBaseCamera_ptr != NULL ? "%p" : "???", CBaseCamera_ptr != NULL ? c_green : c_red, CBaseCamera_ptr);
		PrintSpaces(consoleWidth - GetCurrentCursorPosX());
		printf("\n");
		PrintCustom("[.] CGame->CLevel->CBaseCamera->FreeCamera addr: ", c_brightwhite);
		PrintCustom(FreeCamera_ptr != NULL ? "%p" : "???", FreeCamera_ptr != NULL ? c_green : c_red, FreeCamera_ptr);
		PrintSpaces(consoleWidth - GetCurrentCursorPosX());
		printf("\n");
		PrintCustom("[.] CGame->CLevel->CBaseCamera->FreeCamera->CoBaseCameraProxy addr: ", c_brightwhite);
		PrintCustom(CoBaseCameraProxy_ptr != NULL ? "%p" : "???", CoBaseCameraProxy_ptr != NULL ? c_green : c_red, CoBaseCameraProxy_ptr);
		PrintSpaces(consoleWidth - GetCurrentCursorPosX());
		printf("\n");
		PrintCustom("[.] CGame->CLevel->CBaseCamera->FreeCamera->CoBaseCameraProxy->CameraFPPDI addr: ", c_brightwhite);
		PrintCustom(IsValidPtr(CameraFPPDIInstance) ? "%p" : "???", IsValidPtr(CameraFPPDIInstance) ? c_green : c_red, CameraFPPDIInstance);
		PrintSpaces(consoleWidth - GetCurrentCursorPosX());

		printf("\n");
		PrintSpaces(consoleWidth);
		printf("\n");

		PrintCustom("[.] CGame->CLevel2 addr: ", c_brightwhite);
		PrintCustom(CLevel2_ptr != NULL ? "%p" : "???", CLevel2_ptr != NULL ? c_green : c_red, CLevel2_ptr);
		PrintSpaces(consoleWidth - GetCurrentCursorPosX());
		printf("\n");
		PrintCustom("[.] CGame->CLevel2->CGSObject addr: ", c_brightwhite);
		PrintCustom(CGSObject_ptr != NULL ? "%p" : "???", CGSObject_ptr != NULL ? c_green : c_red, CGSObject_ptr);
		PrintSpaces(consoleWidth - GetCurrentCursorPosX());
		printf("\n");
		PrintCustom("[.] CGame->CLevel2->CGSObject->PlayerState addr: ", c_brightwhite);
		PrintCustom(PlayerState_ptr != NULL ? "%p" : "???", PlayerState_ptr != NULL ? c_green : c_red, PlayerState_ptr);
		PrintSpaces(consoleWidth - GetCurrentCursorPosX());
		printf("\n");
		PrintCustom("[.] CGame->CLevel2->CGSObject->PlayerState->PlayerVariables addr: ", c_brightwhite);
		PrintCustom(IsValidPtr(PlayerVariablesInstance) ? "%p" : "???", IsValidPtr(PlayerVariablesInstance) ? c_green : c_red, PlayerVariablesInstance);
		PrintSpaces(consoleWidth - GetCurrentCursorPosX());
		printf("\n");

		const COORD endCursorLocation = GetCurrentCursorPos();

		PrintSpaces(consoleWidth);
		printf("\n");
		PrintSpaces(consoleWidth);

		SetConsoleCursorPosition(hConsole, endCursorLocation);
	}
}