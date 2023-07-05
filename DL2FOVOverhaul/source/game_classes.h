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
		DEFINE_MEMBER_N(float, CameraDefaultFOV, 0xCD8);
		DEFINE_MEMBER_N(float, DEF_CameraDefaultFOV, 0xCDC);
		DEFINE_MEMBER_N(float, CameraDefaultFOVReduction, 0xCF0);
		DEFINE_MEMBER_N(float, DEF_CameraDefaultFOVReduction, 0xCF4);
		DEFINE_MEMBER_N(float, CameraDefaultFOVReducedSpringTime, 0xD08);
		DEFINE_MEMBER_N(float, DEF_CameraDefaultFOVReducedSpringTime, 0xD0C);
		DEFINE_MEMBER_N(float, CameraDefaultFOVRemoveReductionSpringTime, 0xD20);
		DEFINE_MEMBER_N(float, DEF_CameraDefaultFOVRemoveReductionSpringTime, 0xD24);
	};
};

class PlayerState {
public:
	union {
		//				Type		Name		Offset
		DEFINE_MEMBER_N(PlayerVariables*, PlayerVariables_ptr, 0x278);
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
		DEFINE_MEMBER_N(CLevel*, CLevel_ptr, 0x370);
		DEFINE_MEMBER_N(CLevel2*, CLevel2_ptr, 0x630);
	};
};

class CLobbySteam {
public:
	union {
		//				Type		Name		Offset
		DEFINE_MEMBER_N(CGame*, CGame_ptr, 0xF8);
	};
};