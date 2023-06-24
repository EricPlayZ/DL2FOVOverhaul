#include "print.h"
#include "core.h"
#include "memory.h"
#include "time_tools.h"

std::chrono::steady_clock::time_point refreshConsoleTimestamp{};
const int refreshConsoleIntervalMs = 2000;

LPVOID CLobbySteam_ptr = NULL;
LPVOID CGame_ptr = NULL;

LPVOID CVideoSettings_ptr = NULL;

LPVOID CLevel_ptr = NULL;
LPVOID CBaseCamera_ptr = NULL;
LPVOID FreeCamera_ptr = NULL;
LPVOID CoBaseCameraProxy_ptr = NULL;
LPVOID CameraFPPDI_ptr = NULL;

LPVOID CLevel2_ptr = NULL;
LPVOID CGSObject_ptr = NULL;
LPVOID PlayerState_ptr = NULL;
LPVOID PlayerVariables_ptr = NULL;

void ResetPointersCLobbySteam(int level = 0) {
	switch (level) {
	case 0: {
		CLobbySteam_ptr = NULL;
		CGame_ptr = NULL;
		CVideoSettings_ptr = NULL;
		break;
	}
	case 1: {
		CGame_ptr = NULL;
		CVideoSettings_ptr = NULL;
		break;
	}
	case 2: {
		CVideoSettings_ptr = NULL;
	}
	}
}

void ResetPointersCLevel(int level = 0) {
	switch (level) {
	case 0: {
		CLevel_ptr = NULL;
		CBaseCamera_ptr = NULL;
		FreeCamera_ptr = NULL;
		CoBaseCameraProxy_ptr = NULL;
		CameraFPPDI_ptr = NULL;
		break;
	}
	case 1: {
		CBaseCamera_ptr = NULL;
		FreeCamera_ptr = NULL;
		CoBaseCameraProxy_ptr = NULL;
		CameraFPPDI_ptr = NULL;
		break;
	}
	case 2: {
		FreeCamera_ptr = NULL;
		CoBaseCameraProxy_ptr = NULL;
		CameraFPPDI_ptr = NULL;
		break;
	}
	case 3: {
		CoBaseCameraProxy_ptr = NULL;
		CameraFPPDI_ptr = NULL;
		break;
	}
	case 4: {
		CameraFPPDI_ptr = NULL;
		break;
	}
	}
}

void ResetPointersCLevel2(int level = 0) {
	switch (level) {
	case 0: {
		CLevel2_ptr = NULL;
		CGSObject_ptr = NULL;
		PlayerState_ptr = NULL;
		PlayerVariables_ptr = NULL;
		break;
	}
	case 1: {
		CGSObject_ptr = NULL;
		PlayerState_ptr = NULL;
		PlayerVariables_ptr = NULL;
		break;
	}
	case 2: {
		PlayerState_ptr = NULL;
		PlayerVariables_ptr = NULL;
		break;
	}
	case 3: {
		PlayerVariables_ptr = NULL;
		break;
	}
	}
}

void UpdatePointers() {
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
	CGame_ptr = CLobbySteamLoc->CLobbySteam_ptr->CGame_ptr;

	if (IsAddressValid(CGameInstance->CVideoSettings_ptr))
		CVideoSettings_ptr = CGameInstance->CVideoSettings_ptr;
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
						CameraFPPDI_ptr = CGameInstance->CLevel_ptr->CBaseCamera_ptr->FreeCamera_ptr->CoBaseCameraProxy_ptr->CameraFPPDI_ptr;
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
					PlayerVariables_ptr = CGameInstance->CLevel2_ptr->CGSObject_ptr->PlayerState_ptr->PlayerVariables_ptr;
				} else
					ResetPointersCLevel2(3);
			} else
				ResetPointersCLevel2(2);
		} else
			ResetPointersCLevel2(1);
	} else
		ResetPointersCLevel2();
}

void DrawConsoleOut() {
	if (since(refreshConsoleTimestamp).count() < refreshConsoleIntervalMs)
		return;

	UpdatePointers();

	system("cls");

	const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int consoleWidth, consoleHeight;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	std::string fovText = "[.] ExtraFOV: ";
	fovText.append((CVideoSettings_ptr != NULL ? std::to_string(CVideoSettingsInstance->ExtraFOV) : "???") + "; FOV: ");
	fovText.append((CameraFPPDI_ptr != NULL ? std::to_string(CameraFPPDIInstance->FOV) : "???"));

	COORD fovTextPos{};
	fovTextPos.X = (consoleWidth - fovText.length()) / 2;
	SetConsoleCursorPosition(hConsole, fovTextPos);

	PrintCustom("[.] ExtraFOV: ", c_brightwhite);
	if (CVideoSettings_ptr != NULL)
		PrintCustom("%f", c_green, CVideoSettingsInstance->ExtraFOV);
	else
		PrintCustom("???", c_red);
	PrintCustom("; FOV: ", c_brightwhite);
	if (CameraFPPDI_ptr != NULL)
		PrintCustom("%f", c_green, CameraFPPDIInstance->FOV);
	else
		PrintCustom("???", c_red);

	printf("\n\n");

	PrintCustom("[.] CLobbySteam_loc addr: ", c_brightwhite);
	PrintCustom(CLobbySteamLoc != NULL ? "%p" : "???", CLobbySteamLoc != NULL ? c_green : c_red, CLobbySteamLoc);
	printf("\n");
	PrintCustom("[.] CLobbySteam addr: ", c_brightwhite);
	PrintCustom(CLobbySteam_ptr != NULL ? "%p" : "???", CLobbySteam_ptr != NULL ? c_green : c_red, CLobbySteam_ptr);
	printf("\n");
	PrintCustom("[.] CLobbySteam->CGame addr: ", c_brightwhite);
	PrintCustom(CGame_ptr != NULL ? "%p" : "???", CGame_ptr != NULL ? c_green : c_red, CGame_ptr);

	printf("\n\n");

	PrintCustom("[.] CGame->CVideoSettings addr: ", c_brightwhite);
	PrintCustom(CVideoSettings_ptr != NULL ? "%p" : "???", CVideoSettings_ptr != NULL ? c_green : c_red, CVideoSettings_ptr);

	printf("\n\n");

	PrintCustom("[.] CGame->CLevel addr: ", c_brightwhite);
	PrintCustom(CLevel_ptr != NULL ? "%p" : "???", CLevel_ptr != NULL ? c_green : c_red, CLevel_ptr);
	printf("\n");
	PrintCustom("[.] CGame->CLevel->CBaseCamera addr: ", c_brightwhite);
	PrintCustom(CBaseCamera_ptr != NULL ? "%p" : "???", CBaseCamera_ptr != NULL ? c_green : c_red, CBaseCamera_ptr);
	printf("\n");
	PrintCustom("[.] CGame->CLevel->CBaseCamera->FreeCamera addr: ", c_brightwhite);
	PrintCustom(FreeCamera_ptr != NULL ? "%p" : "???", FreeCamera_ptr != NULL ? c_green : c_red, FreeCamera_ptr);
	printf("\n");
	PrintCustom("[.] CGame->CLevel->CBaseCamera->FreeCamera->CoBaseCameraProxy addr: ", c_brightwhite);
	PrintCustom(CoBaseCameraProxy_ptr != NULL ? "%p" : "???", CoBaseCameraProxy_ptr != NULL ? c_green : c_red, CoBaseCameraProxy_ptr);
	printf("\n");
	PrintCustom("[.] CGame->CLevel->CBaseCamera->FreeCamera->CoBaseCameraProxy->CameraFPPDI addr: ", c_brightwhite);
	PrintCustom(CameraFPPDI_ptr != NULL ? "%p" : "???", CameraFPPDI_ptr != NULL ? c_green : c_red, CameraFPPDI_ptr);

	printf("\n\n");

	PrintCustom("[.] CGame->CLevel2 addr: ", c_brightwhite);
	PrintCustom(CLevel2_ptr != NULL ? "%p" : "???", CLevel2_ptr != NULL ? c_green : c_red, CLevel2_ptr);
	printf("\n");
	PrintCustom("[.] CGame->CLevel2->CGSObject addr: ", c_brightwhite);
	PrintCustom(CGSObject_ptr != NULL ? "%p" : "???", CGSObject_ptr != NULL ? c_green : c_red, CGSObject_ptr);
	printf("\n");
	PrintCustom("[.] CGame->CLevel2->CGSObject->PlayerState addr: ", c_brightwhite);
	PrintCustom(PlayerState_ptr != NULL ? "%p" : "???", PlayerState_ptr != NULL ? c_green : c_red, PlayerState_ptr);
	printf("\n");
	PrintCustom("[.] CGame->CLevel2->CGSObject->PlayerState->PlayerVariables addr: ", c_brightwhite);
	PrintCustom(PlayerVariables_ptr != NULL ? "%p" : "???", PlayerVariables_ptr != NULL ? c_green : c_red, PlayerVariables_ptr);
	printf("\n");

	refreshConsoleTimestamp = std::chrono::steady_clock::now();
}