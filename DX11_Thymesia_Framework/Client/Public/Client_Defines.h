#pragma once
#include "Engine_Defines.h"
#include "SMath.h"
#include <filesystem>
#include "Client_Structsh.h"

#include "imgui.h"

namespace Client 
{
	static const unsigned int g_iWinCX = 1600;
	static const unsigned int g_iWinCY = 900;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_LOBBY, LEVEL_GAMEPLAY, LEVEL_STAGE2, LEVEL_STAGE3, LEVEL_EDIT, LEVEL_END };
	enum class TIMER { TIMER_DEFAULT, TIMER_FRAME, TIMER_HYPERSPACE, TIMER_END };
	enum class FONT_INDEX { DREAM, FONT_END };
	enum class EVENT_TYPE
	{
		ON_HITPLAYER,
		ON_HITMONSTER,
		ON_EDITINIT,
		ON_EDITDRAW,
		ON_ANIMATIONEND,
		ON_MOVING,
		ON_MONSTERDEAD,
		ON_MONSTERBEGIN,
		ON_TRIGGER,
		ON_FIRSTHIT,
		EVENT_END
	};

	enum  class HIT_TYPE
	{
		NORMAL_HIT,
		UPPER_HIT,
		DOWN_HIT,
		WARNING,
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
		HYPERSPACE,
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
		PLAYER,
		MONSTER,
		TERRAIN,
		PLAYER_ATTACK,
		MONSTER_ATTACK,
		PLAYER_RIGIDBODY,
		MONSTER_RIGIDBODY,
		EDIT_NAVIMESH,
		TRIGGER,
		LAYER_END
	};

	enum class SHAKE_DIRECTION
	{
		RIGHT,
		UP,
		LOOK,
		DIRECTION_END
	};

}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

using namespace Client;
namespace fs = std::filesystem;


#define EVENT_DRAW_EDITER 1
#define EVENT_INIT_EDITER 0