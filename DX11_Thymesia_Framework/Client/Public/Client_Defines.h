#pragma once

/////////////////////////////////////////// DEBUG MODE DEFINES ///////////////////////////////////////////
#ifdef _DEBUG


#pragma region System
//#define _BAKE_MIPMAPS_define
#define _USE_THREAD_
#define _144HZ_
//#define _RENDER_FPS_
#define _Actor_Culling_
#define _INSTANCE_CULLING_
#define _OVERDRIVE_LOAD_EFFECTGROUP_
//#define _LOAD_CAPTURED_RESOURCE_
#pragma endregion // System

// #define _SKYBOX_

#define _USE_GRAVITY_
#define _LIFEGUARD_FOR_FALL_

//#define _DEBUG_COUT_

#pragma region Map
//#define _BRIGHT_LIGHT_
#define _GENERATE_PROP_COLLIDER_ true
//#define _TEST_DYNAMIC_PROPS_
#pragma endregion // Map

#pragma region Tool
//#define _EFFECT_TOOL_
#define _ANIMATION_TOOL_WEAPON_
#pragma endregion // Tool
  
#pragma region Effects
//#define _DAGGER_TRAIL_
#define _CORVUS_EFFECT_
//#define _VARG_EFFECT_
//#define _BAT_EFFECT_
//#define _URD_EFFECT_
#define _INTERACTION_EFFECT_
#pragma endregion // Effects

#endif // _DEBUG
/////////////////////////////////////////// DEBUG MODE DEFINES ///////////////////////////////////////////

/////////////////////////////////////////// RELEASE MODE DEFINES ///////////////////////////////////////////
#ifdef NDEBUG

#pragma region System
#define _USE_THREAD_
#define _144HZ_
#define _RENDER_FPS_
#define _Actor_Culling_
//#define _INSTANCE_CULLING_
//#define _LOAD_CAPTURED_RESOURCE_
#define _OVERDRIVE_LOAD_EFFECTGROUP_
#pragma endregion // System

//#define _SKYBOX_

#define _USE_GRAVITY_
#undef _LIFEGUARD_FOR_FALL_

#pragma region Map
#undef _BRIGHT_LIGHT_
#define _GENERATE_PROP_COLLIDER_ true
#undef _TEST_DYNAMIC_PROPS_
#pragma endregion // Map

#pragma region Effects
#undef _BAKE_EFFECTMESH_FBX_
//#define _EFFECT_TOOL_
#undef _DAGGER_TRAIL_
#define _CORVUS_EFFECT_
#define _VARG_EFFECT_
#define	_BAT_EFFECT_
#define _URD_EFFECT_
#define _INTERACTION_EFFECT_
#pragma endregion // Effects

#endif // NDEBUG
/////////////////////////////////////////// RELEASE MODE DEFINES ///////////////////////////////////////////

/////////////////////////////////////////// For. Effect Tool ///////////////////////////////////////////
#ifdef _EFFECT_TOOL_

#pragma region System
#define _USE_THREAD_
#define _144HZ_
#define _RENDER_FPS_
#undef _INSTANCE_CULLING_
#undef _LOAD_CAPTURED_RESOURCE_
#pragma endregion // System

#undef _USE_GRAVITY_
#undef _LIFEGUARD_FOR_FALL_

#pragma region Console Outputs
#undef _DEBUG_COUT_
#pragma endregion // Console Outputs

#pragma region Map
#undef _BRIGHT_LIGHT_
#undef _GENERATE_PROP_COLLIDER_ true
#pragma endregion // Map

#pragma region Tool
#define _CORVUS_MODEL_
#define _BOSS_MODEL_
#define _ELITE_MOB_MODEL_
#define _NORMAL_MOB_MODEL_
// #define _ANIMATION_TOOL_WEAPON_
#undef _BAKE_EFFECTMESH_FBX_
#pragma endregion // Tool

#pragma region Effects
#undef _DAGGER_TRAIL_
#pragma endregion // Effects


#endif // _EFFECT_TOOL_
/////////////////////////////////////////// For. Effect Tool ///////////////////////////////////////////

#define CLIENT_DECLATION_UI class CUI; class CCustomUI; class CProgressBar; class CHUD_Hover;
#define ENGINE_DECLATION_UI class CTexture; class CShader;
#define ADD_STATIC_CUSTOMUI GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC)

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

using namespace Client;
namespace fs = std::filesystem;

#define EVENT_DRAW_EDITER 1
#define EVENT_INIT_EDITER 0
