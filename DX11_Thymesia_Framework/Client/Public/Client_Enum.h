#pragma once

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
		LEVEL_STAGE1,
		LEVEL_STAGE2,
		LEVEL_STAGE3,
		LEVEL_STAGE4,
		LEVEL_EDIT,
		LEVEL_TEST,
		LEVEL_END
	};
	enum class TIMER { TIMER_DEFAULT, TIMER_FRAME, TIMER_HYPERSPACE, TIMER_END };
	enum class FONT_INDEX { DREAM, PRETENDARD, HEIROLIGHT, FONT_END };
	enum class EVENT_TYPE
	{
		ON_HITPLAYER,
		ON_HITMONSTER,
		ON_EDITINIT,
		ON_EDITDRAW,
		ON_EDITDRAW_NONE,
		ON_EDITDRAW_ACCEPT,
		ON_EDITDRAW_SUB,
		ON_EDIT_PHYSXINFO,
		ON_EDIT_PHYSXINFO_N,
		ON_EDIT_DELETE,
		ON_EDIT_UDATE,
		ON_ANIMATIONEND,
		ON_MOVING,
		ON_MONSTERDEAD,
		ON_MONSTERBEGIN,
		ON_TRIGGER,
		ON_FIRSTHIT,
		ON_EXCUTION_NORMOB,
		ON_GROGGY,
		ON_LADDERMOVEUP,
		ON_CATCH,
		ON_ENTER_SECTION,
		ON_EXIT_SECTION,
		ON_VARGEXECUTION,
		ON_JOKEREXECUTION,
		ON_SITUP,
		ON_CONSOLE_ENABLE,
		ON_CONSOLE_DISABLE,
		ON_DIE,
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
		UI,
		LAYER_END
	};

	enum class OBJECT_LAYER
	{
		PLAYER,
		MONSTER,
		ALIVE_MONSTER,
		FADEMASK,
		GROOGYMOSNTER,
		BATTLEUI,
		INTERACTIONUI,
		PLAYERHUD,
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
		PLAYER_ATTACK,
		MONSTER_ATTACK,
		EDIT_NAVIMESH,
		DYNAMIC_PROP,
		TRIGGER,
		LADDER_UP,
		LADDER_DOWN,
		DOOR,
		ELEVATOR,
		CHECKPOINT,
		PLAYER_BATCOL,
		ITEM,

		LAYER_END
	};

	enum class PHYSX_COLLISION_LAYER
	{
		NONE,
		PLAYER,
		PLAYER_WEAPON,
		MONSTER,
		TERRAIN,
		STATIC_PROP,
		DYNAMIC_PROP,
		DYNAMIC_PEICE,
		GROUND,
		CAMERA,
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
		WEAPON_JOKER,
		WEAPON_GREATSWORD,
		MON_WEAPON_END
	};

	enum class  MONSTERTYPE {
		START_NORMAL_MONSTER = 0,
		AXEMAN,			   // 도끼든남자 방패든도끼든남자
		KNIFEWOMAN,		   // 기본단검여자
		SKULL,			   // 기본해골
		GARDENER,		   // 낫든몬스터
		SHIELDAXEMAN,	   // 실드도끼남자
		BALLOON,		   // 풍선
		ENHANCE_GARDENER,  // 강화 가드너


		START_ELITE_MONSTER = 10000,
		JOKER,

		STARTBOSS_MONSTER = 20000,
		VARG, // 보스바그
		BAT, // 보스박쥐
		URD, //보스우두루

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

	enum class CORSS_RESULT
	{
		LEFT,
		RIGHT,
		CORSS_END
	};

	enum class DOT_RESULT
	{
		LEFT,
		MID,
		RIGHT,
		DOT_RESULT_END
	};

	enum class PARRY_SUCCESS
	{
		LEFT,
		LEFTUP,
		RIGHT,
		RIGHTUP,
		FAIL,
		PARRY_SUCCESS_END
	};

	enum class	TALENT_RESULT
	{
		FAILED,
		NOT_ENOUGHTPOINT,
		USING_ATHORTREE,
		SUCCESS,
		SUBSCRIPTPOINT,
		RESULT_END
	};


	//enum class  RAREMONSTERTYPE {
	//	RMON_ARMOR,
	//	RMON_KNIFEWOMAN,
	//	RMON, SKULL,
	//	RMON_END
	//};

	enum class TALENT_EFFECT_FLAG
	{
		//검파트
		NONE = 0,
		SABER_ATTACK_LV2 = (1 << 1),
		AVOID_SLASH_LV1 = (1 << 2),
		AVOID_SLASH_LV2 = (1 << 3),
		AVOID_THRUST_LV1 = (1 << 4),
		AVOID_THRUST_LV2 = (1 << 5),

		JUMP_ATTACK_LV2 = (1 << 6),
		JUMP_ATTACK_LV3 = (1 << 7),

		SHARP_WEAPON_LV1 = (1 << 8),
		SHARP_WEAPON_LV2 = (1 << 9),

		ENERGISED_WEAPON_LV1 = (1 << 10),
		ENERGISED_WEAPON_LV2 = (1 << 11),

		EXECUTION_LV1 = (1 << 12),
		HEALING_EXELCUTION_LV1 = (1 << 13),
		HEALING_EXELCUTION_LV2 = (1 << 14),

		PARRING_LV1 = (1 << 15),
		PARRING_LV2 = (1 << 16),
		RECKLESS_PARRING_LV1 = (1 << 17),
		RECKLESS_PARRING_LV2 = (1<< 18),

		AVOID_LV1 = (1 << 19),
		SHORT_AVOID_LV1 = (1 << 20),
		SHORT_AVOID_LV2 = (1 << 21),
		LONG_AVOID_LV1 = (1 << 22),
		LONG_AVOID_LV2 = (1 << 23),

		CLAW_LV1 = (1 << 24),
		LONG_CLAW_LV1 = (1 << 25),
		LONG_CLAW_LV2 = (1 << 26),
		SHORT_CLAW_LV1 = (1 << 27),
		SHORT_CLAW_LV2 = (1 << 28),

		PREADAOTR_LV1 = (1 << 29),
		PREADAOTR_LV2 = (1 << 30),

		FEATHER_FLYING_LV1 = (1 << 31),
		FEATHER_FLYING_LV2 = (1 << 32),
		FEATHER_FLYING_LV3 = (1 << 33),
		FEATHER_FLYING_LV4 = (1 << 34),


		ABUNDANT_FEATHER_LV1 = (1 << 35),
		ABUNDANT_FEATHER_LV2 = (1 << 36),
		ABUNDANT_FEATHER_LV3 = (1 << 37),
		
		GROWING_FEATHER_LV1 = (1 << 38),
		GROWING_FEATHER_LV2 = (1 << 39),
		GROWING_FEATHER_LV3 = (1 << 40),










		FLAG_END
	};

	enum class TALENT_NAME
	{
		NORSWORDLV1,
		NORSWORDLV2,
		AVOIDSLASHLV1,
		AVOIDSLASHLV2,
		AVOIDTHRUSTLV1,
		AVOIDTHRUSTLV2,

		JUMPATTACKLV1,
		JUMPATTACKLV2,
		JUMPATTACKLV3,

		EXECUTION,
		HEALINGEXECUTIONLV1,
		HEALINGEXECUTIONLV2,

		SHARPWEAPONLV1,
		SHARPWEAPONLV2,

		ENERGISEDWEAPONLV1,
		ENERGISEDWEAPONLV2,

		PARRING_LV1 ,
		PARRING_LV2 ,
		RECKLESS_PARRING_LV1,
		RECKLESS_PARRING_LV2,

		AVOID_LV1 ,
		SHORT_AVOID_LV1     ,
		SHORT_AVOID_LV2     ,
		LONG_AVOID_LV1      ,
		LONG_AVOID_LV2      ,

		CLAW_LV1            ,
		LONG_CLAW_LV1       ,
		LONG_CLAW_LV2       ,
		SHORT_CLAW_LV1      ,
		SHORT_CLAW_LV2      ,

		PREADAOTR_LV1       ,
		PREADAOTR_LV2       ,

		FEATHER_FLYING_LV1  ,
		FEATHER_FLYING_LV2  ,
		FEATHER_FLYING_LV3  ,
		FEATHER_FLYING_LV4  ,


		ABUNDANT_FEATHER_LV1,
		ABUNDANT_FEATHER_LV2,
		ABUNDANT_FEATHER_LV3,

		GROWING_FEATHER_LV1 ,
		GROWING_FEATHER_LV2 ,
		GROWING_FEATHER_LV3 ,

		TALENT_NAME_END




	};

	enum class COLISIONOBJECT_FLAG
	{
		LADDERUP = (1 << 0),
		LADDERDOWN = (1 << 1),
		ELEVATOR = (1 << 2),
		DOOR = (1 << 3),
		CHECKPOINT = (1 << 4),
		ITEM = (1 << 5),
		FLAG_END
	};

	enum class STATE_FLAG
	{
		EXECUTABLE = (1 << 0),
		READY_SPECIAL_ATK = (1 << 1),
		FLAG_END
	};

	enum class COLOR_PALETTE
	{
		COLOR_WHITE,
		COLOR_BLACK,
		COLOR_RED,
		COLOR_GREEN,
		COLOR_BLUE
	};

	enum class SKILL_TYPE
	{
		SKILL_ATK,
		SKILL_BUFF,
		SKILL_TYPE_END
	};

	enum class SKILL_NAME
	{
		SKILL_VARGSWORD,
		SKILL_END
	};

	enum class ITEM_NAME
	{
		BASIL,
		THYME,
		CINNAMON,
		GARDEN_KEY,
		MEMORY01
	};

	enum class ITEM_TYPE
	{
		INGREDIENT,
		CONSUMPTION,
		SKILLPIECE,
		COMMON
	};

	enum class BUTTON_LEVEL
	{
		LEVEL1,
		LEVEL2,
		LEVEL3,
		LEVEL4,
		LEVEL5
	};
}
