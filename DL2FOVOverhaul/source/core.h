#pragma once
#include "game_classes.h"

extern std::string configStatus;
extern std::string configError;

// Pointers for shortening/cleaning the code a bit
extern CLobbySteam_loc* CLobbySteamLoc;
extern CGame* CGameInstance;
extern PlayerVariables* PlayerVariablesInstance;
extern CVideoSettings* CVideoSettingsInstance;
extern CameraFPPDI* CameraFPPDIInstance;

extern LPVOID CLobbySteam_ptr;
extern LPVOID CLevel_ptr, CBaseCamera_ptr, FreeCamera_ptr, CoBaseCameraProxy_ptr;
extern LPVOID CLevel2_ptr, CGSObject_ptr, PlayerState_ptr;