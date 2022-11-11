#pragma once

enum class RENDERGROUP { 
	RENDER_PRIORITY, 
	RENDER_AFTER_PRIORITY, 
	RENDER_SHADOWDEPTH,
	RENDER_NONALPHABLEND, 
	RENDER_NONLIGHT, 
	RENDER_ALPHABLEND, 
	RENDER_DISTORTION,
	RENDER_BEFOREUI, 
	RENDER_UI, 
	RENDER_AFTER_UI, 
	RENDER_AFTER_POSTPROCESS,
	RENDER_AFTER_POSTPROCESS_EFFECT,
	RENDER_FINAL,
	RENDER_END 
};
enum class MEMORY_TYPE { MEMORY_STATIC = 0, MEMORY_DYNAMIC, MEMORY_END };
enum class KEY_STATE {
	TAP, // 막 누른 시점
	HOLD,// 꾹 누르고 있음
	AWAY,// 막 뗀 시점
	NONE,// 걍 안눌린 상태
};
enum class KEY
{
	LEFT = 0,
	RIGHT,
	UP,
	DOWN,
	NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9, NUM0,
	F1, F2, F3, F4, F5,
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,
	ALT,
	LSHIFT,
	SPACE,
	CTRL,
	ENTER,
	ESC,
	TAB,
	LBUTTON,
	RBUTTON,
	BACKSPACE,
	DELETEKEY,
	LAST,
};

enum class MODEL_TYPE { NONANIM = 0, ANIM, NAVI, GROUND, TYPE_END };

enum class COLLISION_TYPE { SPHERE = 0, AABB, OBB, TYPE_END };

enum class LINEAR_TYPE { LNIEAR, SQRT, POW, POLY, TYPE_END };
enum FADER_TYPE
{
	FADER_OUT,
	FADER_IN,
	FADER_OUTINLOOPING,
	FADER_INOUTLOOPING,
	FADER_END
};

enum class INTERPOLATION
{
	LINEAR,
	SQRT,
	POW,
	INTERPOLATION_END
};

enum CHANNELID { BGM, ENVIRONMENT, PLAYER, PLAYERBULLET, EFFECT, EFFECT2, UI, SYSTEM_EFFECT, SYSTEM_EFFECT2, SYSTEM_EFFECT3, MAXCHANNEL };