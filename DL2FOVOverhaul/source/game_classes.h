#pragma once
#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}
#define DEFINE_MEMBER_0(type, name) struct {type name;}

class PlayerVariables {
public:
	union {
		//				Type		Name		Offset
		DEFINE_MEMBER_N(float, AggresionPerHit, 0x8);
		DEFINE_MEMBER_N(float, DEF_AggresionPerHit, 0xC);
		DEFINE_MEMBER_N(float, AggresionCooldownOnMax, 0x20);
		DEFINE_MEMBER_N(float, DEF_AggresionCooldownOnMax, 0x24);
		DEFINE_MEMBER_N(float, AggresionDecreseTime, 0x38);
		DEFINE_MEMBER_N(float, DEF_AggresionDecreseTime, 0x3C);
		DEFINE_MEMBER_N(float, AngleToChangeHands, 0x5F8);
		DEFINE_MEMBER_N(float, DEF_AngleToChangeHands, 0x5FC);
		DEFINE_MEMBER_N(float, CameraDefaultFOV, 0xCC8);
		DEFINE_MEMBER_N(float, DEF_CameraDefaultFOV, 0xCCC);
		DEFINE_MEMBER_N(float, CameraDefaultFOVReduction, 0xCE0);
		DEFINE_MEMBER_N(float, DEF_CameraDefaultFOVReduction, 0xCE4);
		DEFINE_MEMBER_N(float, CameraDefaultFOVReducedSpringTime, 0xCF8);
		DEFINE_MEMBER_N(float, DEF_CameraDefaultFOVReducedSpringTime, 0xCFC);
		DEFINE_MEMBER_N(float, CameraDefaultFOVRemoveReductionSpringTime, 0xD10);
		DEFINE_MEMBER_N(float, DEF_CameraDefaultFOVRemoveReductionSpringTime, 0xD14);
		DEFINE_MEMBER_N(float, PerfectBlockTimingEnd, 0x4120);
		DEFINE_MEMBER_N(float, DEF_PerfectBlockTimingEnd, 0x4124);
		DEFINE_MEMBER_N(float, RecoilOnHitBlock, 0x4150);
		DEFINE_MEMBER_N(float, DEF_RecoilOnHitBlock, 0x4154);
		DEFINE_MEMBER_N(bool, CanBlockWithFistsOtherWeapons, 0x41E8);
		DEFINE_MEMBER_N(bool, DEF_CanBlockWithFistsOtherWeapons, 0x41E9);
		DEFINE_MEMBER_N(bool, NightVisionEnabled, 0x41F8);
		DEFINE_MEMBER_N(bool, DEF_NightVisionEnabled, 0x41F9);
		DEFINE_MEMBER_N(float, NightVisionPower, 0x4208);
		DEFINE_MEMBER_N(float, DEF_NightVisionPower, 0x420C);
		DEFINE_MEMBER_N(bool, GREDetectorWithoutFacility, 0x4740);
		DEFINE_MEMBER_N(bool, DEF_GREDetectorWithoutFacility, 0x4741);
		DEFINE_MEMBER_N(float, GREDetectorRangeNearby, 0x4750);
		DEFINE_MEMBER_N(float, DEF_GREDetectorRangeNearby, 0x4754);
		DEFINE_MEMBER_N(float, NightRunnerSegment1StartVal, 0x48E8);
		DEFINE_MEMBER_N(float, DEF_NightRunnerSegment1StartVal, 0x48EC);
		DEFINE_MEMBER_N(float, ShimmyBackwardRange, 0x56B0);
		DEFINE_MEMBER_N(float, DEF_ShimmyBackwardRange, 0x56B4);
		DEFINE_MEMBER_N(float, StaminaRegeneration, 0x5D48);
		DEFINE_MEMBER_N(float, DEF_StaminaRegeneration, 0x5D4C);
		DEFINE_MEMBER_N(bool, StaminaRegenerationRelativeEnabled, 0x5DA8);
		DEFINE_MEMBER_N(bool, DEF_StaminaRegenerationRelativeEnabled, 0x5DA9);
		DEFINE_MEMBER_N(bool, StaminaRegenerationRelativeForAllWeapons, 0x5DB8);
		DEFINE_MEMBER_N(bool, DEF_StaminaRegenerationRelativeForAllWeapons, 0x5DB9);
		DEFINE_MEMBER_N(float, StaminaRegenerationFactorSlow, 0x5DC8);
		DEFINE_MEMBER_N(float, DEF_StaminaRegenerationFactorSlow, 0x5DCC);
		DEFINE_MEMBER_N(float, StaminaRegenerationFromDuck, 0x5DE0);
		DEFINE_MEMBER_N(float, DEF_StaminaRegenerationFromDuck, 0x5DE4);
		DEFINE_MEMBER_N(float, StaminaRegenerationFactorStand, 0x5DF8);
		DEFINE_MEMBER_N(float, DEF_StaminaRegenerationFactorStand, 0x5DFC);
		DEFINE_MEMBER_N(float, StaminaRegenerationFactorStandDalay, 0x5E10);
		DEFINE_MEMBER_N(float, DEF_StaminaRegenerationFactorStandDalay, 0x5E14);
		DEFINE_MEMBER_N(float, WallRunTimeTolleranceFromSprint, 0x67D8);
		DEFINE_MEMBER_N(float, DEF_WallRunTimeTolleranceFromSprint, 0x67DC);
		DEFINE_MEMBER_N(bool, GrapplePullToEnemyEnabled, 0x7D28);
		DEFINE_MEMBER_N(bool, DEF_GrapplePullToEnemyEnabled, 0x7D29);
		DEFINE_MEMBER_N(float, VeryLargeJumpDistance, 0x7D50);
		DEFINE_MEMBER_N(float, DEF_VeryLargeJumpDistance, 0x7D54);
	};
};

class PlayerState {
public:
	union {
		//				Type		Name		Offset
		DEFINE_MEMBER_N(PlayerVariables*, PlayerVariables_ptr, 0x100);
	};
};

class CGSObject {
public:
	union {
		//				Type		Name		Offset
		DEFINE_MEMBER_N(PlayerState*, PlayerState_ptr, 0x20);
	};
};

class CLevel2 {
public:
	union {
		//				Type		Name		Offset
		DEFINE_MEMBER_N(CGSObject*, CGSObject_ptr, 0x28);
	};
};

class CameraFPPDI {
public:
	union {
		//				Type		Name		Offset
		DEFINE_MEMBER_N(float, FOV, 0x9C);
	};
};

class CoBaseCameraProxy {
public:
	union {
		//				Type		Name		Offset
		DEFINE_MEMBER_N(CameraFPPDI*, CameraFPPDI_ptr, 0x150);
	};
};

class FreeCamera {
public:
	union {
		//				Type		Name		Offset
		DEFINE_MEMBER_N(CoBaseCameraProxy*, CoBaseCameraProxy_ptr, 0x18);
	};
};

class CBaseCamera {
public:
	union {
		//				Type		Name		Offset
		DEFINE_MEMBER_N(FreeCamera*, FreeCamera_ptr, 0x20);
	};
};

class CLevel {
public:
	union {
		//				Type		Name		Offset
		DEFINE_MEMBER_N(CBaseCamera*, CBaseCamera_ptr, 0x28);
	};
};

class CChromeSync {
public:
	union {
		//				Type		Name		Offset
		DEFINE_MEMBER_N(int*, CEngineSyncDI_ptr, 0x20);
	};
};

class CVideoSettings {
public:
	union {
		//				Type		Name		Offset
		DEFINE_MEMBER_N(float, ExtraFOV, 0x78);
	};
};

class CGame {
public:
	union {
		//				Type		Name		Offset
		DEFINE_MEMBER_N(CVideoSettings*, CVideoSettings_ptr, 0x28);
		DEFINE_MEMBER_N(CChromeSync*, CChromeSync_ptr, 0x340);
		DEFINE_MEMBER_N(CLevel*, CLevel_ptr, 0x388);
		DEFINE_MEMBER_N(CLevel2*, CLevel2_ptr, 0x648);
	};
};

class CLobbySteam {
public:
	union {
		//				Type		Name		Offset
		DEFINE_MEMBER_N(CGame*, CGame_ptr, 0xF8);
	};
};

class CLobbySteam_loc {
public:
	union {
		//				Type		Name		Offset
		DEFINE_MEMBER_0(CLobbySteam*, CLobbySteam_ptr);
		DEFINE_MEMBER_N(CLobbySteam*, CLobbySteam_ptr2, 0x38);
	};
};