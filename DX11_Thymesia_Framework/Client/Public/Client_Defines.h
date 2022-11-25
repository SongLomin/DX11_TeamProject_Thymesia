#pragma once
#include "Engine_Defines.h"
#include "SMath.h"
#include <filesystem>
#include "Client_Structs.h"
#include "Client_Presets.h"

#include "imgui.h"

#ifdef _DEBUG
//#define		_USE_THREAD_
//#define		_144HZ_
//#define		_LIFEGUARD_FOR_FALL_
// #define		_DEBUG_COUT_
//#define		_DEBUG_COUT_JOJO
//#define		_STAGE_1_
//#define		_STAGE_2_
 //#define		_STAGE_1_MONSTER_
//#define		_INSTANCE_CULLING_
// #define		_GENERATE_PROP_COLLIDER_ true
 //#define		_USE_GRAVITY_
// #define		_DAGGER_TRAIL_
// UI제외한 오브젝트 생성 X
#define		_ONLY_UI_
#endif // _DEBUG

#ifdef NDEBUG
#define		_INSTANCE_CULLING_
#define		_144HZ_
#define		_DAGGER_TRAIL_
#define		_USE_THREAD_
#define		_STAGE_2_
#define		_LIFEGUARD_FOR_FALL_
#define		_STAGE_1_MONSTER_
#define		_USE_GRAVITY_
#define		_GENERATE_PROP_COLLIDER_ true
#endif // NDEBUG


#define CLIENT_DECLATION_UI class CUI; class CCustomUI; class CProgressBar; class CHUD_Hover;

#define ENGINE_DECLATION_UI class CTexture; class CShader;   



namespace Client 
{
	static const _uint	g_iScreenCX = GetSystemMetrics(SM_CXSCREEN);
	static const _uint	g_iScreenCY = GetSystemMetrics(SM_CYSCREEN);
	static const _float g_fScreenCenterX = g_iScreenCX / 2.f;
	static const _float g_fScreenCenterY = g_iScreenCY / 2.f;
	static const _uint	g_iWinCX = 1600;
	static const _uint	g_iWinCY = 900;

	enum LEVEL { 
		LEVEL_STATIC, 
		LEVEL_LOADING, 
		LEVEL_LOGO, 
		LEVEL_LOBBY, 
		LEVEL_GAMEPLAY, 
		LEVEL_STAGE2, 
		LEVEL_STAGE3, 
		LEVEL_EDIT, 
		LEVEL_TEST,
		LEVEL_END };
	enum class TIMER { TIMER_DEFAULT, TIMER_FRAME, TIMER_HYPERSPACE, TIMER_END };
	enum class FONT_INDEX { DREAM, PRETENDARD, FONT_END };
	enum class EVENT_TYPE
	{
		ON_HITPLAYER,
		ON_HITMONSTER,
		ON_EDITINIT,
		ON_EDITDRAW,
		ON_EDITDRAW_NONE,
		ON_EDITDRAW_ACCEPT,
		ON_EDITDRAW_SUB,
		ON_EDITPICKING,
		ON_EDIT_PHYSXINFO,
		ON_EDIT_PHYSXINFO_N,
		ON_ANIMATIONEND,
		ON_MOVING,
		ON_MONSTERDEAD,
		ON_MONSTERBEGIN,
		ON_TRIGGER,
		ON_FIRSTHIT,
		ON_EXCUTION_NORMOB,
		ON_GROGGY,
		EVENT_END
	};

	enum  class HIT_TYPE
	{
		NORMAL_HIT,
		UPPER_HIT,
		DOWN_HIT,
		WARNING,
		LEFT_HIT,
		RIGHT_HIT,
		EXCTUE_HIT,
		PARRY_HIT,
		TYPE_END
	};

	enum  class EFFECT_TYPE
	{
		MESH,
		PARTICLE,
		TYPE_END
	};

	enum class TIMESCALE_LAYER
	{
		NONE,
		PLAYER,
		MONSTER,
		EDITER,
		LAYER_END
	};

	enum class OBJECT_LAYER
	{
		PLAYER,
		MONSTER,
		FADEMASK,
		GROOGYMOSNTER,
		BATTLEUI,
		SWAPUI,
		LAYER_END
	};

	enum class GAME_STATE
	{
		PEACE,
		BATTLE,
		BATTLE_STOP,
		BATTLE_END,
		STATE_END
	};

	enum class COLLISION_LAYER 
	{
		NONE,
		PLAYER,
		MONSTER,
		TERRAIN,
		PLAYER_ATTACK,
		MONSTER_ATTACK,
		PLAYER_RIGIDBODY,
		MONSTER_RIGIDBODY,
		EDIT_NAVIMESH,
		TRIGGER,
		STATIC_PROP,
		LAYER_END
	};

	enum class SHAKE_DIRECTION
	{
		RIGHT,
		UP,
		LOOK,
		DIRECTION_END
	};

	enum class  MONSTERWEAPONTYPE
	{
		WEAPON_AXE,
		WEAPON_KNIFE,
		WEAPON_BOSSVARG,
		WEAPON_SCYTHE,
		MON_WEAPON_END
	};

	enum class  MONSTERTYPE {
		AXEMAN,  // 도끼든남자 방패든도끼든남자
		KNIFEWOMAN, // 기본단검여자
		SKULL, // 기본해골
		GARDENER, // 낫든몬스터
		NMON_END
	};

	
	enum class NORMONSTERIDLETYPE {
		NORIDLE,
		SITIDLE,
		FIDGETIDLE,
		IDLEEND
	};

	enum class BOSSSTARTTYPE {
		BEGINSTART,
		NORMALSTART,
		BOSSSTARTEND
	};

	enum class ATTACK_OPTION {
		NONE,
		NORMAL,
		PLAGUE,
		SPECIAL_ATTACK,
		PARRY,
		OPTION_END
	};

	enum class PARRY_TYPE
	{
		PERFECT,
		NORMAL,
		FAIL,
		NONE,
		PARRY_TYPE_END
	};


	//enum class  RAREMONSTERTYPE {
	//	RMON_ARMOR,
	//	RMON_KNIFEWOMAN,
	//	RMON, SKULL,
	//	RMON_END
	//};

}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

using namespace Client;
namespace fs = std::filesystem;


#define EVENT_DRAW_EDITER 1
#define EVENT_INIT_EDITER 0

#ifdef _DEBUG
#define _SKYBOX_N
#endif // _DEBUG

#ifdef NDEBUG
#define _SKYBOX_
#endif // NDEBUG
